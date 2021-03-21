#include "HexagonalGrid.h"

const float INRADII_PER_CIRCUMRADIUS = 1.1547005383792515290182975610039149112952035025402537520372046529f;

//hexagonal grid constructor
//initializes member variables
HexagonalGrid::HexagonalGrid(sf::RenderWindow* window, int width, int height, sf::Color* colors) : Grid(window, width, height, colors)
{
	float pixelsPerInradius = (height > 1) ? (window->getSize().x / (2.0f * width + 1.0f)) : (window->getSize().x / (2.0f * width));
	float pixelsPerCircumradius = window->getSize().y * 2.0f / (3.0f * height + 1.0f);
	float circleRadius;

	if (pixelsPerInradius * INRADII_PER_CIRCUMRADIUS < pixelsPerCircumradius)
	{
		circleRadius = pixelsPerInradius * INRADII_PER_CIRCUMRADIUS;
		pixelsPerCircumradius = circleRadius;
	}
	else
	{
		circleRadius = pixelsPerCircumradius;
		pixelsPerInradius = circleRadius / INRADII_PER_CIRCUMRADIUS;
	}

	gridA = new int*[WIDTH];
	gridB = new int*[WIDTH];
	image = new sf::CircleShape**[WIDTH];

	for (int x = 0; x < WIDTH; ++x)
	{
		gridA[x] = new int[HEIGHT];
		gridB[x] = new int[HEIGHT];
		image[x] = new sf::CircleShape*[HEIGHT];

		for (int y = 0; y < WIDTH; ++y)
		{
			gridA[x][y] = 0;
			gridB[x][y] = 0;
			image[x][y] = new sf::CircleShape(circleRadius, 6);
			image[x][y]->setPosition((y % 2 == 0) ? (x * 2.0f * pixelsPerInradius) : ((x * 2.0f + 1.0f) * pixelsPerInradius), y * 1.5f * pixelsPerCircumradius);
			image[x][y]->setFillColor(COLORS[0]);
		}
	}
}

//square grid destructor
//deletes grid and image
HexagonalGrid::~HexagonalGrid()
{
	for (int x = 0; x < WIDTH; ++x)
	{
		for (int y = 0; y < WIDTH; ++y)
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
int HexagonalGrid::getCellState(int x, int y)
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

//sets grid[x][y] to value and updates color of cell
void HexagonalGrid::setCellState(int value, int x, int y)
{
	if (shouldReadGridA)
	{
		gridB[x][y] = value;
	}
	else
	{
		gridA[x][y] = value;
	}

	image[x][y]->setFillColor(COLORS[value]);
}

//puts the states of the cells in the neighborhood of the cell at (x, y) in the neighborhood array
//bools control whether the left edge of the grid should connect to the right and whether the top edge of the grid should connect to the bottom
//anything outside the grid is given a state of 0
void HexagonalGrid::getNeighborhood(int neighborhood[13], int x, int y, bool isMooreNeighborhood, bool shouldLoopHorizontally, bool shouldLoopVertically)
{
	int** grid = (shouldReadGridA) ? (gridA) : (gridB);
	int otherRowEastOffset = y % 2;						//0 or 1
	int otherRowWestOffset = otherRowEastOffset - 1;	//-1 or 0

	if (shouldLoopHorizontally)
	{
		if (shouldLoopVertically)
		{
			//torus
			neighborhood[0] = grid[(x + WIDTH + otherRowWestOffset) % WIDTH][(y + HEIGHT - 1) % HEIGHT];	//NW
			neighborhood[1] = grid[(x + otherRowEastOffset) % WIDTH][(y + HEIGHT - 1) % HEIGHT];			//NE
			neighborhood[2] = grid[(x + WIDTH - 1) % WIDTH][y];												//W
			neighborhood[3] = grid[x][y];																	//C
			neighborhood[4] = grid[(x + 1) % WIDTH][y];														//E
			neighborhood[5] = grid[(x + WIDTH + otherRowWestOffset) % WIDTH][(y + 1) % HEIGHT];				//SW
			neighborhood[6] = grid[(x + otherRowEastOffset) % WIDTH][(y + 1) % HEIGHT];						//SE
		}
		else
		{
			//vertical cylinder
			neighborhood[0] = (y > 0) ? (grid[(x + WIDTH + otherRowWestOffset) % WIDTH][y - 1]) : (0);			//NW
			neighborhood[1] = (y > 0) ? (grid[(x + otherRowEastOffset) % WIDTH][y - 1]) : (0);					//NE
			neighborhood[2] = grid[(x + WIDTH - 1) % WIDTH][y];													//W
			neighborhood[3] = grid[x][y];																		//C
			neighborhood[4] = grid[(x + 1) % WIDTH][y];															//E
			neighborhood[5] = (y < HEIGHT - 1) ? (grid[(x + WIDTH + otherRowWestOffset) % WIDTH][y + 1]) : (0);	//SW
			neighborhood[6] = (y < HEIGHT - 1) ? (grid[(x + otherRowEastOffset) % WIDTH][y + 1]) : (0);			//SE
		}
	}
	else
	{
		if (shouldLoopVertically)
		{
			//horizontal cylinder
			neighborhood[0] = (x + otherRowWestOffset > -1) ? (grid[x + otherRowWestOffset][(y + HEIGHT - 1) % HEIGHT]) : (0);		//NW
			neighborhood[1] = (x + otherRowEastOffset < WIDTH) ? (grid[x + otherRowEastOffset][(y + HEIGHT - 1) % HEIGHT]) : (0);	//NE
			neighborhood[2] = (x > 0) ? (grid[x - 1][y]) : (0);																		//W
			neighborhood[3] = grid[x][y];																							//C
			neighborhood[4] = (x < WIDTH - 1) ? (grid[x + 1][y]) : (0);																//E
			neighborhood[5] = (x + otherRowWestOffset > -1) ? (grid[x + otherRowWestOffset][(y + 1) % HEIGHT]) : (0);				//SW
			neighborhood[6] = (x + otherRowEastOffset < WIDTH) ? (grid[x + otherRowEastOffset][(y + 1) % HEIGHT]) : (0);			//SE
		}
		else
		{
			//flat surface
			neighborhood[0] = (x + otherRowWestOffset > -1 && y > 0) ? (grid[x + otherRowWestOffset][y - 1]) : (0);				//NW
			neighborhood[1] = (x + otherRowEastOffset < WIDTH && y > 0) ? (grid[x + otherRowEastOffset][y - 1]) : (0);			//NE
			neighborhood[2] = (x > 0) ? (grid[x - 1][y]) : (0);																	//W
			neighborhood[3] = grid[x][y];																						//C
			neighborhood[4] = (x < WIDTH - 1) ? (grid[x + 1][y]) : (0);															//E
			neighborhood[5] = (x + otherRowWestOffset > -1 && y < HEIGHT - 1) ? (grid[x + otherRowWestOffset][y + 1]) : (0);	//SW
			neighborhood[6] = (x + otherRowEastOffset < WIDTH && y < HEIGHT - 1) ? (grid[x + otherRowEastOffset][y + 1]) : (0);	//SE
		}
	}
}

//draws cells
void HexagonalGrid::draw()
{
	for (int x = 0; x < WIDTH; ++x)
	{
		for (int y = 0; y < WIDTH; ++y)
		{
			WINDOW->draw(*image[x][y]);
		}
	}
}