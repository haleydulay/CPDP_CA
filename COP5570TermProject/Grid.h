#pragma once

#include <SFML/Graphics.hpp>

//grid class
//base class for grids
//virtual methods are for reading, writing, and drawing grid
class Grid
{
public:
	const int WIDTH;	//width of grid in cells
	const int HEIGHT;	//height of grid in cells

	Grid(sf::RenderWindow* window, int width, int height, sf::Color* colors);

	void toggleGrid();

	virtual int getCellState(int x, int y) = 0;
	virtual void setCellState(int state, int x, int y) = 0;

	virtual void getNeighborhood(int neighborhood[13], int x, int y, bool isMooreNeighborhood, bool shouldLoopHorizontally, bool shouldLoopVertically) = 0;

	virtual sf::Vector2i getGridPositionAtMouse() = 0;

	virtual void draw() = 0;

protected:
	sf::RenderWindow* const WINDOW;	//window to draw on
	sf::Color* const COLORS;		//array of colors that maps cell state to color

	bool shouldReadGridA;	//decides which grid to read and which to write
	int** gridA;			//1st 2D array of cell states
	int** gridB;			//2nd 2D array of cell states
};