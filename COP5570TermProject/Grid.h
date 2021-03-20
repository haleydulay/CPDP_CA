#pragma once

#include <SFML/Graphics.hpp>

//grid class
//base class for grids
//virtual methods are for reading, writing, and drawing grid
class Grid
{
public:
	Grid(sf::RenderWindow* window);

	virtual int getIntFromGrid(int x, int y) = 0;
	virtual void setIntToGrid(int value, int x, int y) = 0;

	virtual void draw() = 0;

protected:
	sf::RenderWindow* const WINDOW;	//window to draw on

	int** grid;	//2D array of cell states
};