#include <SFML/Graphics.hpp>
#include "HexagonalGrid.h"
#include "SquareGrid.h"
#include "TriangularGrid.h"

//Langton's Ant constants
//square grids
//neighbor
const int NEIGHBOR_SQUARE_E = 3;
const int NEIGHBOR_SQUARE_N = 0;
const int NEIGHBOR_SQUARE_W = 1;
const int NEIGHBOR_SQUARE_S = 4;

//cell
const int MASK_SQUARE_CELL = 1;
const int STATE_SQUARE_ON = 1;

//ant
const int MASK_SQUARE_ANT = 6;
const int STATE_SQUARE_CLOCKWISE = 2;
const int STATE_SQUARE_COUNTER = 4;

//facing
const int MASK_SQUARE_FACING = 24;
const int STATE_SQUARE_E = 0;
const int STATE_SQUARE_N = 8;
const int STATE_SQUARE_W = 16;
const int STATE_SQUARE_S = 24;

//TODO: give this function its own class?
//TODO: parallelize this (that's the whole point of the assignment :P)
//does one time step for an outer-totalistic cellular automaton
void updateOuterTotalisticCellularAutomaton(Grid* grid, const int rulesIfOff[13], const int rulesIfOn[13], bool isMooreNeighborhood, bool shouldLoopHorizontally, bool shouldLoopVertically)
{
	grid->toggleGrid();

	for (int x = 0; x < grid->WIDTH; ++x)
	{
		for (int y = 0; y < grid->HEIGHT; ++y)
		{
			int neighborhood[13] = {0};
			int state = grid->getCellState(x, y);
			int total = -state;	//subtract center state from total in outer-totalistic

			grid->getNeighborhood(neighborhood, x, y, isMooreNeighborhood, shouldLoopHorizontally, shouldLoopVertically);

			for (int neighborState : neighborhood)
			{
				total += neighborState;
			}

			if (state == 0)
			{
				grid->setCellState(rulesIfOff[total], x, y);
			}
			else
			{
				grid->setCellState(rulesIfOn[total], x, y);
			}
		}
	}
}

//checks if ant at (x, y) with state state will be at (0, 0) on next time step
//used on square grids
bool isAntComingToThisSquare(int x, int y, int state)
{
	int ant = state & MASK_SQUARE_ANT;

	//ants move in the opposite direction when the cell is on
	if ((state & MASK_SQUARE_CELL) == STATE_SQUARE_ON)
	{
		ant ^= MASK_SQUARE_ANT;
	}

	if (ant == STATE_SQUARE_CLOCKWISE)
	{
		switch (state & MASK_SQUARE_FACING)
		{
		case STATE_SQUARE_E:
			++y;
			break;

		case STATE_SQUARE_N:
			++x;
			break;

		case STATE_SQUARE_W:
			--y;
			break;

		case STATE_SQUARE_S:
			--x;
			break;

		default:
			break;
		}
	}
	else if (ant == STATE_SQUARE_COUNTER)
	{
		switch (state & MASK_SQUARE_FACING)
		{
		case STATE_SQUARE_E:
			--y;
			break;

		case STATE_SQUARE_N:
			--x;
			break;

		case STATE_SQUARE_W:
			++y;
			break;

		case STATE_SQUARE_S:
			++x;
			break;

		default:
			break;
		}
	}

	return x == 0 && y == 0;
}

//TODO: give this function its own class?
//TODO: parallelize this (that's the whole point of the assignment :P)
//does one time step for a cellular automaton based on Langton's Ant
//ant rotates, recolors cell, and moves forward
//ants die if they stop on the same space
//state is 5 bits: FFAAC
//C0 = cell is off, C1 = cell is on
//AA00 = no ant, AA01 = clockwise ant, AA10 = counter-clockwise ant, A11 = no ant
//FF00 = facing E, FF01 = facing N, FF10 = facing W, FF11 = facing S
void updateLangtonsAntAutomaton(SquareGrid* grid, bool shouldLoopHorizontally, bool shouldLoopVertically)
{
	grid->toggleGrid();

	for (int x = 0; x < grid->WIDTH; ++x)
	{
		for (int y = 0; y < grid->HEIGHT; ++y)
		{
			int neighborhood[13] = {0};
			int state = grid->getCellState(x, y);
			int numAnts = 0;
			int ant = state & MASK_SQUARE_ANT;
			int facing = 0;
			int cell = state & MASK_SQUARE_CELL;

			grid->getNeighborhood(neighborhood, x, y, false, shouldLoopHorizontally, shouldLoopVertically);

			if (ant == STATE_SQUARE_CLOCKWISE || ant == STATE_SQUARE_COUNTER)
			{
				cell ^= MASK_SQUARE_CELL;
			}

			if (isAntComingToThisSquare(1, 0, neighborhood[NEIGHBOR_SQUARE_E]))
			{
				++numAnts;
				ant = neighborhood[NEIGHBOR_SQUARE_E] & MASK_SQUARE_ANT;
				facing = STATE_SQUARE_W;
			}

			if (isAntComingToThisSquare(0, -1, neighborhood[NEIGHBOR_SQUARE_N]))
			{
				++numAnts;
				ant = neighborhood[NEIGHBOR_SQUARE_N] & MASK_SQUARE_ANT;
				facing = STATE_SQUARE_S;
			}

			if (isAntComingToThisSquare(-1, 0, neighborhood[NEIGHBOR_SQUARE_W]))
			{
				++numAnts;
				ant = neighborhood[NEIGHBOR_SQUARE_W] & MASK_SQUARE_ANT;
				facing = STATE_SQUARE_E;
			}

			if (isAntComingToThisSquare(0, 1, neighborhood[NEIGHBOR_SQUARE_S]))
			{
				++numAnts;
				ant = neighborhood[NEIGHBOR_SQUARE_S] & MASK_SQUARE_ANT;
				facing = STATE_SQUARE_N;
			}

			if (numAnts == 1)
			{
				grid->setCellState(facing | ant | cell, x, y);
			}
			else
			{
				grid->setCellState(cell, x, y);
			}
		}
	}
}

//main function
//sets program up and handles events and drawing
int main()
{
	sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width - 100, sf::VideoMode::getDesktopMode().height - 100), "COP5570 Term Project", sf::Style::Close | sf::Style::Titlebar);
	bool isUnpaused = false;
	bool shouldStep = false;
	int numStepsPerFrame = 1;

	bool shouldLoopHorizontally = true;
	bool shouldLoopVertically = true;

	/*
	sf::Color colors[2] = {sf::Color(63, 63, 63), sf::Color(255, 255, 255)};
	bool isMooreNeighborhood = true;
	*/

	/*
	SquareGrid* grid = new SquareGrid(&window, 64, 64, colors);
	int rulesIfOff[13] = {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int rulesIfOn[13] = {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	*/

	/*
	HexagonalGrid* grid = new HexagonalGrid(&window, 64, 64, colors);
	int rulesIfOff[13] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int rulesIfOn[13] = {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
	*/

	/*
	TriangularGrid* grid = new TriangularGrid(&window, 64, 64, colors);
	int rulesIfOff[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int rulesIfOn[13] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	*/

	///*
	sf::Color colors[32];
	SquareGrid* grid;

	for (int state = 0; state < 32; ++state)
	{
		if ((state & MASK_SQUARE_ANT) == STATE_SQUARE_CLOCKWISE)
		{
			colors[state] = sf::Color(255, 0, 0);
		}
		else if ((state & MASK_SQUARE_ANT) == STATE_SQUARE_COUNTER)
		{
			colors[state] = sf::Color(0, 0, 255);
		}
		else if ((state & MASK_SQUARE_CELL) == STATE_SQUARE_ON)
		{
			colors[state] = sf::Color(255, 255, 255);
		}
		else
		{
			colors[state] = sf::Color(63, 63, 63);
		}
	}

	grid = new SquareGrid(&window, 64, 64, colors);
	grid->setCellState(STATE_SQUARE_CLOCKWISE | STATE_SQUARE_N, 21, 21);
	grid->setCellState(STATE_SQUARE_COUNTER | STATE_SQUARE_N, 43, 43);
	//*/

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::KeyPressed:
				switch (event.key.code)
				{
				case sf::Keyboard::Space:
					isUnpaused = !isUnpaused;
					break;

				case sf::Keyboard::Return:
					if (!isUnpaused)
					{
						shouldStep = true;
					}

					break;

				default:
					break;
				}

				break;

			default:
				break;
			}
		}

		/*
		if (isUnpaused)
		{
			for (int step = 0; step < numStepsPerFrame; ++step)
			{
				updateOuterTotalisticCellularAutomaton((Grid*)grid, rulesIfOff, rulesIfOn, isMooreNeighborhood, shouldLoopHorizontally, shouldLoopVertically);
			}
		}
		else if (shouldStep)
		{
			updateOuterTotalisticCellularAutomaton((Grid*)grid, rulesIfOff, rulesIfOn, isMooreNeighborhood, shouldLoopHorizontally, shouldLoopVertically);
			shouldStep = false;
		}
		*/

		///*
		if (isUnpaused)
		{
			for (int step = 0; step < numStepsPerFrame; ++step)
			{
				updateLangtonsAntAutomaton(grid, shouldLoopHorizontally, shouldLoopVertically);
			}
		}
		else if (shouldStep)
		{
			updateLangtonsAntAutomaton(grid, shouldLoopHorizontally, shouldLoopVertically);
			shouldStep = false;
		}
		//*/

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			sf::Vector2i gridPosition = grid->getGridPositionAtMouse();

			if (gridPosition.x > -1 && gridPosition.x < grid->WIDTH && gridPosition.y > -1 && gridPosition.y < grid->HEIGHT)
			{
				grid->setCellState(1, gridPosition.x, gridPosition.y);
			}
		}

		window.clear();
		grid->draw();
		window.display();
	}

	delete grid;
	return 0;
}