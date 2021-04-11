#include "TriangularGrid.h"

const float CIRCUMRADII_PER_HEIGHT = 1.5f;
const float CIRCUMRADII_PER_BASE = 1.7320508075688772935274463415058723669428052538103806280558069794f;

//triangle grid constructor
//initializes member variables
TriangularGrid::TriangularGrid(sf::RenderWindow* window, int width, int height, sf::Color* colors) : Grid(window, width, height, colors)
{
	float circleRadius;
	
	pixelsPerBase = getCanvasWidth() * 2.0f / (width + 1.0f);
	pixelsPerHeight = getCanvasHeight() / (float)height;

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
	image = sf::VertexArray(sf::Triangles, 3 * WIDTH * HEIGHT);

	for (int x = 0; x < WIDTH; ++x)
	{
		gridA[x] = new int[HEIGHT];
		gridB[x] = new int[HEIGHT];

		for (int y = 0; y < HEIGHT; ++y)
		{
			float offX = x * 0.5f * pixelsPerBase;
			float offY = y * pixelsPerHeight;

			gridA[x][y] = 0;
			gridB[x][y] = 0;

			if ((x + y) % 2 == 0)
			{
				image[(HEIGHT * x + y) * 3] = sf::Vertex(sf::Vector2f(offX + 0.5f * pixelsPerBase, offY), COLORS[0]);					//N
				image[(HEIGHT * x + y) * 3 + 1] = sf::Vertex(sf::Vector2f(offX + pixelsPerBase, offY + pixelsPerHeight), COLORS[0]);	//SE
				image[(HEIGHT * x + y) * 3 + 2] = sf::Vertex(sf::Vector2f(offX, offY + pixelsPerHeight), COLORS[0]);					//SW
			}
			else
			{
				image[(HEIGHT * x + y) * 3] = sf::Vertex(sf::Vector2f(offX, offY), COLORS[0]);												//NW
				image[(HEIGHT * x + y) * 3 + 1] = sf::Vertex(sf::Vector2f(offX + pixelsPerBase, offY), COLORS[0]);							//NE
				image[(HEIGHT * x + y) * 3 + 2] = sf::Vertex(sf::Vector2f(offX + 0.5f * pixelsPerBase, offY + pixelsPerHeight), COLORS[0]);	//S
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
		delete[] gridA[x];
		delete[] gridB[x];
	}

	delete[] gridA;
	delete[] gridB;
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

	image[(HEIGHT * x + y) * 3].color = COLORS[state];
	image[(HEIGHT * x + y) * 3 + 1].color = COLORS[state];
	image[(HEIGHT * x + y) * 3 + 2].color = COLORS[state];
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

//gets grid (x, y) at mouse position
//may be outside grid's actual range
sf::Vector2i TriangularGrid::getGridPositionAtMouse()
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*WINDOW);
	float numBases = mousePosition.x / pixelsPerBase;
	float numHeights = mousePosition.y / pixelsPerHeight;
	int x = (int)floorf(numBases);
	int y = (int)floorf(numHeights);

	numBases -= x;
	numHeights -= y;
	x *= 2;

	if (numBases < 0.5f)							//left of center, may actually be in (x - 1, y)
	{
		if (y % 2 == 0)								//(x, y) points up, border with (x - 1, y) is slanted upward
		{
			if (numHeights < 2 * (0.5f - numBases))	//+y is down, in (x - 1, y) if mouse above line
			{
				--x;
			}
		}
		else										//(x, y) points down, border with (x - 1, y) is slanted downward
		{
			if (numHeights > 2 * numBases)			//+y is down, in (x - 1, y) if mouse below line
			{
				--x;
			}
		}
	}
	else											//right of center, may actually be in (x + 1, y)
	{
		numBases -= 0.5f;

		if (y % 2 == 0)								//(x, y) points up, border with (x + 1, y) is slanted downward
		{
			if (numHeights < 2 * numBases)			//+y is down, in (x + 1, y) if mouse above line
			{
				++x;
			}
		}
		else										//(x, y) points down, border with (x + 1, y) is slanted upward
		{
			if (numHeights > 2 * (0.5f - numBases))	//+y is down, in (x + 1, y) if mouse below line
			{
				++x;
			}
		}
	}

	return sf::Vector2i(x, y);
}

//draws cells
void TriangularGrid::draw()
{
	WINDOW->draw(image);
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
				for (int dx = ((dy < 0) ? (-1) : (-2)); dx < ((dy < 0) ? (2) : (3)); ++dx)
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
				for (int dx = ((dy < 0) ? (-1) : (-2)); dx < ((dy < 0) ? (2) : (3)); ++dx)
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
				for (int dx = ((dy < 0) ? (-1) : (-2)); dx < ((dy < 0) ? (2) : (3)); ++dx)
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
				for (int dx = ((dy < 0) ? (-1) : (-2)); dx < ((dy < 0) ? (2) : (3)); ++dx)
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
				for (int dx = ((dy > 0) ? (-1) : (-2)); dx < ((dy > 0) ? (2) : (3)); ++dx)
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
				for (int dx = ((dy > 0) ? (-1) : (-2)); dx < ((dy > 0) ? (2) : (3)); ++dx)
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
				for (int dx = ((dy > 0) ? (-1) : (-2)); dx < ((dy > 0) ? (2) : (3)); ++dx)
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
				for (int dx = ((dy > 0) ? (-1) : (-2)); dx < ((dy > 0) ? (2) : (3)); ++dx)
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