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

//initializes outer-totalistic buttons
void initializeOtMenuButtons(sf::RenderWindow &window, TextBox &pencilButton, TextBox &neighborhoodButton, TextBox &rulesIfOffButton, TextBox &rulesIfOnButton, std::vector<TextBox>& rulesIfOffButtons, std::vector<TextBox>& rulesIfOnButtons)
{
	pencilButton.setSize(window.getSize().x * 0.125f, window.getSize().y * 0.125f, 20);
	neighborhoodButton.setSize(window.getSize().x * 0.125f, window.getSize().y * 0.125f, 20);
	rulesIfOffButton.setSize(window.getSize().x * 0.046875f, window.getSize().y * 0.125f, 20);
	rulesIfOnButton.setSize(window.getSize().x * 0.046875f, window.getSize().y * 0.125f, 20);

	for (int b = 0; b < 13; ++b)
	{
		rulesIfOffButtons[b].setSize(window.getSize().x * 0.015625f, window.getSize().y * 0.125f, 20);
		rulesIfOnButtons[b].setSize(window.getSize().x * 0.015625f, window.getSize().y * 0.125f, 20);
	}

	pencilButton.setPosition(window.getSize().x * 0.75f, window.getSize().y * 0.5f);
	neighborhoodButton.setPosition(window.getSize().x * 0.875f, window.getSize().y * 0.5f);
	rulesIfOffButton.setPosition(window.getSize().x * 0.75f, window.getSize().y * 0.625f);
	rulesIfOnButton.setPosition(window.getSize().x * 0.75f, window.getSize().y * 0.75f);

	for (int b = 0; b < 13; ++b)
	{
		rulesIfOffButtons[b].setPosition(window.getSize().x * (51 + b) * 0.015625f, window.getSize().y * 0.625f);
		rulesIfOnButtons[b].setPosition(window.getSize().x * (51 + b) * 0.015625f, window.getSize().y * 0.75f);
	}
}

//initializes Langton's Ant buttons
void initializeLaMenuButtons(sf::RenderWindow &window, TextBox &cellButton, TextBox &antButton, TextBox &facingButton)
{
	cellButton.setSize(window.getSize().x * 0.25f, window.getSize().y * 0.125f, 20);
	antButton.setSize(window.getSize().x * 0.25f, window.getSize().y * 0.125f, 20);
	facingButton.setSize(window.getSize().x * 0.25f, window.getSize().y * 0.125f, 20);
	
	cellButton.setPosition(window.getSize().x * 0.75f, window.getSize().y * 0.5f);
	antButton.setPosition(window.getSize().x * 0.75f, window.getSize().y * 0.625f);
	facingButton.setPosition(window.getSize().x * 0.75f, window.getSize().y * 0.75f);
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

//cycles through ant types
void advanceAnt(GRID gridType, int &antState, TextBox &antButton)
{
	switch (gridType)
	{
	case SQUARE:
		switch (antState)
		{
		case STATE_SQUARE_CLOCKWISE:
			antState = 0;
			antButton.setString("No Ant");
			break;

		case STATE_SQUARE_COUNTER:
			antState = STATE_SQUARE_CLOCKWISE;
			antButton.setString("Clockwise Ant");
			break;

		default:
			antState = STATE_SQUARE_COUNTER;
			antButton.setString("Counterclockwise Ant");
			break;
		}

		break;

	case HEXAGONAL:
		switch (antState)
		{
		case STATE_HEXAGONAL_SHARPCOUNTER:
			antState = STATE_HEXAGONAL_SLIGHTCOUNTER;
			antButton.setString("Slight Left Ant");
			break;

		case STATE_HEXAGONAL_SLIGHTCOUNTER:
			antState = STATE_HEXAGONAL_SLIGHTCLOCKWISE;
			antButton.setString("Slight Right Ant");
			break;

		case STATE_HEXAGONAL_SLIGHTCLOCKWISE:
			antState = STATE_HEXAGONAL_SHARPCLOCKWISE;
			antButton.setString("Sharp Right Ant");
			break;

		case STATE_HEXAGONAL_SHARPCLOCKWISE:
			antState = 0;
			antButton.setString("No Ant");
			break;

		default:
			antState = STATE_HEXAGONAL_SHARPCOUNTER;
			antButton.setString("Sharp Left Ant");
			break;
		}

		break;

	case TRIANGULAR:
		switch (antState)
		{
		case STATE_TRIANGULAR_CLOCKWISE:
			antState = 0;
			antButton.setString("No Ant");
			break;

		case STATE_TRIANGULAR_COUNTER:
			antState = STATE_TRIANGULAR_CLOCKWISE;
			antButton.setString("Clockwise Ant");
			break;

		default:
			antState = STATE_TRIANGULAR_COUNTER;
			antButton.setString("Counterclockwise Ant");
			break;
		}

		break;

	default:
		break;
	}
}

//cycles through facing types
void advanceFacing(GRID gridType, int &facingState, TextBox &facingButton)
{
	switch (gridType)
	{
	case SQUARE:
		switch (facingState)
		{
		case STATE_SQUARE_E:
			facingState = STATE_SQUARE_S;
			facingButton.setString("Facing South");
			break;

		case STATE_SQUARE_S:
			facingState = STATE_SQUARE_W;
			facingButton.setString("Facing West");
			break;

		case STATE_SQUARE_W:
			facingState = STATE_SQUARE_N;
			facingButton.setString("Facing North");
			break;

		default:
			facingState = STATE_SQUARE_E;
			facingButton.setString("Facing East");
			break;
		}

		break;

	case HEXAGONAL:
		switch (facingState)
		{
		case STATE_HEXAGONAL_E:
			facingState = STATE_HEXAGONAL_SE;
			facingButton.setString("Facing Southeast");
			break;

		case STATE_HEXAGONAL_SE:
			facingState = STATE_HEXAGONAL_SW;
			facingButton.setString("Facing Southwest");
			break;

		case STATE_HEXAGONAL_SW:
			facingState = STATE_HEXAGONAL_W;
			facingButton.setString("Facing West");
			break;

		case STATE_HEXAGONAL_W:
			facingState = STATE_HEXAGONAL_NW;
			facingButton.setString("Facing Northwest");
			break;

		case STATE_HEXAGONAL_NW:
			facingState = STATE_HEXAGONAL_NE;
			facingButton.setString("Facing Northeast");
			break;

		default:
			facingState = STATE_HEXAGONAL_E;
			facingButton.setString("Facing East");
			break;
		}

		break;

	case TRIANGULAR:
		switch (facingState)
		{
		case STATE_TRIANGULAR_NS:
			facingState = STATE_TRIANGULAR_E;
			facingButton.setString("Facing East");
			break;

		case STATE_TRIANGULAR_E:
			facingState = STATE_TRIANGULAR_W;
			facingButton.setString("Facing West");
			break;

		default:
			facingState = STATE_TRIANGULAR_NS;
			facingButton.setString("Facing Vertically");
			break;
		}

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
	int cellState = 1;
	int antState = 0;
	int facingState = 0;

	sf::Color colors[128];
	bool shouldLoopHorizontally = true;
	bool shouldLoopVertically = true;
	Grid* grid = nullptr;
	CellularAutomaton* cellularAutomaton = nullptr;
	ThreadController* automatonUpdater = nullptr;

	bool isMooreNeighborhood = true;
	int rulesIfOff[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int rulesIfOn[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	arialFont.loadFromFile("arial.ttf");

	TextBox* selectedTextBox = nullptr;
	std::vector<TextBox> rulesIfOffButtons;
	std::vector<TextBox> rulesIfOnButtons;

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

	TextBox cellButton("Cell On", &window, arialFont, sf::Color(63, 63, 63), sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Color(0, 0, 0), true);
	TextBox antButton("No Ant", &window, arialFont, sf::Color(63, 63, 63), sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Color(0, 0, 0), true);
	TextBox facingButton("Facing East", &window, arialFont, sf::Color(63, 63, 63), sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Color(0, 0, 0), true);

	TextBox pencilButton("Draw", &window, arialFont, sf::Color(63, 63, 63), sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Color(0, 0, 0), true);
	TextBox neighborhoodButton("Vertex Neighbors", &window, arialFont, sf::Color(63, 63, 63), sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Color(0, 0, 0), true);
	TextBox rulesIfOffButton("Birth", &window, arialFont, sf::Color(0, 0, 0), sf::Color(0, 0, 0), sf::Color(255, 255, 255), sf::Color(0, 0, 0), true);
	TextBox rulesIfOnButton("Survival", &window, arialFont, sf::Color(0, 0, 0), sf::Color(0, 0, 0), sf::Color(255, 255, 255), sf::Color(0, 0, 0), true);

	for (int r = 0; r < 13; ++r)
	{
		rulesIfOffButtons.push_back(TextBox("", &window, arialFont, sf::Color(63, 63, 63), sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Color(0, 0, 0), true));
		rulesIfOnButtons.push_back(TextBox("", &window, arialFont, sf::Color(63, 63, 63), sf::Color(255, 255, 255), sf::Color(255, 255, 255), sf::Color(0, 0, 0), true));
	}

	initializeMainMenuButtons(window, caButton, gridButton, widthButton, heightButton, widthText, heightText, doneButton);
	initializeSideMenuButtons(window, threadButton, threadText, stepsPerFrameButton, stepsPerFrameText, pauseButton, stepButton, loopsHorizontallyButton, loopsVerticallyButton, menuButton);
	initializeOtMenuButtons(window, pencilButton, neighborhoodButton, rulesIfOffButton, rulesIfOnButton, rulesIfOffButtons, rulesIfOnButtons);
	initializeLaMenuButtons(window, cellButton, antButton, facingButton);

	while (window.isOpen())
	{
		sf::Event event;

		//process key presses and window closing
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

						for (int b = 0; b < 13; ++b)
						{
							rulesIfOffButtons[b].setString((rulesIfOff[b]) ? (std::to_string(b)) : (""));
							rulesIfOnButtons[b].setString((rulesIfOn[b]) ? (std::to_string(b)) : (""));
						}

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

		//handle button clicking
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

					for (int b = 0; b < 13; ++b)
					{
						rulesIfOffButtons[b].setString((rulesIfOff[b]) ? (std::to_string(b)) : (""));
						rulesIfOnButtons[b].setString((rulesIfOn[b]) ? (std::to_string(b)) : (""));
					}
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

					cellState = 1;
					antState = 0;
					facingState = 0;

					cellButton.setString("Cell On");
					antButton.setString("No Ant");
					facingButton.setString("Facing East");

					delete automatonUpdater;
					delete cellularAutomaton;
					delete grid;

					automatonUpdater = nullptr;
				}
				else
				{
					if (caType == OUTER_TOTALISTIC)
					{
						if (pencilButton.doesContainPoint(mousePosition))
						{
							pencilState = 1 - pencilState;
							pencilButton.setString((pencilState) ? ("Draw") : ("Erase"));
						}
						else if (neighborhoodButton.doesContainPoint(mousePosition))
						{
							isMooreNeighborhood = !isMooreNeighborhood;
							neighborhoodButton.setString((isMooreNeighborhood) ? ("Vertex Neighbors") : ("Edge Neighbors"));
							((OuterTotalisticCA*)cellularAutomaton)->setIsMooreNeighborhood(isMooreNeighborhood);
						}
						else
						{
							for (int b = 0; b < 13; ++b)
							{
								if (rulesIfOffButtons[b].doesContainPoint(mousePosition))
								{
									rulesIfOff[b] = 1 - rulesIfOff[b];
									rulesIfOffButtons[b].setString((rulesIfOff[b]) ? (std::to_string(b)) : (""));
									((OuterTotalisticCA*)cellularAutomaton)->setBirthRule(b, rulesIfOff[b]);
									break;
								}
								else if (rulesIfOnButtons[b].doesContainPoint(mousePosition))
								{
									rulesIfOn[b] = 1 - rulesIfOn[b];
									rulesIfOnButtons[b].setString((rulesIfOn[b]) ? (std::to_string(b)) : (""));
									((OuterTotalisticCA*)cellularAutomaton)->setSurvivalRule(b, rulesIfOn[b]);
									break;
								}
							}
						}
					}
					else
					{
						if (cellButton.doesContainPoint(mousePosition))
						{
							cellState = 1 - cellState;
							cellButton.setString((cellState) ? ("Cell On") : ("Cell Off"));
						}
						else if (antButton.doesContainPoint(mousePosition))
						{
							advanceAnt(gridType, antState, antButton);
						}
						else if (facingButton.doesContainPoint(mousePosition))
						{
							advanceFacing(gridType, facingState, facingButton);
						}
					}
				}
			}
		}

		//update grid
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
					grid->setCellState((caType == OUTER_TOTALISTIC) ? (pencilState) : (cellState | antState | facingState), gridPosition.x, gridPosition.y);
				}
			}
		}

		window.clear();
		
		//draw everything
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
				pencilButton.draw();
				neighborhoodButton.draw();
				rulesIfOffButton.draw();
				rulesIfOnButton.draw();

				for (TextBox button : rulesIfOffButtons)
				{
					button.draw();
				}

				for (TextBox button : rulesIfOnButtons)
				{
					button.draw();
				}
			}
			else
			{
				cellButton.draw();
				antButton.draw();
				facingButton.draw();
			}

			menuButton.draw();

			grid->draw();
		}

		window.display();
	}

	if (automatonUpdater)
	{
		delete automatonUpdater;
		delete cellularAutomaton;
		delete grid;
	}

	return 0;
}