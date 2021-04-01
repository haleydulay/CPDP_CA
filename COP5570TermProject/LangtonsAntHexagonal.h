#pragma once

#include "CellularAutomaton.h"
#include "HexagonalGrid.h"

//neighbor
const int NEIGHBOR_HEXAGONAL_E = 4;
const int NEIGHBOR_HEXAGONAL_NE = 1;
const int NEIGHBOR_HEXAGONAL_NW = 0;
const int NEIGHBOR_HEXAGONAL_W = 2;
const int NEIGHBOR_HEXAGONAL_SW = 5;
const int NEIGHBOR_HEXAGONAL_SE = 6;

//cell
const int MASK_HEXAGONAL_CELL = 1;
const int STATE_HEXAGONAL_ON = 1;

//ant
const int IS_HEXAGONAL_ANT = 8;
const int REVERSE_HEXAGONAL_ANT = 6;
const int MASK_HEXAGONAL_ANT = 14;
const int STATE_HEXAGONAL_SHARPCLOCKWISE = 8;
const int STATE_HEXAGONAL_SLIGHTCLOCKWISE = 10;
const int STATE_HEXAGONAL_SLIGHTCOUNTER = 12;
const int STATE_HEXAGONAL_SHARPCOUNTER = 14;

//facing
const int MASK_HEXAGONAL_FACING = 112;
const int STATE_HEXAGONAL_E = 0;
const int STATE_HEXAGONAL_NE = 16;
const int STATE_HEXAGONAL_NW = 32;
const int STATE_HEXAGONAL_W = 48;
const int STATE_HEXAGONAL_SW = 64;
const int STATE_HEXAGONAL_SE = 80;

//derived class of cellular automaton for Langton's Ant on hexagonal grids
class LangtonsAntHexagonal : public CellularAutomaton
{
public:
	LangtonsAntHexagonal(HexagonalGrid* grid, bool shouldLoopHorizontally, bool shouldLoopVertically);

	int getWidth();
	int getHeight();

	void toggleGrid();
	void update(int minX, int minY, int maxX, int maxY);

private:
	HexagonalGrid* const GRID;	//grid of cells

	bool isAntComingHere(int x, int y, int state, int otherRowEastOffset);
};