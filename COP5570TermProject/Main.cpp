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
void initializeMainMenuButtons(sf::RenderWindow &window, TextBox &caButton, TextBox &gridButton, TextBox &widthButton, TextBox &heightButton, TextBox &widthText, TextBox &heightText, TextBox &doneButton)
{
	caButton.setSize(window.getSize().x * 0.5f, window.getSize().y * 0.1f, 30);
	gridButton.setSize(window.getSize().x * 0.5f, window.getSize().y * 0.1f, 30);
	widthButton.setSize(window.getSize().x * 0.25f, window.getSize().y * 0.1f, 30);
	heightButton.setSize(window.getSize().x * 0.25f, window.getSize().y * 0.1f, 30);
	widthText.setSize(window.getSize().x * 0.25f, window.getSize().y * 0.1f, 30);
	heightText.setSize(window.getSize().x * 0.25f, window.getSize().y * 0.1f, 30);
	doneButton.setSize(window.getSize().x * 0.5f, window.getSize().y * 0.1f, 30);

	caButton.setPosition(window.getSize().x * 0.25f, window.getSize().y * 0.25f);
	gridButton.setPosition(window.getSize().x * 0.25f, window.getSize().y * 0.35f);
	widthButton.setPosition(window.getSize().x * 0.25f, window.getSize().y * 0.45f);
	heightButton.setPosition(window.getSize().x * 0.25f, window.getSize().y * 0.55f);
	widthText.setPosition(window.getSize().x * 0.5f, window.getSize().y * 0.45f);
	heightText.setPosition(window.getSize().x * 0.5f, window.getSize().y * 0.55f);
	doneButton.setPosition(window.getSize().x * 0.25f, window.getSize().y * 0.65f);
}

//initializes side menu buttons
void initializeSideMenuButtons(sf::RenderWindow &window, TextBox &threadButton, TextBox &threadText, TextBox &stepsPerFrameButton, TextBox &stepsPerFrameText, TextBox &pauseButton, TextBox &stepButton, TextBox &loopsHorizontallyButton, TextBox &loopsVerticallyButton, TextBox &menuButton)
{
	threadButton.setSize(window.getSize().x * 0.125f, window.getSize().y * 0.125f, 20);
	threadText.setSize(window.getSize().x * 0.125f, window.getSize().y * 0.125f, 20);
	stepsPerFrameButton.setSize(window.getSize().x * 0.125f, window.getSize().y * 0.125f, 20);
	stepsPerFrameText.setSize(window.getSize().x * 0.125f, window.getSize().y * 0.125f, 20);
	pauseButton.setSize(window.getSize().x * 0.125f, window.getSize().y * 0.125f, 20);
	stepButton.setSize(window.getSize().x * 0.125f, window.getSize().y * 0.125f, 20);
	loopsHorizontallyButton.setSize(window.getSize().x * 0.125f, window.getSize().y * 0.125f, 20);
	loopsVerticallyButton.setSize(window.getSize().x * 0.125f, window.getSize().y * 0.125f, 20);
	menuButton.setSize(window.getSize().x * 0.25f, window.getSize().y * 0.125f, 20);

	threadButton.setPosition(window.getSize().x * 0.75f, 0.0f);
	threadText.setPosition(window.getSize().x * 0.875f, 0.0f);
	stepsPerFrameButton.setPosition(window.getSize().x * 0.75f, window.getSize().y * 0.125f);
	stepsPerFrameText.setPosition(window.getSize().x * 0.875f, window.getSize().y * 0.125f);
	pauseButton.setPosition(window.getSize().x * 0.75f, window.getSize().y * 0.25f);
	stepButton.setPosition(window.getSize().x * 0.875f, window.getSize().y * 0.25f);
	loopsHorizontallyButton.setPosition(window.getSize().x * 0.75f, window.getSize().y * 0.375f);
	loopsVerticallyButton.setPosition(window.getSize().x * 0.875f, window.getSize().y * 0.375f);
	menuButton.setPosition(window.getSize().x * 0.75f, window.getSize().y * 0.875f);
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

	bool isUnpaused = false;
	bool shouldStep = false;
	int numStepsPerFrame = 1;
	int numThreads = 4;
	int pencilState = 1;

	arialFont.loadFromFile("arial.ttf");

	TextBox* selectedTextBox = nullptr;

	TextBox caButton("Outer Totalistic", &window, arialFont, sf::Color(63, 63, 63), sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Color(0, 0, 0), true);
	TextBox gridButton("Square", &window, arialFont, sf::Color(63, 63, 63), sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Color(0, 0, 0), true);
	TextBox widthButton("Width", &window, arialFont, sf::Color(0, 0, 0), sf::Color(0, 0, 0), sf::Color(255, 255, 255), sf::Color(0, 0, 0), true);
	TextBox heightButton("Height", &window, arialFont, sf::Color(0, 0, 0), sf::Color(0, 0, 0), sf::Color(255, 255, 255), sf::Color(0, 0, 0), true);
	TextBox widthText("64", &window, arialFont, sf::Color(63, 63, 63), sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Color(0, 0, 0), false);
	TextBox heightText("64", &window, arialFont, sf::Color(63, 63, 63), sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Color(0, 0, 0), false);
	TextBox doneButton("Done", &window, arialFont, sf::Color(63, 63, 63), sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Color(0, 0, 0), true);

	TextBox threadButton("Threads", &window, arialFont, sf::Color(0, 0, 0), sf::Color(0, 0, 0), sf::Color(255, 255, 255), sf::Color(0, 0, 0), true);
	TextBox threadText("4", &window, arialFont, sf::Color(63, 63, 63), sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Color(0, 0, 0), false);
	TextBox stepsPerFrameButton("Steps per Frame", &window, arialFont, sf::Color(0, 0, 0), sf::Color(0, 0, 0), sf::Color(255, 255, 255), sf::Color(0, 0, 0), true);
	TextBox stepsPerFrameText("1", &window, arialFont, sf::Color(63, 63, 63), sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Color(0, 0, 0), false);
	TextBox pauseButton("Paused", &window, arialFont, sf::Color(63, 63, 63), sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Color(0, 0, 0), true);
	TextBox stepButton("Step", &window, arialFont, sf::Color(63, 63, 63), sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Color(0, 0, 0), true);
	TextBox loopsHorizontallyButton("Loops Horizontally", &window, arialFont, sf::Color(63, 63, 63), sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Color(0, 0, 0), true);
	TextBox loopsVerticallyButton("Loops Vertically", &window, arialFont, sf::Color(63, 63, 63), sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Color(0, 0, 0), true);
	TextBox menuButton("Main Menu", &window, arialFont, sf::Color(63, 63, 63), sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Color(0, 0, 0), true);

	initializeMainMenuButtons(window, caButton, gridButton, widthButton, heightButton, widthText, heightText, doneButton);
	initializeSideMenuButtons(window, threadButton, threadText, stepsPerFrameButton, stepsPerFrameText, pauseButton, stepButton, loopsHorizontallyButton, loopsVerticallyButton, menuButton);

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
						pauseButton.setString((isUnpaused) ? ("Running") : ("Paused"));

						if (selectedTextBox)
						{
							selectedTextBox->setString(std::to_string(selectedTextBox->getValue(1)));

							if (selectedTextBox == &threadText)
							{
								numThreads = threadText.getValue(1);
								automatonUpdater->setNumThreads(numThreads);
							}
							else if (selectedTextBox == &stepsPerFrameText)
							{
								numStepsPerFrame = stepsPerFrameText.getValue(1);
							}
						}

						selectedTextBox = nullptr;
						break;

					case sf::Keyboard::Return:
						if (!isUnpaused)
						{
							shouldStep = true;
						}

						if (selectedTextBox)
						{
							selectedTextBox->setString(std::to_string(selectedTextBox->getValue(1)));

							if (selectedTextBox == &threadText)
							{
								numThreads = threadText.getValue(1);
								automatonUpdater->setNumThreads(numThreads);
							}
							else if (selectedTextBox == &stepsPerFrameText)
							{
								numStepsPerFrame = stepsPerFrameText.getValue(1);
							}
						}

						selectedTextBox = nullptr;
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

		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			isClicking = false;
		}
		else if (!isClicking)
		{
			sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

			isClicking = true;

			if (isShowingMainMenu)
			{
				if (selectedTextBox)
				{
					selectedTextBox->setString(std::to_string(selectedTextBox->getValue(3)));
				}

				selectedTextBox = nullptr;

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
				else if (doneButton.doesContainPoint(mousePosition))
				{
					isShowingMainMenu = false;
					initializeCellularAutomaton(window, numThreads, isMooreNeighborhood, shouldLoopHorizontally, shouldLoopVertically, caType, gridType, widthText.getValue(3), heightText.getValue(3), grid, cellularAutomaton, automatonUpdater, colors, rulesIfOff, rulesIfOn);
				}
			}
			else
			{
				if (selectedTextBox)
				{
					selectedTextBox->setString(std::to_string(selectedTextBox->getValue(1)));

					if (selectedTextBox == &threadText)
					{
						numThreads = threadText.getValue(1);
						automatonUpdater->setNumThreads(numThreads);
					}
					else if (selectedTextBox == &stepsPerFrameText)
					{
						numStepsPerFrame = stepsPerFrameText.getValue(1);
					}
				}

				selectedTextBox = nullptr;

				if (threadText.doesContainPoint(mousePosition))
				{
					selectedTextBox = &threadText;
				}
				else if (stepsPerFrameText.doesContainPoint(mousePosition))
				{
					selectedTextBox = &stepsPerFrameText;
				}
				else if (pauseButton.doesContainPoint(mousePosition))
				{
					isUnpaused = !isUnpaused;
					pauseButton.setString((isUnpaused) ? ("Running") : ("Paused"));
				}
				else if (stepButton.doesContainPoint(mousePosition))
				{
					if (!isUnpaused)
					{
						shouldStep = true;
					}
				}
				else if (loopsHorizontallyButton.doesContainPoint(mousePosition))
				{
					shouldLoopHorizontally = !shouldLoopHorizontally;
					loopsHorizontallyButton.setString((shouldLoopHorizontally) ? ("Loops Horizontally") : ("Clips Horizontally"));
					cellularAutomaton->setShouldLoopHorizontally(shouldLoopHorizontally);
				}
				else if (loopsVerticallyButton.doesContainPoint(mousePosition))
				{
					shouldLoopVertically = !shouldLoopVertically;
					loopsVerticallyButton.setString((shouldLoopVertically) ? ("Loops Vertically") : ("Clips Vertically"));
					cellularAutomaton->setShouldLoopVertically(shouldLoopVertically);
				}
				else if (menuButton.doesContainPoint(mousePosition))
				{
					isShowingMainMenu = true;
					pencilState = 1;

					delete automatonUpdater;
					delete cellularAutomaton;
					delete grid;
				}
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
					grid->setCellState(pencilState, gridPosition.x, gridPosition.y);
				}
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
			doneButton.draw();
		}
		else
		{
			threadButton.draw();
			threadText.draw();
			stepsPerFrameButton.draw();
			stepsPerFrameText.draw();
			pauseButton.draw();
			stepButton.draw();
			loopsHorizontallyButton.draw();
			loopsVerticallyButton.draw();

			if (caType == OUTER_TOTALISTIC)
			{

			}
			else
			{

			}

			menuButton.draw();

			grid->draw();
		}

		window.display();
	}

	delete automatonUpdater;
	delete cellularAutomaton;
	delete grid;

	return 0;
}