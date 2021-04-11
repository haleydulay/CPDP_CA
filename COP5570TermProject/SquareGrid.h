#pragma once

#include "Grid.h"

//square grid class
//derived class of grid for square cells
class SquareGrid : public Grid
{
public:
	SquareGrid(sf::RenderWindow* window, int width, int height, sf::Color* colors);

	int getCellState(int x, int y);
	void setCellState(int state, int x, int y);

	void getNeighborhood(int neighborhood[13], int x, int y, bool isMooreNeighborhood, bool shouldLoopHorizontally, bool shouldLoopVertically);

	sf::Vector2i getGridPositionAtMouse();

	void draw();

private:
	float squareLength;		//length of a cell in pixels
	sf::VertexArray image;	//array of cell vertices

	void getMooreNeighborhood(int neighborhood[13], int x, int y, bool shouldLoopHorizontally, bool shouldLoopVertically);
	void getVonNeumannNeighborhood(int neighborhood[13], int x, int y, bool shouldLoopHorizontally, bool shouldLoopVertically);
};