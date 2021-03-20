#pragma once

#include "Grid.h"

//square grid class
//derived class of grid for square cells
class SquareGrid : Grid
{
public:
	SquareGrid(sf::RenderWindow* window, int width, int height, sf::Color* colors);
	~SquareGrid();

	int getCellState(int x, int y);
	void setCellState(int state, int x, int y);

	void getMooreNeighborhood(int neighborhood[13], int x, int y, bool shouldLoopHorizontally, bool shouldLoopVertically);
	void getVonNeumannNeighborhood(int neighborhood[13], int x, int y, bool shouldLoopHorizontally, bool shouldLoopVertically);

	void draw();

private:
	sf::RectangleShape*** image;	//2D array of cell images
};