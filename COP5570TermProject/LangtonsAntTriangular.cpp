#include "LangtonsAntTriangular.h"

//Langton's Ant on a triangular grid constructor
//initializes member variables
LangtonsAntTriangular::LangtonsAntTriangular(TriangularGrid* grid, bool shouldLoopHorizontally, bool shouldLoopVertically) : GRID(grid), CellularAutomaton(shouldLoopHorizontally, shouldLoopVertically)
{
}

//gets width of grid in cells
int LangtonsAntTriangular::getWidth()
{
	return GRID->WIDTH;
}

//gets height of grid in cells
int LangtonsAntTriangular::getHeight()
{
	return GRID->HEIGHT;
}

//toggles grid
void LangtonsAntTriangular::toggleGrid()
{
	GRID->toggleGrid();
}

//does one time step for a cellular automaton based on Langton's Ant
//used on triangular grids
//ant rotates, recolors cell, and moves forward
//ants die if they stop on the same space
//state is 5 bits: FFAAC
//C0 = cell is off, C1 = cell is on
//AA00 = no ant, A01 = no ant, AA10 = clockwise ant, AA11 = counter-clockwise ant
//FF00 = facing E, FF01 = facing N or S, FF10 = facing W, FF11 = facing N or S
void LangtonsAntTriangular::update(int minX, int minY, int maxX, int maxY)
{
	for (int x = minX; x < maxX; ++x)
	{
		for (int y = minY; y < maxY; ++y)
		{
			int neighborhood[13] = { 0 };
			int state = GRID->getCellState(x, y);
			int numAnts = 0;
			int ant = state & MASK_TRIANGULAR_ANT;
			int facing = 0;
			int cell = state & MASK_TRIANGULAR_CELL;

			GRID->getNeighborhood(neighborhood, x, y, false, shouldLoopHorizontally, shouldLoopVertically);

			if (ant * IS_TRIANGULAR_ANT)
			{
				cell ^= MASK_TRIANGULAR_CELL;
			}

			if ((x + y) % 2 == 0)
			{
				if ((neighborhood[NEIGHBOR_TRIANGULAR_NE] & IS_TRIANGULAR_ANT) && isAntComingHereUp(1, 0, neighborhood[NEIGHBOR_TRIANGULAR_NE]))
				{
					++numAnts;
					ant = neighborhood[NEIGHBOR_TRIANGULAR_NE] & MASK_TRIANGULAR_ANT;
					facing = STATE_TRIANGULAR_W;
				}

				if ((neighborhood[NEIGHBOR_TRIANGULAR_NW] & IS_TRIANGULAR_ANT) && isAntComingHereUp(-1, 0, neighborhood[NEIGHBOR_TRIANGULAR_NW]))
				{
					++numAnts;
					ant = neighborhood[NEIGHBOR_TRIANGULAR_NW] & MASK_TRIANGULAR_ANT;
					facing = STATE_TRIANGULAR_E;
				}

				if ((neighborhood[NEIGHBOR_TRIANGULAR_S] & IS_TRIANGULAR_ANT) && isAntComingHereUp(0, 1, neighborhood[NEIGHBOR_TRIANGULAR_S]))
				{
					++numAnts;
					ant = neighborhood[NEIGHBOR_TRIANGULAR_S] & MASK_TRIANGULAR_ANT;
					facing = STATE_TRIANGULAR_NS;
				}
			}
			else
			{
				if ((neighborhood[NEIGHBOR_TRIANGULAR_N] & IS_TRIANGULAR_ANT) && isAntComingHereDown(0, -1, neighborhood[NEIGHBOR_TRIANGULAR_N]))
				{
					++numAnts;
					ant = neighborhood[NEIGHBOR_TRIANGULAR_N] & MASK_TRIANGULAR_ANT;
					facing = STATE_TRIANGULAR_NS;
				}

				if ((neighborhood[NEIGHBOR_TRIANGULAR_SW] & IS_TRIANGULAR_ANT) && isAntComingHereDown(-1, 0, neighborhood[NEIGHBOR_TRIANGULAR_SW]))
				{
					++numAnts;
					ant = neighborhood[NEIGHBOR_TRIANGULAR_SW] & MASK_TRIANGULAR_ANT;
					facing = STATE_TRIANGULAR_E;
				}

				if ((neighborhood[NEIGHBOR_TRIANGULAR_SE] & IS_TRIANGULAR_ANT) && isAntComingHereDown(1, 0, neighborhood[NEIGHBOR_TRIANGULAR_SE]))
				{
					++numAnts;
					ant = neighborhood[NEIGHBOR_TRIANGULAR_SE] & MASK_TRIANGULAR_ANT;
					facing = STATE_TRIANGULAR_W;
				}
			}

			if (numAnts == 1)
			{
				GRID->setCellState(facing | ant | cell, x, y);
			}
			else
			{
				GRID->setCellState(cell, x, y);
			}
		}
	}
}

//checks if ant at (x, y) with state state will be at (0, 0) on next time step
//used by up triangles
bool LangtonsAntTriangular::isAntComingHereUp(int x, int y, int state)
{
	int ant = state & MASK_TRIANGULAR_ANT;

	//ants move in the opposite direction when the cell is on
	if ((state & MASK_TRIANGULAR_CELL) == STATE_TRIANGULAR_ON)
	{
		ant ^= REVERSE_TRIANGULAR_ANT;
	}

	if (ant == STATE_TRIANGULAR_CLOCKWISE)
	{
		switch (state & MASK_TRIANGULAR_FACING)
		{
		case STATE_TRIANGULAR_E:
			++x;
			break;

		case STATE_TRIANGULAR_W:
			--y;
			break;

		case STATE_TRIANGULAR_NS:
		default:
			--x;
			break;
		}
	}
	else if (ant == STATE_TRIANGULAR_COUNTER)
	{
		switch (state & MASK_TRIANGULAR_FACING)
		{
		case STATE_TRIANGULAR_E:
			--y;
			break;

		case STATE_TRIANGULAR_W:
			--x;
			break;

		case STATE_TRIANGULAR_NS:
		default:
			++x;
			break;
		}
	}

	return x == 0 && y == 0;
}

//checks if ant at (x, y) with state state will be at (0, 0) on next time step
//used by down triangles
bool LangtonsAntTriangular::isAntComingHereDown(int x, int y, int state)
{
	int ant = state & MASK_TRIANGULAR_ANT;

	//ants move in the opposite direction when the cell is on
	if ((state & MASK_TRIANGULAR_CELL) == STATE_TRIANGULAR_ON)
	{
		ant ^= REVERSE_TRIANGULAR_ANT;
	}

	if (ant == STATE_TRIANGULAR_CLOCKWISE)
	{
		switch (state & MASK_TRIANGULAR_FACING)
		{
		case STATE_TRIANGULAR_E:
			++y;
			break;

		case STATE_TRIANGULAR_W:
			--x;
			break;

		case STATE_TRIANGULAR_NS:
		default:
			++x;
			break;
		}
	}
	else if (ant == STATE_TRIANGULAR_COUNTER)
	{
		switch (state & MASK_TRIANGULAR_FACING)
		{
		case STATE_TRIANGULAR_E:
			++x;
			break;

		case STATE_TRIANGULAR_W:
			++y;
			break;

		case STATE_TRIANGULAR_NS:
		default:
			--x;
			break;
		}
	}

	return x == 0 && y == 0;
}