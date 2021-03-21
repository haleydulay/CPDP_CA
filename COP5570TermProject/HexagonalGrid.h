#pragma once

#include "Grid.h"

//hexagonal grid class
//derived class of grid for hexagonal cells
//pointy-top with odd-numbered rows shifted right
class HexagonalGrid : Grid
{
public:
	HexagonalGrid(sf::RenderWindow* window, int width, int height, sf::Color* colors);
	~HexagonalGrid();

	int getCellState(int x, int y);
	void setCellState(int state, int x, int y);

	void getNeighborhood(int neighborhood[13], int x, int y, bool isMooreNeighborhood, bool shouldLoopHorizontally, bool shouldLoopVertically);

	void draw();

private:
	sf::CircleShape*** image;	//2D array of cell images
};