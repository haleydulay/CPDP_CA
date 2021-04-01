#include "LangtonsAntSquare.h"

//Langton's Ant on a square grid constructor
//initializes member variables
LangtonsAntSquare::LangtonsAntSquare(SquareGrid* grid, bool shouldLoopHorizontally, bool shouldLoopVertically) : GRID(grid), CellularAutomaton(shouldLoopHorizontally, shouldLoopVertically)
{
}

//gets width of grid in cells
int LangtonsAntSquare::getWidth()
{
	return GRID->WIDTH;
}

//gets height of grid in cells
int LangtonsAntSquare::getHeight()
{
	return GRID->HEIGHT;
}

//toggles grid
void LangtonsAntSquare::toggleGrid()
{
	GRID->toggleGrid();
}

//does one time step for a cellular automaton based on Langton's Ant
//used on square grids
//ant rotates, recolors cell, and moves forward
//ants die if they stop on the same space
//state is 5 bits: FFAAC
//C0 = cell is off, C1 = cell is on
//AA00 = no ant, A01 = no ant, AA10 = clockwise ant, AA11 = counter-clockwise ant
//FF00 = facing E, FF01 = facing N, FF10 = facing W, FF11 = facing S
void LangtonsAntSquare::update(int minX, int minY, int maxX, int maxY)
{
	for (int x = minX; x < maxX; ++x)
	{
		for (int y = minY; y < maxY; ++y)
		{
			int neighborhood[13] = { 0 };
			int state = GRID->getCellState(x, y);
			int numAnts = 0;
			int ant = state & MASK_SQUARE_ANT;
			int facing = 0;
			int cell = state & MASK_SQUARE_CELL;

			GRID->getNeighborhood(neighborhood, x, y, false, shouldLoopHorizontally, shouldLoopVertically);

			if (ant & IS_SQUARE_ANT)
			{
				cell ^= MASK_SQUARE_CELL;
			}

			if ((neighborhood[NEIGHBOR_SQUARE_E] & IS_SQUARE_ANT) && isAntComingHere(1, 0, neighborhood[NEIGHBOR_SQUARE_E]))
			{
				++numAnts;
				ant = neighborhood[NEIGHBOR_SQUARE_E] & MASK_SQUARE_ANT;
				facing = STATE_SQUARE_W;
			}

			if ((neighborhood[NEIGHBOR_SQUARE_N] & IS_SQUARE_ANT) && isAntComingHere(0, -1, neighborhood[NEIGHBOR_SQUARE_N]))
			{
				++numAnts;
				ant = neighborhood[NEIGHBOR_SQUARE_N] & MASK_SQUARE_ANT;
				facing = STATE_SQUARE_S;
			}

			if ((neighborhood[NEIGHBOR_SQUARE_W] & IS_SQUARE_ANT) && isAntComingHere(-1, 0, neighborhood[NEIGHBOR_SQUARE_W]))
			{
				++numAnts;
				ant = neighborhood[NEIGHBOR_SQUARE_W] & MASK_SQUARE_ANT;
				facing = STATE_SQUARE_E;
			}

			if ((neighborhood[NEIGHBOR_SQUARE_S] & IS_SQUARE_ANT) && isAntComingHere(0, 1, neighborhood[NEIGHBOR_SQUARE_S]))
			{
				++numAnts;
				ant = neighborhood[NEIGHBOR_SQUARE_S] & MASK_SQUARE_ANT;
				facing = STATE_SQUARE_N;
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
bool LangtonsAntSquare::isAntComingHere(int x, int y, int state)
{
	int ant = state & MASK_SQUARE_ANT;

	//ants move in the opposite direction when the cell is on
	if ((state & MASK_SQUARE_CELL) == STATE_SQUARE_ON)
	{
		ant ^= REVERSE_SQUARE_ANT;
	}

	if (ant == STATE_SQUARE_CLOCKWISE)
	{
		switch (state & MASK_SQUARE_FACING)
		{
		case STATE_SQUARE_E:
			++y;
			break;

		case STATE_SQUARE_N:
			++x;
			break;

		case STATE_SQUARE_W:
			--y;
			break;

		case STATE_SQUARE_S:
			--x;
			break;

		default:
			break;
		}
	}
	else if (ant == STATE_SQUARE_COUNTER)
	{
		switch (state & MASK_SQUARE_FACING)
		{
		case STATE_SQUARE_E:
			--y;
			break;

		case STATE_SQUARE_N:
			--x;
			break;

		case STATE_SQUARE_W:
			++y;
			break;

		case STATE_SQUARE_S:
			++x;
			break;

		default:
			break;
		}
	}

	return x == 0 && y == 0;
}