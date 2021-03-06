#include "HexagonalGrid.h"

const float INRADII_PER_CIRCUMRADIUS = 1.1547005383792515290182975610039149112952035025402537520372046529f;
const float SQRT_3 = 1.7320508075688772935274463415058723669428052538103806280558069794f;

//hexagonal grid constructor
//initializes member variables
HexagonalGrid::HexagonalGrid(sf::RenderWindow* window, int width, int height, sf::Color* colors) : Grid(window, width, height, colors)
{
	float circleRadius;
	pixelsPerInradius = (height > 1) ? (getCanvasWidth() / (2.0f * width + 1.0f)) : (getCanvasWidth() / (2.0f * width));
	pixelsPerCircumradius = getCanvasHeight() * 2.0f / (3.0f * height + 1.0f);

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
	image = sf::VertexArray(sf::Triangles, 18 * WIDTH * HEIGHT);

	for (int x = 0; x < WIDTH; ++x)
	{
		gridA[x] = new int[HEIGHT];
		gridB[x] = new int[HEIGHT];

		for (int y = 0; y < HEIGHT; ++y)
		{
			float offX = (y % 2 == 0) ? (x * 2.0f * pixelsPerInradius) : ((x * 2.0f + 1.0f) * pixelsPerInradius);
			float offY = y * 1.5f * pixelsPerCircumradius;

			gridA[x][y] = 0;
			gridB[x][y] = 0;

			image[(HEIGHT * x + y) * 18] = sf::Vertex(sf::Vector2f(offX + pixelsPerInradius, offY + pixelsPerCircumradius), COLORS[0]);						//C
			image[(HEIGHT * x + y) * 18 + 1] = sf::Vertex(sf::Vector2f(offX + pixelsPerInradius, offY), COLORS[0]);											//N
			image[(HEIGHT * x + y) * 18 + 2] = sf::Vertex(sf::Vector2f(offX + 2.0f * pixelsPerInradius, offY + 0.5f * pixelsPerCircumradius), COLORS[0]);	//NE
			image[(HEIGHT * x + y) * 18 + 3] = sf::Vertex(sf::Vector2f(offX + pixelsPerInradius, offY + pixelsPerCircumradius), COLORS[0]);					//C
			image[(HEIGHT * x + y) * 18 + 4] = sf::Vertex(sf::Vector2f(offX + 2.0f * pixelsPerInradius, offY + 0.5f * pixelsPerCircumradius), COLORS[0]);	//NE
			image[(HEIGHT * x + y) * 18 + 5] = sf::Vertex(sf::Vector2f(offX + 2.0f * pixelsPerInradius, offY + 1.5f * pixelsPerCircumradius), COLORS[0]);	//SE
			image[(HEIGHT * x + y) * 18 + 6] = sf::Vertex(sf::Vector2f(offX + pixelsPerInradius, offY + pixelsPerCircumradius), COLORS[0]);					//C
			image[(HEIGHT * x + y) * 18 + 7] = sf::Vertex(sf::Vector2f(offX + 2.0f * pixelsPerInradius, offY + 1.5f * pixelsPerCircumradius), COLORS[0]);	//SE
			image[(HEIGHT * x + y) * 18 + 8] = sf::Vertex(sf::Vector2f(offX + pixelsPerInradius, offY + 2.0f * pixelsPerCircumradius), COLORS[0]);			//S
			image[(HEIGHT * x + y) * 18 + 9] = sf::Vertex(sf::Vector2f(offX + pixelsPerInradius, offY + pixelsPerCircumradius), COLORS[0]);					//C
			image[(HEIGHT * x + y) * 18 + 10] = sf::Vertex(sf::Vector2f(offX + pixelsPerInradius, offY + 2.0f * pixelsPerCircumradius), COLORS[0]);			//S
			image[(HEIGHT * x + y) * 18 + 11] = sf::Vertex(sf::Vector2f(offX, offY + 1.5f * pixelsPerCircumradius), COLORS[0]);								//SW
			image[(HEIGHT * x + y) * 18 + 12] = sf::Vertex(sf::Vector2f(offX + pixelsPerInradius, offY + pixelsPerCircumradius), COLORS[0]);					//C
			image[(HEIGHT * x + y) * 18 + 13] = sf::Vertex(sf::Vector2f(offX, offY + 1.5f * pixelsPerCircumradius), COLORS[0]);								//SW
			image[(HEIGHT * x + y) * 18 + 14] = sf::Vertex(sf::Vector2f(offX, offY + 0.5f * pixelsPerCircumradius), COLORS[0]);								//NW
			image[(HEIGHT * x + y) * 18 + 15] = sf::Vertex(sf::Vector2f(offX + pixelsPerInradius, offY + pixelsPerCircumradius), COLORS[0]);					//C
			image[(HEIGHT * x + y) * 18 + 16] = sf::Vertex(sf::Vector2f(offX, offY + 0.5f * pixelsPerCircumradius), COLORS[0]);								//NW
			image[(HEIGHT * x + y) * 18 + 17] = sf::Vertex(sf::Vector2f(offX + pixelsPerInradius, offY), COLORS[0]);											//N
		}
	}
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

//sets grid[x][y] to state and updates color of cell
void HexagonalGrid::setCellState(int state, int x, int y)
{
	if (shouldReadGridA)
	{
		gridB[x][y] = state;
	}
	else
	{
		gridA[x][y] = state;
	}

	for (int v = 0; v < 18; ++v)
	{
		image[(HEIGHT * x + y) * 18 + v].color = COLORS[state];
	}
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

//gets grid (x, y) at mouse position
//may be outside grid's actual range
//adapted from https://www.redblobgames.com/grids/hexagons/
sf::Vector2i HexagonalGrid::getGridPositionAtMouse()
{
	return convertCubeCoordsToGridPosition(roundCubeCoords(getCubeCoordsAtMouse()));
}

//draws cells
void HexagonalGrid::draw()
{
	WINDOW->draw(image);
}

//gets cube coordinates at mouse position
//the algorithm expects the origin to be at the center of the cell at (0, 0), but it is at the upper-left corner of the bounding box of the cell at (0, 0)
//the offset from the expected origin is taken into account
//adapted from https://www.redblobgames.com/grids/hexagons/
sf::Vector3f HexagonalGrid::getCubeCoordsAtMouse()
{
	sf::Vector2i trueMousePosition = sf::Mouse::getPosition(*WINDOW);
	sf::Vector2f adjustedMousePosition(trueMousePosition.x - pixelsPerInradius, trueMousePosition.y - pixelsPerCircumradius);
	float x = (adjustedMousePosition.x / SQRT_3 - adjustedMousePosition.y / 3.0f) / pixelsPerCircumradius;
	float z = (adjustedMousePosition.y * 2.0f / 3.0f) / pixelsPerCircumradius;

	return sf::Vector3f(x, -x - z, z);
}

//gets the cube coordinates of the cell that the input cube coordinates are in
//adapted from https://www.redblobgames.com/grids/hexagons/
sf::Vector3i HexagonalGrid::roundCubeCoords(const sf::Vector3f& coords)
{
	int xRound = (int)roundf(coords.x);
	int yRound = (int)roundf(coords.y);
	int zRound = (int)roundf(coords.z);

	float dx = fabsf(xRound - coords.x);
	float dy = fabsf(yRound - coords.y);
	float dz = fabsf(zRound - coords.z);

	if (dx > dy && dx > dz)
	{
		xRound = -yRound - zRound;
	}
	else if (dy > dz)
	{
		yRound = -xRound - zRound;
	}
	else
	{
		zRound = -xRound - yRound;
	}

	return sf::Vector3i(xRound, yRound, zRound);
}

//converts from cube coordinate system to grid coordinate system
//adapted from https://www.redblobgames.com/grids/hexagons/
sf::Vector2i HexagonalGrid::convertCubeCoordsToGridPosition(const sf::Vector3i& coords)
{
	int y = coords.z;
	int x = (y % 2 == 0) ? (coords.x + y / 2) : (coords.x + (y - 1) / 2);

	return sf::Vector2i(x, y);
}