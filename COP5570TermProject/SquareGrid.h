#pragma once

#include "Grid.h"

//square grid class
//derived class of grid for square cells
class SquareGrid : Grid
{
public:
	SquareGrid(sf::RenderWindow* window, int width, int height, sf::Color* colors);
	~SquareGrid();

	int getIntFromGrid(int x, int y);
	void setIntToGrid(int value, int x, int y);

	void draw();

private:
	const int WIDTH;			//width of grid in squares
	const int HEIGHT;			//height of grid in squares
	sf::Color* const COLORS;	//array of colors that maps cell state to color
	
	sf::RectangleShape*** image;	//2D array of cell images
};