#pragma once

#include "Grid.h"

//hexagonal grid class
//derived class of grid for hexagonal cells
//pointy-top with odd-numbered rows shifted right
class HexagonalGrid : public Grid
{
public:
	HexagonalGrid(sf::RenderWindow* window, int width, int height, sf::Color* colors);
	~HexagonalGrid();

	int getCellState(int x, int y);
	void setCellState(int state, int x, int y);

	void getNeighborhood(int neighborhood[13], int x, int y, bool isMooreNeighborhood, bool shouldLoopHorizontally, bool shouldLoopVertically);

	sf::Vector2i getGridPositionAtMouse();

	void draw();

private:
	float pixelsPerInradius;		//half the width of a cell in pixels
	float pixelsPerCircumradius;	//half the height of a cell in pixels
	sf::CircleShape*** image;		//2D array of cell images

	sf::Vector3f getCubeCoordsAtMouse();
	sf::Vector3i roundCubeCoords(const sf::Vector3f& coords);
	sf::Vector2i convertCubeCoordsToGridPosition(const sf::Vector3i& coords);
};