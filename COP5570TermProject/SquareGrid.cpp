#include "SquareGrid.h"

//square grid constructor
//initializes member variables
SquareGrid::SquareGrid(sf::RenderWindow* window, int width, int height, sf::Color* colors) : Grid(window), WIDTH(width), HEIGHT(height), COLORS(colors)
{
	float rectangleWidth = window->getSize().x / (float)width;
	float rectangleHeight = window->getSize().y / (float)height;
	float squareLength = (rectangleWidth < rectangleHeight) ? (rectangleWidth) : (rectangleHeight);
	sf::Vector2f squareSize(squareLength, squareLength);

	grid = new int*[WIDTH];
	image = new sf::RectangleShape**[WIDTH];

	for (int x = 0; x < WIDTH; ++x)
	{
		grid[x] = new int[HEIGHT];
		image[x] = new sf::RectangleShape*[HEIGHT];

		for (int y = 0; y < WIDTH; ++y)
		{
			grid[x][y] = 0;
			image[x][y] = new sf::RectangleShape(squareSize);
			image[x][y]->setPosition(x * squareLength, y * squareLength);
			image[x][y]->setFillColor(COLORS[0]);
		}
	}
}

//square grid destructor
//deletes grid and image
SquareGrid::~SquareGrid()
{
	for (int x = 0; x < WIDTH; ++x)
	{
		for (int y = 0; y < WIDTH; ++y)
		{
			delete image[x][y];
		}

		delete[] grid[x];
		delete[] image[x];
	}

	delete[] grid;
	delete[] image;
}

//returns grid[x][y]
int SquareGrid::getIntFromGrid(int x, int y)
{
	return grid[x][y];
}

//sets grid[x][y] to value and updates color of cell
void SquareGrid::setIntToGrid(int value, int x, int y)
{
	grid[x][y] = value;
	image[x][y]->setFillColor(COLORS[value]);
}

//draws cells
void SquareGrid::draw()
{
	for (int x = 0; x < WIDTH; ++x)
	{
		for (int y = 0; y < WIDTH; ++y)
		{
			WINDOW->draw(*image[x][y]);
		}
	}
}