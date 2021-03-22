#pragma once

#include "Grid.h"

//triangular grid class
//derived class of grid for triangular cells
//rows are separated by straight lines
//grid[x][y] is a triangle that points up if x + y is even and down otherwise
class TriangularGrid : public Grid
{
public:
	TriangularGrid(sf::RenderWindow* window, int width, int height, sf::Color* colors);
	~TriangularGrid();

	int getCellState(int x, int y);
	void setCellState(int state, int x, int y);

	void getNeighborhood(int neighborhood[13], int x, int y, bool isMooreNeighborhood, bool shouldLoopHorizontally, bool shouldLoopVertically);

	sf::Vector2i getGridPositionAtMouse();

	void draw();

private:
	sf::CircleShape*** image;	//2D array of cell images

	void getMooreNeighborhoodForUpTriangle(int neighborhood[13], int x, int y, bool shouldLoopHorizontally, bool shouldLoopVertically);
	void getMooreNeighborhoodForDownTriangle(int neighborhood[13], int x, int y, bool shouldLoopHorizontally, bool shouldLoopVertically);
	void getVonNeumannNeighborhoodForUpTriangle(int neighborhood[13], int x, int y, bool shouldLoopHorizontally, bool shouldLoopVertically);
	void getVonNeumannNeighborhoodForDownTriangle(int neighborhood[13], int x, int y, bool shouldLoopHorizontally, bool shouldLoopVertically);
};