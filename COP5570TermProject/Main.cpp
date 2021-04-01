#include "LangtonsAntHexagonal.h"
#include "LangtonsAntSquare.h"
#include "LangtonsAntTriangular.h"
#include "ThreadController.h"
#include "OuterTotalisticCA.h"

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

	/*
	sf::Color colors[2] = {sf::Color(63, 63, 63), sf::Color(255, 255, 255)};
	bool isMooreNeighborhood = true;
	SquareGrid grid(&window, 64, 64, colors);
	int rulesIfOff[13] = {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int rulesIfOn[13] = {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	OuterTotalisticCA cellularAutomaton(&grid, rulesIfOff, rulesIfOn, isMooreNeighborhood, shouldLoopHorizontally, shouldLoopVertically);
	ThreadController automatonUpdater(numThreads, &cellularAutomaton);
	*/

	/*
	sf::Color colors[2] = {sf::Color(63, 63, 63), sf::Color(255, 255, 255)};
	bool isMooreNeighborhood = true;
	HexagonalGrid grid(&window, 64, 64, colors);
	int rulesIfOff[13] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int rulesIfOn[13] = {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0};
	OuterTotalisticCA cellularAutomaton(&grid, rulesIfOff, rulesIfOn, isMooreNeighborhood, shouldLoopHorizontally, shouldLoopVertically);
	ThreadController automatonUpdater(numThreads, &cellularAutomaton);
	*/

	/*
	sf::Color colors[2] = {sf::Color(63, 63, 63), sf::Color(255, 255, 255)};
	bool isMooreNeighborhood = true;
	TriangularGrid grid(&window, 64, 64, colors);
	int rulesIfOff[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int rulesIfOn[13] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	OuterTotalisticCA cellularAutomaton(&grid, rulesIfOff, rulesIfOn, isMooreNeighborhood, shouldLoopHorizontally, shouldLoopVertically);
	ThreadController automatonUpdater(numThreads, &cellularAutomaton);
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
	LangtonsAntSquare cellularAutomaton(&grid, shouldLoopHorizontally, shouldLoopVertically);
	ThreadController automatonUpdater(numThreads, &cellularAutomaton);

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
	LangtonsAntHexagonal cellularAutomaton(&grid, shouldLoopHorizontally, shouldLoopVertically);
	ThreadController automatonUpdater(numThreads, &cellularAutomaton);

	grid.setCellState(STATE_HEXAGONAL_SHARPCLOCKWISE | STATE_HEXAGONAL_E, 21, 21);
	grid.setCellState(STATE_HEXAGONAL_SHARPCOUNTER | STATE_HEXAGONAL_E, 43, 21);
	grid.setCellState(STATE_HEXAGONAL_SLIGHTCLOCKWISE | STATE_HEXAGONAL_E, 21, 43);
	grid.setCellState(STATE_HEXAGONAL_SLIGHTCOUNTER | STATE_HEXAGONAL_E, 43, 43);
	*/

	///*
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
	LangtonsAntTriangular cellularAutomaton(&grid, shouldLoopHorizontally, shouldLoopVertically);
	ThreadController automatonUpdater(numThreads, &cellularAutomaton);

	grid.setCellState(STATE_TRIANGULAR_CLOCKWISE | STATE_TRIANGULAR_NS, 21, 21);
	grid.setCellState(STATE_TRIANGULAR_COUNTER | STATE_TRIANGULAR_NS, 43, 43);
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