#include <SFML/Graphics.hpp>
#include "HexagonalGrid.h"
#include "SquareGrid.h"
#include "ThreadController.h"
#include "TriangularGrid.h"
#include "OuterTotalisticCA.h"

//TODO: give the CAs their own classes?
//TODO: parallelize them (that's the whole point of the assignment :P)

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
const int IS_SQUARE_ANT = 4;
const int REVERSE_SQUARE_ANT = 2;
const int MASK_SQUARE_ANT = 6;
const int STATE_SQUARE_CLOCKWISE = 4;
const int STATE_SQUARE_COUNTER = 6;

//facing
const int MASK_SQUARE_FACING = 24;
const int STATE_SQUARE_E = 0;
const int STATE_SQUARE_N = 8;
const int STATE_SQUARE_W = 16;
const int STATE_SQUARE_S = 24;

//hexagonal grids
//neighbor
const int NEIGHBOR_HEXAGONAL_E = 4;
const int NEIGHBOR_HEXAGONAL_NE = 1;
const int NEIGHBOR_HEXAGONAL_NW = 0;
const int NEIGHBOR_HEXAGONAL_W = 2;
const int NEIGHBOR_HEXAGONAL_SW = 5;
const int NEIGHBOR_HEXAGONAL_SE = 6;

//cell
const int MASK_HEXAGONAL_CELL = 1;
const int STATE_HEXAGONAL_ON = 1;

//ant
const int IS_HEXAGONAL_ANT = 8;
const int REVERSE_HEXAGONAL_ANT = 6;
const int MASK_HEXAGONAL_ANT = 14;
const int STATE_HEXAGONAL_SHARPCLOCKWISE = 8;
const int STATE_HEXAGONAL_SLIGHTCLOCKWISE = 10;
const int STATE_HEXAGONAL_SLIGHTCOUNTER = 12;
const int STATE_HEXAGONAL_SHARPCOUNTER = 14;

//facing
const int MASK_HEXAGONAL_FACING = 112;
const int STATE_HEXAGONAL_E = 0;
const int STATE_HEXAGONAL_NE = 16;
const int STATE_HEXAGONAL_NW = 32;
const int STATE_HEXAGONAL_W = 48;
const int STATE_HEXAGONAL_SW = 64;
const int STATE_HEXAGONAL_SE = 80;

//triangular grids
//neighbor
const int NEIGHBOR_TRIANGULAR_NE = 2;
const int NEIGHBOR_TRIANGULAR_N = 0;
const int NEIGHBOR_TRIANGULAR_NW = 0;
const int NEIGHBOR_TRIANGULAR_SW = 1;
const int NEIGHBOR_TRIANGULAR_S = 3;
const int NEIGHBOR_TRIANGULAR_SE = 3;

//cell
const int MASK_TRIANGULAR_CELL = 1;
const int STATE_TRIANGULAR_ON = 1;

//ant
const int IS_TRIANGULAR_ANT = 4;
const int REVERSE_TRIANGULAR_ANT = 2;
const int MASK_TRIANGULAR_ANT = 6;
const int STATE_TRIANGULAR_CLOCKWISE = 4;
const int STATE_TRIANGULAR_COUNTER = 6;

//facing
const int MASK_TRIANGULAR_FACING = 24;
const int STATE_TRIANGULAR_E = 0;
const int STATE_TRIANGULAR_NS = 8;
const int STATE_TRIANGULAR_W = 16;

//checks if ant at (x, y) with state state will be at (0, 0) on next time step
//used on square grids
bool isAntComingToThisSquare(int x, int y, int state)
{
	int ant = state & MASK_SQUARE_ANT;

	//ants move in the opposite direction when the cell is on
	if ((state & MASK_SQUARE_CELL) == STATE_SQUARE_ON)
	{
		ant ^= REVERSE_SQUARE_ANT;
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

//does one time step for a cellular automaton based on Langton's Ant
//used on square grids
//ant rotates, recolors cell, and moves forward
//ants die if they stop on the same space
//state is 5 bits: FFAAC
//C0 = cell is off, C1 = cell is on
//AA00 = no ant, A01 = no ant, AA10 = clockwise ant, AA11 = counter-clockwise ant
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

			if (ant & IS_SQUARE_ANT)
			{
				cell ^= MASK_SQUARE_CELL;
			}

			if ((neighborhood[NEIGHBOR_SQUARE_E] & IS_SQUARE_ANT) && isAntComingToThisSquare(1, 0, neighborhood[NEIGHBOR_SQUARE_E]))
			{
				++numAnts;
				ant = neighborhood[NEIGHBOR_SQUARE_E] & MASK_SQUARE_ANT;
				facing = STATE_SQUARE_W;
			}

			if ((neighborhood[NEIGHBOR_SQUARE_N] & IS_SQUARE_ANT) && isAntComingToThisSquare(0, -1, neighborhood[NEIGHBOR_SQUARE_N]))
			{
				++numAnts;
				ant = neighborhood[NEIGHBOR_SQUARE_N] & MASK_SQUARE_ANT;
				facing = STATE_SQUARE_S;
			}

			if ((neighborhood[NEIGHBOR_SQUARE_W] & IS_SQUARE_ANT) && isAntComingToThisSquare(-1, 0, neighborhood[NEIGHBOR_SQUARE_W]))
			{
				++numAnts;
				ant = neighborhood[NEIGHBOR_SQUARE_W] & MASK_SQUARE_ANT;
				facing = STATE_SQUARE_E;
			}

			if ((neighborhood[NEIGHBOR_SQUARE_S] & IS_SQUARE_ANT) && isAntComingToThisSquare(0, 1, neighborhood[NEIGHBOR_SQUARE_S]))
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

//checks if ant at (x, y) with state state will be at (0, 0) on next time step
//used on hexagonal grids
bool isAntComingToThisHexagon(int x, int y, int state, int otherRowEastOffset)
{
	int ant = state & MASK_HEXAGONAL_ANT;

	//ants move in the opposite direction when the cell is on
	if ((state & MASK_HEXAGONAL_CELL) == STATE_HEXAGONAL_ON)
	{
		ant ^= REVERSE_HEXAGONAL_ANT;
	}

	switch (ant)
	{
	case STATE_HEXAGONAL_SHARPCLOCKWISE:
		switch (state & MASK_HEXAGONAL_FACING)
		{
		case STATE_HEXAGONAL_E:
			++y;
			x += otherRowEastOffset - 1;
			break;

		case STATE_HEXAGONAL_NE:
			++y;
			x += otherRowEastOffset;
			break;

		case STATE_HEXAGONAL_NW:
			++x;
			break;

		case STATE_HEXAGONAL_W:
			--y;
			x += otherRowEastOffset;
			break;

		case STATE_HEXAGONAL_SW:
			--y;
			x += otherRowEastOffset - 1;
			break;

		case STATE_HEXAGONAL_SE:
			--x;
			break;

		default:
			break;
		}

		break;

	case STATE_HEXAGONAL_SLIGHTCLOCKWISE:
		switch (state & MASK_HEXAGONAL_FACING)
		{
		case STATE_HEXAGONAL_E:
			++y;
			x += otherRowEastOffset;
			break;

		case STATE_HEXAGONAL_NE:
			++x;
			break;

		case STATE_HEXAGONAL_NW:
			--y;
			x += otherRowEastOffset;
			break;

		case STATE_HEXAGONAL_W:
			--y;
			x += otherRowEastOffset - 1;
			break;

		case STATE_HEXAGONAL_SW:
			--x;
			break;

		case STATE_HEXAGONAL_SE:
			++y;
			x += otherRowEastOffset - 1;
			break;

		default:
			break;
		}

		break;

	case STATE_HEXAGONAL_SLIGHTCOUNTER:
		switch (state & MASK_HEXAGONAL_FACING)
		{
		case STATE_HEXAGONAL_E:
			--y;
			x += otherRowEastOffset;
			break;

		case STATE_HEXAGONAL_NE:
			--y;
			x += otherRowEastOffset - 1;
			break;

		case STATE_HEXAGONAL_NW:
			--x;
			break;

		case STATE_HEXAGONAL_W:
			++y;
			x += otherRowEastOffset - 1;
			break;

		case STATE_HEXAGONAL_SW:
			++y;
			x += otherRowEastOffset;
			break;

		case STATE_HEXAGONAL_SE:
			++x;
			break;

		default:
			break;
		}

		break;

	case STATE_HEXAGONAL_SHARPCOUNTER:
		switch (state & MASK_HEXAGONAL_FACING)
		{
		case STATE_HEXAGONAL_E:
			--y;
			x += otherRowEastOffset - 1;
			break;

		case STATE_HEXAGONAL_NE:
			--x;
			break;

		case STATE_HEXAGONAL_NW:
			++y;
			x += otherRowEastOffset - 1;
			break;

		case STATE_HEXAGONAL_W:
			++y;
			x += otherRowEastOffset;
			break;

		case STATE_HEXAGONAL_SW:
			++x;
			break;

		case STATE_HEXAGONAL_SE:
			--y;
			x += otherRowEastOffset;
			break;

		default:
			break;
		}

		break;

	default:
		break;
	}

	return x == 0 && y == 0;
}

//does one time step for a cellular automaton based on Langton's Ant
//used on hexagonal grids
//ant rotates, recolors cell, and moves forward
//ants die if they stop on the same space
//state is 7 bits: FFFAAAC
//C0 = cell is off, C1 = cell is on
//AA0** = no ant, A100 = sharp clockwise ant, A101 = slight clockwise ant, AA110 = slight counter-clockwise ant, AA111 = sharp counter-clockwise ant
//FF000 = facing E, FF001 = facing NE, FF010 = facing NW, FF011 = facing W, FF100 = facing SW, FF101 = facing SE, FF11* = undefined
void updateLangtonsAntAutomaton(HexagonalGrid* grid, bool shouldLoopHorizontally, bool shouldLoopVertically)
{
	grid->toggleGrid();

	for (int x = 0; x < grid->WIDTH; ++x)
	{
		for (int y = 0; y < grid->HEIGHT; ++y)
		{
			int neighborhood[13] = {0};
			int state = grid->getCellState(x, y);
			int numAnts = 0;
			int ant = state & MASK_HEXAGONAL_ANT;
			int facing = 0;
			int cell = state & MASK_HEXAGONAL_CELL;
			int otherRowEastOffset = y % 2;
			int otherRowWestOffset = otherRowEastOffset - 1;

			grid->getNeighborhood(neighborhood, x, y, false, shouldLoopHorizontally, shouldLoopVertically);

			if (ant & IS_HEXAGONAL_ANT)
			{
				cell ^= MASK_HEXAGONAL_CELL;
			}

			if ((neighborhood[NEIGHBOR_HEXAGONAL_E] & IS_HEXAGONAL_ANT) && isAntComingToThisHexagon(1, 0, neighborhood[NEIGHBOR_HEXAGONAL_E], otherRowEastOffset))
			{
				++numAnts;
				ant = neighborhood[NEIGHBOR_HEXAGONAL_E] & MASK_HEXAGONAL_ANT;
				facing = STATE_HEXAGONAL_W;
			}

			if ((neighborhood[NEIGHBOR_HEXAGONAL_NE] & IS_HEXAGONAL_ANT) && isAntComingToThisHexagon(otherRowEastOffset, -1, neighborhood[NEIGHBOR_HEXAGONAL_NE], 1 - otherRowEastOffset))
			{
				++numAnts;
				ant = neighborhood[NEIGHBOR_HEXAGONAL_NE] & MASK_HEXAGONAL_ANT;
				facing = STATE_HEXAGONAL_SW;
			}

			if ((neighborhood[NEIGHBOR_HEXAGONAL_NW] & IS_HEXAGONAL_ANT) && isAntComingToThisHexagon(otherRowWestOffset, -1, neighborhood[NEIGHBOR_HEXAGONAL_NW], 1 - otherRowEastOffset))
			{
				++numAnts;
				ant = neighborhood[NEIGHBOR_HEXAGONAL_NW] & MASK_HEXAGONAL_ANT;
				facing = STATE_HEXAGONAL_SE;
			}

			if ((neighborhood[NEIGHBOR_HEXAGONAL_W] & IS_HEXAGONAL_ANT) && isAntComingToThisHexagon(-1, 0, neighborhood[NEIGHBOR_HEXAGONAL_W], otherRowEastOffset))
			{
				++numAnts;
				ant = neighborhood[NEIGHBOR_HEXAGONAL_W] & MASK_HEXAGONAL_ANT;
				facing = STATE_HEXAGONAL_E;
			}

			if ((neighborhood[NEIGHBOR_HEXAGONAL_SW] & IS_HEXAGONAL_ANT) && isAntComingToThisHexagon(otherRowWestOffset, 1, neighborhood[NEIGHBOR_HEXAGONAL_SW], 1 - otherRowEastOffset))
			{
				++numAnts;
				ant = neighborhood[NEIGHBOR_HEXAGONAL_SW] & MASK_HEXAGONAL_ANT;
				facing = STATE_HEXAGONAL_NE;
			}

			if ((neighborhood[NEIGHBOR_HEXAGONAL_SE] & IS_HEXAGONAL_ANT) && isAntComingToThisHexagon(otherRowEastOffset, 1, neighborhood[NEIGHBOR_HEXAGONAL_SE], 1 - otherRowEastOffset))
			{
				++numAnts;
				ant = neighborhood[NEIGHBOR_HEXAGONAL_SE] & MASK_HEXAGONAL_ANT;
				facing = STATE_HEXAGONAL_NW;
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

//checks if ant at (x, y) with state state will be at (0, 0) on next time step
//used on up triangles on triangular grids
bool isAntComingToThisUpTriangle(int x, int y, int state)
{
	int ant = state & MASK_TRIANGULAR_ANT;

	//ants move in the opposite direction when the cell is on
	if ((state & MASK_TRIANGULAR_CELL) == STATE_TRIANGULAR_ON)
	{
		ant ^= REVERSE_TRIANGULAR_ANT;
	}

	if (ant == STATE_TRIANGULAR_CLOCKWISE)
	{
		switch (state & MASK_TRIANGULAR_FACING)
		{
		case STATE_TRIANGULAR_E:
			++x;
			break;

		case STATE_TRIANGULAR_W:
			--y;
			break;

		case STATE_TRIANGULAR_NS:
		default:
			--x;
			break;
		}
	}
	else if (ant == STATE_TRIANGULAR_COUNTER)
	{
		switch (state & MASK_TRIANGULAR_FACING)
		{
		case STATE_TRIANGULAR_E:
			--y;
			break;

		case STATE_TRIANGULAR_W:
			--x;
			break;

		case STATE_TRIANGULAR_NS:
		default:
			++x;
			break;
		}
	}

	return x == 0 && y == 0;
}

//checks if ant at (x, y) with state state will be at (0, 0) on next time step
//used on down triangles on triangular grids
bool isAntComingToThisDownTriangle(int x, int y, int state)
{
	int ant = state & MASK_TRIANGULAR_ANT;

	//ants move in the opposite direction when the cell is on
	if ((state & MASK_TRIANGULAR_CELL) == STATE_TRIANGULAR_ON)
	{
		ant ^= REVERSE_TRIANGULAR_ANT;
	}

	if (ant == STATE_TRIANGULAR_CLOCKWISE)
	{
		switch (state & MASK_TRIANGULAR_FACING)
		{
		case STATE_TRIANGULAR_E:
			++y;
			break;

		case STATE_TRIANGULAR_W:
			--x;
			break;

		case STATE_TRIANGULAR_NS:
		default:
			++x;
			break;
		}
	}
	else if (ant == STATE_TRIANGULAR_COUNTER)
	{
		switch (state & MASK_TRIANGULAR_FACING)
		{
		case STATE_TRIANGULAR_E:
			++x;
			break;

		case STATE_TRIANGULAR_W:
			++y;
			break;

		case STATE_TRIANGULAR_NS:
		default:
			--x;
			break;
		}
	}

	return x == 0 && y == 0;
}

//does one time step for a cellular automaton based on Langton's Ant
//used on triangular grids
//ant rotates, recolors cell, and moves forward
//ants die if they stop on the same space
//state is 5 bits: FFAAC
//C0 = cell is off, C1 = cell is on
//AA00 = no ant, A01 = no ant, AA10 = clockwise ant, AA11 = counter-clockwise ant
//FF00 = facing E, FF01 = facing N or S, FF10 = facing W, FF11 = facing N or S
void updateLangtonsAntAutomaton(TriangularGrid* grid, bool shouldLoopHorizontally, bool shouldLoopVertically)
{
	grid->toggleGrid();

	for (int x = 0; x < grid->WIDTH; ++x)
	{
		for (int y = 0; y < grid->HEIGHT; ++y)
		{
			int neighborhood[13] = {0};
			int state = grid->getCellState(x, y);
			int numAnts = 0;
			int ant = state & MASK_TRIANGULAR_ANT;
			int facing = 0;
			int cell = state & MASK_TRIANGULAR_CELL;

			grid->getNeighborhood(neighborhood, x, y, false, shouldLoopHorizontally, shouldLoopVertically);

			if (ant * IS_TRIANGULAR_ANT)
			{
				cell ^= MASK_TRIANGULAR_CELL;
			}

			if ((x + y) % 2 == 0)
			{
				if ((neighborhood[NEIGHBOR_TRIANGULAR_NE] & IS_TRIANGULAR_ANT) && isAntComingToThisUpTriangle(1, 0, neighborhood[NEIGHBOR_TRIANGULAR_NE]))
				{
					++numAnts;
					ant = neighborhood[NEIGHBOR_TRIANGULAR_NE] & MASK_TRIANGULAR_ANT;
					facing = STATE_TRIANGULAR_W;
				}

				if ((neighborhood[NEIGHBOR_TRIANGULAR_NW] & IS_TRIANGULAR_ANT) && isAntComingToThisUpTriangle(-1, 0, neighborhood[NEIGHBOR_TRIANGULAR_NW]))
				{
					++numAnts;
					ant = neighborhood[NEIGHBOR_TRIANGULAR_NW] & MASK_TRIANGULAR_ANT;
					facing = STATE_TRIANGULAR_E;
				}

				if ((neighborhood[NEIGHBOR_TRIANGULAR_S] & IS_TRIANGULAR_ANT) && isAntComingToThisUpTriangle(0, 1, neighborhood[NEIGHBOR_TRIANGULAR_S]))
				{
					++numAnts;
					ant = neighborhood[NEIGHBOR_TRIANGULAR_S] & MASK_TRIANGULAR_ANT;
					facing = STATE_TRIANGULAR_NS;
				}
			}
			else
			{
				if ((neighborhood[NEIGHBOR_TRIANGULAR_N] & IS_TRIANGULAR_ANT) && isAntComingToThisDownTriangle(0, -1, neighborhood[NEIGHBOR_TRIANGULAR_N]))
				{
					++numAnts;
					ant = neighborhood[NEIGHBOR_TRIANGULAR_N] & MASK_TRIANGULAR_ANT;
					facing = STATE_TRIANGULAR_NS;
				}

				if ((neighborhood[NEIGHBOR_TRIANGULAR_SW] & IS_TRIANGULAR_ANT) && isAntComingToThisDownTriangle(-1, 0, neighborhood[NEIGHBOR_TRIANGULAR_SW]))
				{
					++numAnts;
					ant = neighborhood[NEIGHBOR_TRIANGULAR_SW] & MASK_TRIANGULAR_ANT;
					facing = STATE_TRIANGULAR_E;
				}

				if ((neighborhood[NEIGHBOR_TRIANGULAR_SE] & IS_TRIANGULAR_ANT) && isAntComingToThisDownTriangle(1, 0, neighborhood[NEIGHBOR_TRIANGULAR_SE]))
				{
					++numAnts;
					ant = neighborhood[NEIGHBOR_TRIANGULAR_SE] & MASK_TRIANGULAR_ANT;
					facing = STATE_TRIANGULAR_W;
				}
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
	int numThreads = 4;

	bool shouldLoopHorizontally = true;
	bool shouldLoopVertically = true;

	///*
	sf::Color colors[2] = {sf::Color(63, 63, 63), sf::Color(255, 255, 255)};
	bool isMooreNeighborhood = true;
	SquareGrid grid(&window, 64, 64, colors);
	int rulesIfOff[13] = {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int rulesIfOn[13] = {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	OuterTotalisticCA cellularAutomaton(&grid, rulesIfOff, rulesIfOn, isMooreNeighborhood, shouldLoopHorizontally, shouldLoopVertically);
	ThreadController automatonUpdater(numThreads, &cellularAutomaton);

	grid.setCellState(1, 1, 0);
	grid.setCellState(1, 2, 1);
	grid.setCellState(1, 0, 2);
	grid.setCellState(1, 1, 2);
	grid.setCellState(1, 2, 2);
	//*/

	/*
	sf::Color colors[2] = {sf::Color(63, 63, 63), sf::Color(255, 255, 255)};
	bool isMooreNeighborhood = true;
	HexagonalGrid grid(&window, 64, 64, colors);
	int rulesIfOff[13] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int rulesIfOn[13] = {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
	*/

	/*
	sf::Color colors[2] = {sf::Color(63, 63, 63), sf::Color(255, 255, 255)};
	bool isMooreNeighborhood = true;
	TriangularGrid grid(&window, 64, 64, colors);
	int rulesIfOff[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int rulesIfOn[13] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	*/

	/*
	sf::Color colors[32];

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

	SquareGrid grid(&window, 64, 64, colors);

	grid.setCellState(STATE_SQUARE_CLOCKWISE | STATE_SQUARE_N, 21, 21);
	grid.setCellState(STATE_SQUARE_COUNTER | STATE_SQUARE_N, 43, 43);
	*/

	/*
	sf::Color colors[128];

	for (int state = 0; state < 128; ++state)
	{
		if ((state & MASK_HEXAGONAL_ANT) == STATE_HEXAGONAL_SHARPCLOCKWISE)
		{
			colors[state] = sf::Color(255, 0, 0);
		}
		else if ((state & MASK_HEXAGONAL_ANT) == STATE_HEXAGONAL_SLIGHTCLOCKWISE)
		{
			colors[state] = sf::Color(255, 255, 0);
		}
		else if ((state & MASK_HEXAGONAL_ANT) == STATE_HEXAGONAL_SLIGHTCOUNTER)
		{
			colors[state] = sf::Color(0, 255, 0);
		}
		else if ((state & MASK_HEXAGONAL_ANT) == STATE_HEXAGONAL_SHARPCOUNTER)
		{
			colors[state] = sf::Color(0, 0, 255);
		}
		else if ((state & MASK_HEXAGONAL_CELL) == STATE_HEXAGONAL_ON)
		{
			colors[state] = sf::Color(255, 255, 255);
		}
		else
		{
			colors[state] = sf::Color(63, 63, 63);
		}
	}

	HexagonalGrid grid(&window, 64, 64, colors);

	grid.setCellState(STATE_HEXAGONAL_SHARPCLOCKWISE | STATE_HEXAGONAL_E, 21, 21);
	grid.setCellState(STATE_HEXAGONAL_SHARPCOUNTER | STATE_HEXAGONAL_E, 43, 21);
	grid.setCellState(STATE_HEXAGONAL_SLIGHTCLOCKWISE | STATE_HEXAGONAL_E, 21, 43);
	grid.setCellState(STATE_HEXAGONAL_SLIGHTCOUNTER | STATE_HEXAGONAL_E, 43, 43);
	*/

	/*
	sf::Color colors[32];

	for (int state = 0; state < 32; ++state)
	{
		if ((state & MASK_TRIANGULAR_ANT) == STATE_TRIANGULAR_CLOCKWISE)
		{
			colors[state] = sf::Color(255, 0, 0);
		}
		else if ((state & MASK_TRIANGULAR_ANT) == STATE_TRIANGULAR_COUNTER)
		{
			colors[state] = sf::Color(0, 0, 255);
		}
		else if ((state & MASK_TRIANGULAR_CELL) == STATE_TRIANGULAR_ON)
		{
			colors[state] = sf::Color(255, 255, 255);
		}
		else
		{
			colors[state] = sf::Color(63, 63, 63);
		}
	}

	TriangularGrid grid(&window, 64, 64, colors);

	grid.setCellState(STATE_TRIANGULAR_CLOCKWISE | STATE_TRIANGULAR_NS, 21, 21);
	grid.setCellState(STATE_TRIANGULAR_COUNTER | STATE_TRIANGULAR_NS, 43, 43);
	*/

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

		///*
		if (isUnpaused)
		{
			for (int step = 0; step < numStepsPerFrame; ++step)
			{
				automatonUpdater.activate();
			}
		}
		else if (shouldStep)
		{
			automatonUpdater.activate();
			shouldStep = false;
		}
		//*/

		/*
		if (isUnpaused)
		{
			for (int step = 0; step < numStepsPerFrame; ++step)
			{
				updateLangtonsAntAutomaton(&grid, shouldLoopHorizontally, shouldLoopVertically);
			}
		}
		else if (shouldStep)
		{
			updateLangtonsAntAutomaton(&grid, shouldLoopHorizontally, shouldLoopVertically);
			shouldStep = false;
		}
		*/

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			sf::Vector2i gridPosition = grid.getGridPositionAtMouse();

			if (gridPosition.x > -1 && gridPosition.x < grid.WIDTH && gridPosition.y > -1 && gridPosition.y < grid.HEIGHT)
			{
				grid.setCellState(1, gridPosition.x, gridPosition.y);
			}
		}

		window.clear();
		grid.draw();
		window.display();
	}

	return 0;
}