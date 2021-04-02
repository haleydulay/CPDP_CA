#include "SquareGrid.h"

//square grid constructor
//initializes member variables
SquareGrid::SquareGrid(sf::RenderWindow* window, int width, int height, sf::Color* colors) : Grid(window, width, height, colors)
{
	float rectangleWidth = window->getSize().x / (float)width;
	float rectangleHeight = window->getSize().y / (float)height;

	squareLength = (rectangleWidth < rectangleHeight) ? (rectangleWidth) : (rectangleHeight);

	gridA = new int*[WIDTH];
	gridB = new int*[WIDTH];
	image = sf::VertexArray(sf::Quads, 4 * WIDTH * HEIGHT);

	for (int x = 0; x < WIDTH; ++x)
	{
		gridA[x] = new int[HEIGHT];
		gridB[x] = new int[HEIGHT];

		for (int y = 0; y < HEIGHT; ++y)
		{
			gridA[x][y] = 0;
			gridB[x][y] = 0;

			image[(HEIGHT * x + y) * 4] = sf::Vertex(sf::Vector2f(x * squareLength, y * squareLength), COLORS[0]);
			image[(HEIGHT * x + y) * 4 + 1] = sf::Vertex(sf::Vector2f(x * squareLength + squareLength, y * squareLength), COLORS[0]);
			image[(HEIGHT * x + y) * 4 + 2] = sf::Vertex(sf::Vector2f(x * squareLength + squareLength, y * squareLength + squareLength), COLORS[0]);
			image[(HEIGHT * x + y) * 4 + 3] = sf::Vertex(sf::Vector2f(x * squareLength, y * squareLength + squareLength), COLORS[0]);
		}
	}
}

//square grid destructor
//deletes grid
SquareGrid::~SquareGrid()
{
	for (int x = 0; x < WIDTH; ++x)
	{
		delete[] gridA[x];
		delete[] gridB[x];
	}

	delete[] gridA;
	delete[] gridB;
}

//returns grid[x][y]
int SquareGrid::getCellState(int x, int y)
{
	if (shouldReadGridA)
	{
		return gridA[x][y];
	}
	else
	{
		return gridB[x][y];
	}
}

//sets grid[x][y] to state and updates color of cell
void SquareGrid::setCellState(int state, int x, int y)
{
	if (shouldReadGridA)
	{
		gridB[x][y] = state;
	}
	else
	{
		gridA[x][y] = state;
	}
	
	image[(HEIGHT * x + y) * 4].color = COLORS[state];
	image[(HEIGHT * x + y) * 4 + 1].color = COLORS[state];
	image[(HEIGHT * x + y) * 4 + 2].color = COLORS[state];
	image[(HEIGHT * x + y) * 4 + 3].color = COLORS[state];
}

//puts the states of the cells in the neighborhood of the cell at (x, y) in the neighborhood array
//bools control whether the left edge of the grid should connect to the right and whether the top edge of the grid should connect to the bottom
//anything outside the grid is given a state of 0
void SquareGrid::getNeighborhood(int neighborhood[13], int x, int y, bool isMooreNeighborhood, bool shouldLoopHorizontally, bool shouldLoopVertically)
{
	if (isMooreNeighborhood)
	{
		getMooreNeighborhood(neighborhood, x, y, shouldLoopHorizontally, shouldLoopVertically);
	}
	else
	{
		getVonNeumannNeighborhood(neighborhood, x, y, shouldLoopHorizontally, shouldLoopVertically);
	}
}

//gets grid (x, y) at mouse position
//may be outside grid's actual range
sf::Vector2i SquareGrid::getGridPositionAtMouse()
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*WINDOW);
	int x = (int)floorf(mousePosition.x / squareLength);
	int y = (int)floorf(mousePosition.y / squareLength);

	return sf::Vector2i(x, y);
}

//draws cells
void SquareGrid::draw()
{
	WINDOW->draw(image);
}

//puts the states of the cells in the Moore neighborhood of the cell at (x, y) in the neighborhood array
//cells that share a vertex are neighbors
//bools control whether the left edge of the grid should connect to the right and whether the top edge of the grid should connect to the bottom
//anything outside the grid is given a state of 0
void SquareGrid::getMooreNeighborhood(int neighborhood[13], int x, int y, bool shouldLoopHorizontally, bool shouldLoopVertically)
{
	int** grid = (shouldReadGridA) ? (gridA) : (gridB);

	if (shouldLoopHorizontally)
	{
		if (shouldLoopVertically)
		{
			//torus
			neighborhood[0] = grid[(x + WIDTH - 1) % WIDTH][(y + HEIGHT - 1) % HEIGHT];	//NW
			neighborhood[1] = grid[x][(y + HEIGHT - 1) % HEIGHT];						//N
			neighborhood[2] = grid[(x + 1) % WIDTH][(y + HEIGHT - 1) % HEIGHT];			//NE
			neighborhood[3] = grid[(x + WIDTH - 1) % WIDTH][y];							//W
			neighborhood[4] = grid[x][y];												//C
			neighborhood[5] = grid[(x + 1) % WIDTH][y];									//E
			neighborhood[6] = grid[(x + WIDTH - 1) % WIDTH][(y + 1) % HEIGHT];			//SW
			neighborhood[7] = grid[x][(y + 1) % HEIGHT];								//S
			neighborhood[8] = grid[(x + 1) % WIDTH][(y + 1) % HEIGHT];					//SE
		}
		else
		{
			//vertical cylinder
			neighborhood[0] = (y > 0) ? (grid[(x + WIDTH - 1) % WIDTH][y - 1]) : (0);			//NW
			neighborhood[1] = (y > 0) ? (grid[x][y - 1]) : (0);									//N
			neighborhood[2] = (y > 0) ? (grid[(x + 1) % WIDTH][y - 1]) : (0);					//NE
			neighborhood[3] = grid[(x + WIDTH - 1) % WIDTH][y];									//W
			neighborhood[4] = grid[x][y];														//C
			neighborhood[5] = grid[(x + 1) % WIDTH][y];											//E
			neighborhood[6] = (y < HEIGHT - 1) ? (grid[(x + WIDTH - 1) % WIDTH][y + 1]) : (0);	//SW
			neighborhood[7] = (y < HEIGHT - 1) ? (grid[x][y + 1]) : (0);						//S
			neighborhood[8] = (y < HEIGHT - 1) ? (grid[(x + 1) % WIDTH][y + 1]) : (0);			//SE
		}
	}
	else
	{
		if (shouldLoopVertically)
		{
			//horizontal cylinder
			neighborhood[0] = (x > 0) ? (grid[x - 1][(y + HEIGHT - 1) % HEIGHT]) : (0);			//NW
			neighborhood[1] = grid[x][(y + HEIGHT - 1) % HEIGHT];								//N
			neighborhood[2] = (x < WIDTH - 1) ? (grid[x + 1][(y + HEIGHT - 1) % HEIGHT]) : (0);	//NE
			neighborhood[3] = (x > 0) ? (grid[x - 1][y]) : (0);									//W
			neighborhood[4] = grid[x][y];														//C
			neighborhood[5] = (x < WIDTH - 1) ? (grid[x + 1][y]) : (0);							//E
			neighborhood[6] = (x > 0) ? (grid[x - 1][(y + 1) % HEIGHT]) : (0);					//SW
			neighborhood[7] = grid[x][(y + 1) % HEIGHT];										//S
			neighborhood[8] = (x < WIDTH - 1) ? (grid[x + 1][(y + 1) % HEIGHT]) : (0);			//SE
		}
		else
		{
			//flat surface
			neighborhood[0] = (x > 0 && y > 0) ? (grid[x - 1][y - 1]) : (0);					//NW
			neighborhood[1] = (y > 0) ? (grid[x][y - 1]) : (0);									//N
			neighborhood[2] = (x < WIDTH - 1 && y > 0) ? (grid[x + 1][y - 1]) : (0);			//NE
			neighborhood[3] = (x > 0) ? (grid[x - 1][y]) : (0);									//W
			neighborhood[4] = grid[x][y];														//C
			neighborhood[5] = (x < WIDTH - 1) ? (grid[x + 1][y]) : (0);							//E
			neighborhood[6] = (x > 0 && y < HEIGHT - 1) ? (grid[x - 1][y + 1]) : (0);			//SW
			neighborhood[7] = (y < HEIGHT - 1) ? (grid[x][y + 1]) : (0);						//S
			neighborhood[8] = (x < WIDTH - 1 && y < HEIGHT - 1) ? (grid[x + 1][y + 1]) : (0);	//SE
		}
	}
}

//puts the states of the cells in the von Neumann neighborhood of the cell at (x, y) in the neighborhood array
//cells that share an edge are neighbors
//bools control whether the left edge of the grid should connect to the right and whether the top edge of the grid should connect to the bottom
//anything outside the grid is given a state of 0
void SquareGrid::getVonNeumannNeighborhood(int neighborhood[13], int x, int y, bool shouldLoopHorizontally, bool shouldLoopVertically)
{
	int** grid = (shouldReadGridA) ? (gridA) : (gridB);

	if (shouldLoopHorizontally)
	{
		if (shouldLoopVertically)
		{
			//torus
			neighborhood[0] = grid[x][(y + HEIGHT - 1) % HEIGHT];	//N
			neighborhood[1] = grid[(x + WIDTH - 1) % WIDTH][y];		//W
			neighborhood[2] = grid[x][y];							//C
			neighborhood[3] = grid[(x + 1) % WIDTH][y];				//E
			neighborhood[4] = grid[x][(y + 1) % HEIGHT];			//S
		}
		else
		{
			//vertical cylinder
			neighborhood[0] = (y > 0) ? (grid[x][y - 1]) : (0);				//N
			neighborhood[1] = grid[(x + WIDTH - 1) % WIDTH][y];				//W
			neighborhood[2] = grid[x][y];									//C
			neighborhood[3] = grid[(x + 1) % WIDTH][y];						//E
			neighborhood[4] = (y < HEIGHT - 1) ? (grid[x][y + 1]) : (0);	//S
		}
	}
	else
	{
		if (shouldLoopVertically)
		{
			//horizontal cylinder
			neighborhood[0] = grid[x][(y + HEIGHT - 1) % HEIGHT];		//N
			neighborhood[1] = (x > 0) ? (grid[x - 1][y]) : (0);			//W
			neighborhood[2] = grid[x][y];								//C
			neighborhood[3] = (x < WIDTH - 1) ? (grid[x + 1][y]) : (0);	//E
			neighborhood[4] = grid[x][(y + 1) % HEIGHT];				//S
		}
		else
		{
			//flat surface
			neighborhood[0] = (y > 0) ? (grid[x][y - 1]) : (0);				//N
			neighborhood[1] = (x > 0) ? (grid[x - 1][y]) : (0);				//W
			neighborhood[2] = grid[x][y];									//C
			neighborhood[3] = (x < WIDTH - 1) ? (grid[x + 1][y]) : (0);		//E
			neighborhood[4] = (y < HEIGHT - 1) ? (grid[x][y + 1]) : (0);	//S
		}
	}
}