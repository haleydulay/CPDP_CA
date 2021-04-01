#pragma once

#include "CellularAutomaton.h"
#include "TriangularGrid.h"

//neighbor
const int NEIGHBOR_TRIANGULAR_NE = 2;
const int NEIGHBOR_TRIANGULAR_N = 0;
const int NEIGHBOR_TRIANGULAR_NW = 0;
const int NEIGHBOR_TRIANGULAR_SW = 1;
const int NEIGHBOR_TRIANGULAR_S = 3;
const int NEIGHBOR_TRIANGULAR_SE = 3;

//cell
const int MASK_TRIANGULAR_CELL = 1;
const int STATE_TRIANGULAR_ON = 1;

//ant
const int IS_TRIANGULAR_ANT = 4;
const int REVERSE_TRIANGULAR_ANT = 2;
const int MASK_TRIANGULAR_ANT = 6;
const int STATE_TRIANGULAR_CLOCKWISE = 4;
const int STATE_TRIANGULAR_COUNTER = 6;

//facing
const int MASK_TRIANGULAR_FACING = 24;
const int STATE_TRIANGULAR_E = 0;
const int STATE_TRIANGULAR_NS = 8;
const int STATE_TRIANGULAR_W = 16;

//derived class of cellular automaton for Langton's Ant on triangular grids
class LangtonsAntTriangular : public CellularAutomaton
{
public:
	LangtonsAntTriangular(TriangularGrid* grid, bool shouldLoopHorizontally, bool shouldLoopVertically);

	int getWidth();
	int getHeight();

	void toggleGrid();
	void update(int minX, int minY, int maxX, int maxY);

private:
	TriangularGrid* const GRID;	//grid of cells

	bool isAntComingHereUp(int x, int y, int state);
	bool isAntComingHereDown(int x, int y, int state);
};