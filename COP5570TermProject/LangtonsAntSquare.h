#pragma once

#include "CellularAutomaton.h"
#include "SquareGrid.h"

//neighbor
const int NEIGHBOR_SQUARE_E = 3;
const int NEIGHBOR_SQUARE_N = 0;
const int NEIGHBOR_SQUARE_W = 1;
const int NEIGHBOR_SQUARE_S = 4;

//cell
const int MASK_SQUARE_CELL = 1;
const int STATE_SQUARE_ON = 1;

//ant
const int IS_SQUARE_ANT = 4;
const int REVERSE_SQUARE_ANT = 2;
const int MASK_SQUARE_ANT = 6;
const int STATE_SQUARE_CLOCKWISE = 4;
const int STATE_SQUARE_COUNTER = 6;

//facing
const int MASK_SQUARE_FACING = 24;
const int STATE_SQUARE_E = 0;
const int STATE_SQUARE_N = 8;
const int STATE_SQUARE_W = 16;
const int STATE_SQUARE_S = 24;

//derived class of cellular automaton for Langton's Ant on square grids
class LangtonsAntSquare : public CellularAutomaton
{
public:
	LangtonsAntSquare(SquareGrid* grid, bool shouldLoopHorizontally, bool shouldLoopVertically);

	int getWidth();
	int getHeight();

	void toggleGrid();
	void update(int minX, int minY, int maxX, int maxY);

private:
	SquareGrid* const GRID;	//grid of cells

	bool isAntComingHere(int x, int y, int state);
};