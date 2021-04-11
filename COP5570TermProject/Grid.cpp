#include "Grid.h"

//grid constructor
//initializes member variables
Grid::Grid(sf::RenderWindow* window, int width, int height, sf::Color* colors) : WINDOW(window), WIDTH(width), HEIGHT(height), COLORS(colors)
{
	shouldReadGridA = true;
}

//toggles shouldReadGridA
//should be called between time steps
void Grid::toggleGrid()
{
	shouldReadGridA = !shouldReadGridA;
}

unsigned int Grid::getCanvasWidth()
{
	return (WINDOW->getSize().x * 3) / 4;
}

unsigned int Grid::getCanvasHeight()
{
	return WINDOW->getSize().y;
}
