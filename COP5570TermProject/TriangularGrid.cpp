#include "TriangularGrid.h"

const float CIRCUMRADII_PER_HEIGHT = 1.5f;
const float CIRCUMRADII_PER_BASE = 1.7320508075688772935274463415058723669428052538103806280558069794f;

//triangle grid constructor
//initializes member variables
TriangularGrid::TriangularGrid(sf::RenderWindow* window, int width, int height, sf::Color* colors) : Grid(window, width, height, colors)
{
	float pixelsPerBase = window->getSize().x * 2.0f / (width + 1.0f);
	float pixelsPerHeight = window->getSize().y / (float)height;
	float circleRadius;

	if (pixelsPerBase / CIRCUMRADII_PER_BASE < pixelsPerHeight / CIRCUMRADII_PER_HEIGHT)
	{
		circleRadius = pixelsPerBase / CIRCUMRADII_PER_BASE;
		pixelsPerHeight = circleRadius * CIRCUMRADII_PER_HEIGHT;
	}
	else
	{
		circleRadius = pixelsPerHeight / CIRCUMRADII_PER_HEIGHT;
		pixelsPerBase = circleRadius * CIRCUMRADII_PER_BASE;
	}

	gridA = new int*[WIDTH];
	gridB = new int*[WIDTH];
	image = new sf::CircleShape**[WIDTH];

	for (int x = 0; x < WIDTH; ++x)
	{
		gridA[x] = new int[HEIGHT];
		gridB[x] = new int[HEIGHT];
		image[x] = new sf::CircleShape*[HEIGHT];

		for (int y = 0; y < HEIGHT; ++y)
		{
			gridA[x][y] = 0;
			gridB[x][y] = 0;
			image[x][y] = new sf::CircleShape(circleRadius, 3);
			image[x][y]->setFillColor(COLORS[0]);

			if ((x + y) % 2 == 0)
			{
				image[x][y]->setPosition(x * pixelsPerBase / 2.0f, y * pixelsPerHeight);
			}
			else
			{
				image[x][y]->setRotation(180.0f);
				image[x][y]->setPosition(x * pixelsPerBase / 2.0f + 2.0f * circleRadius, y * pixelsPerHeight + pixelsPerHeight);
			}
		}
	}
}

//triangle grid destructor
//deletes grid and image
TriangularGrid::~TriangularGrid()
{
	for (int x = 0; x < WIDTH; ++x)
	{
		for (int y = 0; y < HEIGHT; ++y)
		{
			delete image[x][y];
		}

		delete[] gridA[x];
		delete[] gridB[x];
		delete[] image[x];
	}

	delete[] gridA;
	delete[] gridB;
	delete[] image;
}

//returns grid[x][y]
int TriangularGrid::getCellState(int x, int y)
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
void TriangularGrid::setCellState(int state, int x, int y)
{
	if (shouldReadGridA)
	{
		gridB[x][y] = state;
	}
	else
	{
		gridA[x][y] = state;
	}

	image[x][y]->setFillColor(COLORS[state]);
}

//puts the states of the cells in the neighborhood of the cell at (x, y) in the neighborhood array
//bools control whether the left edge of the grid should connect to the right and whether the top edge of the grid should connect to the bottom
//anything outside the grid is given a state of 0
void TriangularGrid::getNeighborhood(int neighborhood[13], int x, int y, bool isMooreNeighborhood, bool shouldLoopHorizontally, bool shouldLoopVertically)
{
	if (isMooreNeighborhood)
	{
		if ((x + y) % 2 == 0)
		{
			getMooreNeighborhoodForUpTriangle(neighborhood, x, y, shouldLoopHorizontally, shouldLoopVertically);
		}
		else
		{
			getMooreNeighborhoodForDownTriangle(neighborhood, x, y, shouldLoopHorizontally, shouldLoopVertically);
		}
	}
	else
	{
		if ((x + y) % 2 == 0)
		{
			getVonNeumannNeighborhoodForUpTriangle(neighborhood, x, y, shouldLoopHorizontally, shouldLoopVertically);
		}
		else
		{
			getVonNeumannNeighborhoodForDownTriangle(neighborhood, x, y, shouldLoopHorizontally, shouldLoopVertically);
		}
	}
}

sf::Vector2i TriangularGrid::getGridPositionAtMouse()
{
	return sf::Vector2i();
}

//draws cells
void TriangularGrid::draw()
{
	for (int x = 0; x < WIDTH; ++x)
	{
		for (int y = 0; y < HEIGHT; ++y)
		{
			WINDOW->draw(*image[x][y]);
		}
	}
}

//puts the states of the cells in the Moore neighborhood of the upward-pointing cell at (x, y) in the neighborhood array
//cells that share a vertex are neighbors
//bools control whether the left edge of the grid should connect to the right and whether the top edge of the grid should connect to the bottom
//anything outside the grid is given a state of 0
void TriangularGrid::getMooreNeighborhoodForUpTriangle(int neighborhood[13], int x, int y, bool shouldLoopHorizontally, bool shouldLoopVertically)
{
	//		NW	N	NE
	//	WW	W	C	E	EE
	//	WSW	SSW	S	SSE	ESE
	int** grid = (shouldReadGridA) ? (gridA) : (gridB);
	int n = -1;

	if (shouldLoopHorizontally)
	{
		if (shouldLoopVertically)
		{
			//torus
			for (int dy = -1; dy < 2; ++dy)
			{
				for (int dx = ((dy < 0) ? (-1) : (-2)); dx < ((dy < 0) ? (1) : (2)); ++dx)
				{
					neighborhood[++n] = grid[(x + dx + WIDTH) % WIDTH][(y + dy + HEIGHT) % HEIGHT];
				}
			}
		}
		else
		{
			//vertical cylinder
			for (int dy = -1; dy < 2; ++dy)
			{
				for (int dx = ((dy < 0) ? (-1) : (-2)); dx < ((dy < 0) ? (1) : (2)); ++dx)
				{
					neighborhood[++n] = (x + dx > -1 && x + dx < WIDTH) ? (grid[x + dx][(y + dy + HEIGHT) % HEIGHT]) : (0);
				}
			}
		}
	}
	else
	{
		if (shouldLoopVertically)
		{
			//horizontal cylinder
			for (int dy = -1; dy < 2; ++dy)
			{
				for (int dx = ((dy < 0) ? (-1) : (-2)); dx < ((dy < 0) ? (1) : (2)); ++dx)
				{
					neighborhood[++n] = (y + dy > -1 && y + dy < HEIGHT) ? (grid[(x + dx + WIDTH) % WIDTH][y + dy]) : (0);
				}
			}
		}
		else
		{
			//flat surface
			for (int dy = -1; dy < 2; ++dy)
			{
				for (int dx = ((dy < 0) ? (-1) : (-2)); dx < ((dy < 0) ? (1) : (2)); ++dx)
				{
					neighborhood[++n] = (x + dx > -1 && x + dx < WIDTH && y + dy > -1 && y + dy < HEIGHT) ? (grid[x + dx][y + dy]) : (0);
				}
			}
		}
	}
}

//puts the states of the cells in the Moore neighborhood of the downward-pointing cell at (x, y) in the neighborhood array
//cells that share a vertex are neighbors
//bools control whether the left edge of the grid should connect to the right and whether the top edge of the grid should connect to the bottom
//anything outside the grid is given a state of 0
void TriangularGrid::getMooreNeighborhoodForDownTriangle(int neighborhood[13], int x, int y, bool shouldLoopHorizontally, bool shouldLoopVertically)
{
	//	WNW NNW	N	NNE	ENE
	//	WW	W	C	E	EE
	//		SW	S	E
	int** grid = (shouldReadGridA) ? (gridA) : (gridB);
	int n = -1;

	if (shouldLoopHorizontally)
	{
		if (shouldLoopVertically)
		{
			//torus
			for (int dy = -1; dy < 2; ++dy)
			{
				for (int dx = ((dy > 0) ? (-1) : (-2)); dx < ((dy > 0) ? (1) : (2)); ++dx)
				{
					neighborhood[++n] = grid[(x + dx + WIDTH) % WIDTH][(y + dy + HEIGHT) % HEIGHT];
				}
			}
		}
		else
		{
			//vertical cylinder
			for (int dy = -1; dy < 2; ++dy)
			{
				for (int dx = ((dy > 0) ? (-1) : (-2)); dx < ((dy > 0) ? (1) : (2)); ++dx)
				{
					neighborhood[++n] = (x + dx > -1 && x + dx < WIDTH) ? (grid[x + dx][(y + dy + HEIGHT) % HEIGHT]) : (0);
				}
			}
		}
	}
	else
	{
		if (shouldLoopVertically)
		{
			//horizontal cylinder
			for (int dy = -1; dy < 2; ++dy)
			{
				for (int dx = ((dy > 0) ? (-1) : (-2)); dx < ((dy > 0) ? (1) : (2)); ++dx)
				{
					neighborhood[++n] = (y + dy > -1 && y + dy < HEIGHT) ? (grid[(x + dx + WIDTH) % WIDTH][y + dy]) : (0);
				}
			}
		}
		else
		{
			//flat surface
			for (int dy = -1; dy < 2; ++dy)
			{
				for (int dx = ((dy > 0) ? (-1) : (-2)); dx < ((dy > 0) ? (1) : (2)); ++dx)
				{
					neighborhood[++n] = (x + dx > -1 && x + dx < WIDTH && y + dy > -1 && y + dy < HEIGHT) ? (grid[x + dx][y + dy]) : (0);
				}
			}
		}
	}
}

//puts the states of the cells in the von Neumann neighborhood of the upward-pointing cell at (x, y) in the neighborhood array
//cells that share an edge are neighbors
//bools control whether the left edge of the grid should connect to the right and whether the top edge of the grid should connect to the bottom
//anything outside the grid is given a state of 0
void TriangularGrid::getVonNeumannNeighborhoodForUpTriangle(int neighborhood[13], int x, int y, bool shouldLoopHorizontally, bool shouldLoopVertically)
{
	int** grid = (shouldReadGridA) ? (gridA) : (gridB);

	if (shouldLoopHorizontally)
	{
		if (shouldLoopVertically)
		{
			//torus
			neighborhood[0] = grid[(x + WIDTH - 1) % WIDTH][y];		//W
			neighborhood[1] = grid[x][y];							//C
			neighborhood[2] = grid[(x + 1) % WIDTH][y];				//E
			neighborhood[3] = grid[x][(y + 1) % HEIGHT];			//S
		}
		else
		{
			//vertical cylinder
			neighborhood[0] = grid[(x + WIDTH - 1) % WIDTH][y];				//W
			neighborhood[1] = grid[x][y];									//C
			neighborhood[2] = grid[(x + 1) % WIDTH][y];						//E
			neighborhood[3] = (y < HEIGHT - 1) ? (grid[x][y + 1]) : (0);	//S
		}
	}
	else
	{
		if (shouldLoopVertically)
		{
			//horizontal cylinder
			neighborhood[0] = (x > 0) ? (grid[x - 1][y]) : (0);			//W
			neighborhood[1] = grid[x][y];								//C
			neighborhood[2] = (x < WIDTH - 1) ? (grid[x + 1][y]) : (0);	//E
			neighborhood[3] = grid[x][(y + 1) % HEIGHT];				//S
		}
		else
		{
			//flat surface
			neighborhood[0] = (x > 0) ? (grid[x - 1][y]) : (0);				//W
			neighborhood[1] = grid[x][y];									//C
			neighborhood[2] = (x < WIDTH - 1) ? (grid[x + 1][y]) : (0);		//E
			neighborhood[3] = (y < HEIGHT - 1) ? (grid[x][y + 1]) : (0);	//S
		}
	}
}

//puts the states of the cells in the von Neumann neighborhood of the downward-pointing cell at (x, y) in the neighborhood array
//cells that share an edge are neighbors
//bools control whether the left edge of the grid should connect to the right and whether the top edge of the grid should connect to the bottom
//anything outside the grid is given a state of 0
void TriangularGrid::getVonNeumannNeighborhoodForDownTriangle(int neighborhood[13], int x, int y, bool shouldLoopHorizontally, bool shouldLoopVertically)
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
		}
		else
		{
			//vertical cylinder
			neighborhood[0] = (y > 0) ? (grid[x][y - 1]) : (0);				//N
			neighborhood[1] = grid[(x + WIDTH - 1) % WIDTH][y];				//W
			neighborhood[2] = grid[x][y];									//C
			neighborhood[3] = grid[(x + 1) % WIDTH][y];						//E
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
		}
		else
		{
			//flat surface
			neighborhood[0] = (y > 0) ? (grid[x][y - 1]) : (0);				//N
			neighborhood[1] = (x > 0) ? (grid[x - 1][y]) : (0);				//W
			neighborhood[2] = grid[x][y];									//C
			neighborhood[3] = (x < WIDTH - 1) ? (grid[x + 1][y]) : (0);		//E
		}
	}
}