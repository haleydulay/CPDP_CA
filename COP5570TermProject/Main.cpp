#include "LangtonsAntHexagonal.h"
#include "LangtonsAntSquare.h"
#include "LangtonsAntTriangular.h"
#include "TextBox.h"
#include "ThreadController.h"
#include "OuterTotalisticCA.h"

enum GRID
{
	SQUARE,
	HEXAGONAL,
	TRIANGULAR
};

enum CELLULAR_AUTOMATON
{
	OUTER_TOTALISTIC,
	LANGTONS_ANT
};

//initializes main menu buttons
void initializeMainMenuButtons(sf::RenderWindow &window, TextBox &caButton, TextBox &gridButton, TextBox &widthButton, TextBox &heightButton, TextBox &widthText, TextBox &heightText)
{
	caButton.setSize(window.getSize().x * 0.5f, window.getSize().y * 0.125f, 30);
	gridButton.setSize(window.getSize().x * 0.5f, window.getSize().y * 0.125f, 30);
	widthButton.setSize(window.getSize().x * 0.25f, window.getSize().y * 0.125f, 30);
	heightButton.setSize(window.getSize().x * 0.25f, window.getSize().y * 0.125f, 30);
	widthText.setSize(window.getSize().x * 0.25f, window.getSize().y * 0.125f, 30);
	heightText.setSize(window.getSize().x * 0.25f, window.getSize().y * 0.125f, 30);

	caButton.setPosition(window.getSize().x * 0.25f, window.getSize().y * 0.25f);
	gridButton.setPosition(window.getSize().x * 0.25f, window.getSize().y * 0.375f);
	widthButton.setPosition(window.getSize().x * 0.25f, window.getSize().y * 0.5f);
	heightButton.setPosition(window.getSize().x * 0.25f, window.getSize().y * 0.625f);
	widthText.setPosition(window.getSize().x * 0.5f, window.getSize().y * 0.5f);
	heightText.setPosition(window.getSize().x * 0.5f, window.getSize().y * 0.625f);
}

//uses main menu data to initialize next screen
//outer-totalistic version
void initializeOuterTotalisticCa(sf::RenderWindow &window, int numThreads, bool isMooreNeighborhood, bool shouldLoopHorizontally, bool shouldLoopVertically, GRID gridType, int width, int height, Grid* &grid, CellularAutomaton* &cellularAutomaton, ThreadController* &automatonUpdater, sf::Color colors[128], int rulesIfOff[13], int rulesIfOn[13])
{
	colors[0] = sf::Color(63, 63, 63);
	colors[1] = sf::Color(255, 255, 255);

	for (int r = 0; r < 13; ++r)
	{
		rulesIfOff[r] = 0;
		rulesIfOn[r] = 0;
	}

	switch (gridType)
	{
	case SQUARE:
		grid = new SquareGrid(&window, width, height, colors);

		rulesIfOff[3] = 1;

		rulesIfOn[2] = 1;
		rulesIfOn[3] = 1;
		break;

	case HEXAGONAL:
		grid = new HexagonalGrid(&window, width, height, colors);

		rulesIfOff[2] = 1;

		rulesIfOn[3] = 1;
		rulesIfOn[4] = 1;
		break;

	case TRIANGULAR:
		grid = new TriangularGrid(&window, width, height, colors);

		rulesIfOff[4] = 1;

		rulesIfOn[1] = 1;
		rulesIfOn[2] = 1;
		break;

	default:
		break;
	}

	cellularAutomaton = new OuterTotalisticCA(grid, rulesIfOff, rulesIfOn, isMooreNeighborhood, shouldLoopHorizontally, shouldLoopVertically);
	automatonUpdater = new ThreadController(numThreads, cellularAutomaton);
}

//uses main menu data to initialize next screen
//Langton's Ant version
void initializeLangtonsAntCa(sf::RenderWindow &window, int numThreads, bool shouldLoopHorizontally, bool shouldLoopVertically, GRID gridType, int width, int height, Grid* &grid, CellularAutomaton* &cellularAutomaton, ThreadController* &automatonUpdater, sf::Color colors[128])
{
	switch (gridType)
	{
	case SQUARE:
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

		grid = new SquareGrid(&window, width, height, colors);
		cellularAutomaton = new LangtonsAntSquare((SquareGrid*)grid, shouldLoopHorizontally, shouldLoopVertically);
		grid->setCellState(STATE_SQUARE_CLOCKWISE | STATE_SQUARE_N, width / 2, height / 2);
		break;

	case HEXAGONAL:
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

		grid = new HexagonalGrid(&window, width, height, colors);
		cellularAutomaton = new LangtonsAntHexagonal((HexagonalGrid*)grid, shouldLoopHorizontally, shouldLoopVertically);
		grid->setCellState(STATE_HEXAGONAL_SLIGHTCLOCKWISE | STATE_HEXAGONAL_E, width / 2, height / 2);
		break;

	case TRIANGULAR:
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

		grid = new TriangularGrid(&window, width, height, colors);
		cellularAutomaton = new LangtonsAntTriangular((TriangularGrid*)grid, shouldLoopHorizontally, shouldLoopVertically);
		grid->setCellState(STATE_TRIANGULAR_CLOCKWISE | STATE_TRIANGULAR_NS, width / 2, height / 2);
		break;

	default:
		break;
	}

	automatonUpdater = new ThreadController(numThreads, cellularAutomaton);
}

//uses main menu data to initialize next screen
void initializeCellularAutomaton(sf::RenderWindow &window, int numThreads, bool isMooreNeighborhood, bool shouldLoopHorizontally, bool shouldLoopVertically, CELLULAR_AUTOMATON caType, GRID gridType, int width, int height, Grid* &grid, CellularAutomaton* &cellularAutomaton, ThreadController* &automatonUpdater, sf::Color colors[128], int rulesIfOff[13], int rulesIfOn[13])
{
	switch (caType)
	{
	case OUTER_TOTALISTIC:
		initializeOuterTotalisticCa(window, numThreads, isMooreNeighborhood, shouldLoopHorizontally, shouldLoopVertically, gridType, width, height, grid, cellularAutomaton, automatonUpdater, colors, rulesIfOff, rulesIfOn);
		break;

	case LANGTONS_ANT:
		initializeLangtonsAntCa(window, numThreads, shouldLoopHorizontally, shouldLoopVertically, gridType, width, height, grid, cellularAutomaton, automatonUpdater, colors);
		break;

	default:
		break;
	}
}

//main function
//sets program up and handles events and drawing
int main()
{
	sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width - 100, sf::VideoMode::getDesktopMode().height - 100), "COP5570 Term Project", sf::Style::Close | sf::Style::Titlebar);
	sf::Font arialFont = sf::Font();
	bool isShowingMainMenu = true;
	bool isClicking = false;
	GRID gridType = SQUARE;
	CELLULAR_AUTOMATON caType = OUTER_TOTALISTIC;

	arialFont.loadFromFile("arial.ttf");

	TextBox* selectedTextBox = nullptr;

	TextBox caButton("Outer Totalistic", &window, arialFont, sf::Color(63, 63, 63), sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Color(0, 0, 0), true);
	TextBox gridButton("Square", &window, arialFont, sf::Color(63, 63, 63), sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Color(0, 0, 0), true);
	TextBox widthButton("Width", &window, arialFont, sf::Color(0, 0, 0), sf::Color(0, 0, 0), sf::Color(255, 255, 255), sf::Color(0, 0, 0), true);
	TextBox heightButton("Height", &window, arialFont, sf::Color(0, 0, 0), sf::Color(0, 0, 0), sf::Color(255, 255, 255), sf::Color(0, 0, 0), true);
	TextBox widthText("64", &window, arialFont, sf::Color(63, 63, 63), sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Color(0, 0, 0), false);
	TextBox heightText("64", &window, arialFont, sf::Color(63, 63, 63), sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Color(0, 0, 0), false);

	initializeMainMenuButtons(window, caButton, gridButton, widthButton, heightButton, widthText, heightText);

	bool isUnpaused = false;
	bool shouldStep = false;
	int numStepsPerFrame = 1;
	int numThreads = 4;

	sf::Color colors[128];
	bool shouldLoopHorizontally = true;
	bool shouldLoopVertically = true;
	Grid* grid = nullptr;
	CellularAutomaton* cellularAutomaton = nullptr;
	ThreadController* automatonUpdater = nullptr;

	bool isMooreNeighborhood = true;
	int rulesIfOff[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int rulesIfOn[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

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
				if (isShowingMainMenu)
				{
					switch (event.key.code)
					{
					case sf::Keyboard::Return:
						isShowingMainMenu = false;
						
						if (cellularAutomaton)
						{
							delete grid;
							delete cellularAutomaton;
							delete automatonUpdater;
						}

						initializeCellularAutomaton(window, numThreads, isMooreNeighborhood, shouldLoopHorizontally, shouldLoopVertically, caType, gridType, widthText.getValue(3), heightText.getValue(3), grid, cellularAutomaton, automatonUpdater, colors, rulesIfOff, rulesIfOn);
						break;

					default:
						break;
					}
				}
				else
				{
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
				}

				break;

			case sf::Event::TextEntered:
				if (selectedTextBox)
				{
					selectedTextBox->addCharacter(event.text.unicode);
				}

			default:
				break;
			}
		}

		if (!isShowingMainMenu)
		{
			if (isUnpaused)
			{
				for (int step = 0; step < numStepsPerFrame; ++step)
				{
					automatonUpdater->activate();
				}
			}
			else if (shouldStep)
			{
				automatonUpdater->activate();
				shouldStep = false;
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				sf::Vector2i gridPosition = grid->getGridPositionAtMouse();

				if (gridPosition.x > -1 && gridPosition.x < grid->WIDTH && gridPosition.y > -1 && gridPosition.y < grid->HEIGHT)
				{
					grid->setCellState(1, gridPosition.x, gridPosition.y);
				}
			}
		}

		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			isClicking = false;
		}
		else if (!isClicking)
		{
			sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

			if (selectedTextBox)
			{
				selectedTextBox->setString(std::to_string(selectedTextBox->getValue(3)));
			}

			isClicking = true;
			selectedTextBox = nullptr;

			if (isShowingMainMenu)
			{
				if (caButton.doesContainPoint(mousePosition))
				{
					switch (caType)
					{
					case OUTER_TOTALISTIC:
						caButton.setString("Langton's Ant");
						caType = LANGTONS_ANT;
						break;

					case LANGTONS_ANT:
						caButton.setString("Outer Totalistic");
						caType = OUTER_TOTALISTIC;
						break;

					default:
						break;
					}
				}
				else if (gridButton.doesContainPoint(mousePosition))
				{
					switch (gridType)
					{
					case SQUARE:
						gridButton.setString("Hexagonal");
						gridType = HEXAGONAL;
						break;

					case HEXAGONAL:
						gridButton.setString("Triangular");
						gridType = TRIANGULAR;
						break;

					case TRIANGULAR:
						gridButton.setString("Square");
						gridType = SQUARE;
						break;

					default:
						break;
					}
				}
				else if (widthText.doesContainPoint(mousePosition))
				{
					selectedTextBox = &widthText;
				}
				else if (heightText.doesContainPoint(mousePosition))
				{
					selectedTextBox = &heightText;
				}
			}
			else
			{

			}
		}

		window.clear();
		
		if (isShowingMainMenu)
		{
			caButton.draw();
			gridButton.draw();
			widthButton.draw();
			heightButton.draw();
			widthText.draw();
			heightText.draw();
		}
		else
		{
			grid->draw();
		}

		window.display();
	}

	return 0;
}