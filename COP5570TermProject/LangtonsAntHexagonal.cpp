#include "LangtonsAntHexagonal.h"

//Langton's Ant on a hexagonal grid constructor
//initializes member variables
LangtonsAntHexagonal::LangtonsAntHexagonal(HexagonalGrid* grid, bool shouldLoopHorizontally, bool shouldLoopVertically) : GRID(grid), CellularAutomaton(shouldLoopHorizontally, shouldLoopVertically)
{
}

//gets width of grid in cells
int LangtonsAntHexagonal::getWidth()
{
	return GRID->WIDTH;
}

//gets height of grid in cells
int LangtonsAntHexagonal::getHeight()
{
	return GRID->HEIGHT;
}

//toggles grid
void LangtonsAntHexagonal::toggleGrid()
{
	GRID->toggleGrid();
}

//does one time step for a cellular automaton based on Langton's Ant
//used on hexagonal grids
//ant rotates, recolors cell, and moves forward
//ants die if they stop on the same space
//state is 7 bits: FFFAAAC
//C0 = cell is off, C1 = cell is on
//AA0** = no ant, A100 = sharp clockwise ant, A101 = slight clockwise ant, AA110 = slight counter-clockwise ant, AA111 = sharp counter-clockwise ant
//FF000 = facing E, FF001 = facing NE, FF010 = facing NW, FF011 = facing W, FF100 = facing SW, FF101 = facing SE, FF11* = undefined
void LangtonsAntHexagonal::update(int minX, int minY, int maxX, int maxY)
{
	for (int x = minX; x < maxX; ++x)
	{
		for (int y = minY; y < maxY; ++y)
		{
			int neighborhood[13] = { 0 };
			int state = GRID->getCellState(x, y);
			int numAnts = 0;
			int ant = state & MASK_HEXAGONAL_ANT;
			int facing = 0;
			int cell = state & MASK_HEXAGONAL_CELL;
			int otherRowEastOffset = y % 2;
			int otherRowWestOffset = otherRowEastOffset - 1;

			GRID->getNeighborhood(neighborhood, x, y, false, shouldLoopHorizontally, shouldLoopVertically);

			if (ant & IS_HEXAGONAL_ANT)
			{
				cell ^= MASK_HEXAGONAL_CELL;
			}

			if ((neighborhood[NEIGHBOR_HEXAGONAL_E] & IS_HEXAGONAL_ANT) && isAntComingHere(1, 0, neighborhood[NEIGHBOR_HEXAGONAL_E], otherRowEastOffset))
			{
				++numAnts;
				ant = neighborhood[NEIGHBOR_HEXAGONAL_E] & MASK_HEXAGONAL_ANT;
				facing = STATE_HEXAGONAL_W;
			}

			if ((neighborhood[NEIGHBOR_HEXAGONAL_NE] & IS_HEXAGONAL_ANT) && isAntComingHere(otherRowEastOffset, -1, neighborhood[NEIGHBOR_HEXAGONAL_NE], 1 - otherRowEastOffset))
			{
				++numAnts;
				ant = neighborhood[NEIGHBOR_HEXAGONAL_NE] & MASK_HEXAGONAL_ANT;
				facing = STATE_HEXAGONAL_SW;
			}

			if ((neighborhood[NEIGHBOR_HEXAGONAL_NW] & IS_HEXAGONAL_ANT) && isAntComingHere(otherRowWestOffset, -1, neighborhood[NEIGHBOR_HEXAGONAL_NW], 1 - otherRowEastOffset))
			{
				++numAnts;
				ant = neighborhood[NEIGHBOR_HEXAGONAL_NW] & MASK_HEXAGONAL_ANT;
				facing = STATE_HEXAGONAL_SE;
			}

			if ((neighborhood[NEIGHBOR_HEXAGONAL_W] & IS_HEXAGONAL_ANT) && isAntComingHere(-1, 0, neighborhood[NEIGHBOR_HEXAGONAL_W], otherRowEastOffset))
			{
				++numAnts;
				ant = neighborhood[NEIGHBOR_HEXAGONAL_W] & MASK_HEXAGONAL_ANT;
				facing = STATE_HEXAGONAL_E;
			}

			if ((neighborhood[NEIGHBOR_HEXAGONAL_SW] & IS_HEXAGONAL_ANT) && isAntComingHere(otherRowWestOffset, 1, neighborhood[NEIGHBOR_HEXAGONAL_SW], 1 - otherRowEastOffset))
			{
				++numAnts;
				ant = neighborhood[NEIGHBOR_HEXAGONAL_SW] & MASK_HEXAGONAL_ANT;
				facing = STATE_HEXAGONAL_NE;
			}

			if ((neighborhood[NEIGHBOR_HEXAGONAL_SE] & IS_HEXAGONAL_ANT) && isAntComingHere(otherRowEastOffset, 1, neighborhood[NEIGHBOR_HEXAGONAL_SE], 1 - otherRowEastOffset))
			{
				++numAnts;
				ant = neighborhood[NEIGHBOR_HEXAGONAL_SE] & MASK_HEXAGONAL_ANT;
				facing = STATE_HEXAGONAL_NW;
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
bool LangtonsAntHexagonal::isAntComingHere(int x, int y, int state, int otherRowEastOffset)
{
	int ant = state & MASK_HEXAGONAL_ANT;

	//ants move in the opposite direction when the cell is on
	if ((state & MASK_HEXAGONAL_CELL) == STATE_HEXAGONAL_ON)
	{
		ant ^= REVERSE_HEXAGONAL_ANT;
	}

	switch (ant)
	{
	case STATE_HEXAGONAL_SHARPCLOCKWISE:
		switch (state & MASK_HEXAGONAL_FACING)
		{
		case STATE_HEXAGONAL_E:
			++y;
			x += otherRowEastOffset - 1;
			break;

		case STATE_HEXAGONAL_NE:
			++y;
			x += otherRowEastOffset;
			break;

		case STATE_HEXAGONAL_NW:
			++x;
			break;

		case STATE_HEXAGONAL_W:
			--y;
			x += otherRowEastOffset;
			break;

		case STATE_HEXAGONAL_SW:
			--y;
			x += otherRowEastOffset - 1;
			break;

		case STATE_HEXAGONAL_SE:
			--x;
			break;

		default:
			break;
		}

		break;

	case STATE_HEXAGONAL_SLIGHTCLOCKWISE:
		switch (state & MASK_HEXAGONAL_FACING)
		{
		case STATE_HEXAGONAL_E:
			++y;
			x += otherRowEastOffset;
			break;

		case STATE_HEXAGONAL_NE:
			++x;
			break;

		case STATE_HEXAGONAL_NW:
			--y;
			x += otherRowEastOffset;
			break;

		case STATE_HEXAGONAL_W:
			--y;
			x += otherRowEastOffset - 1;
			break;

		case STATE_HEXAGONAL_SW:
			--x;
			break;

		case STATE_HEXAGONAL_SE:
			++y;
			x += otherRowEastOffset - 1;
			break;

		default:
			break;
		}

		break;

	case STATE_HEXAGONAL_SLIGHTCOUNTER:
		switch (state & MASK_HEXAGONAL_FACING)
		{
		case STATE_HEXAGONAL_E:
			--y;
			x += otherRowEastOffset;
			break;

		case STATE_HEXAGONAL_NE:
			--y;
			x += otherRowEastOffset - 1;
			break;

		case STATE_HEXAGONAL_NW:
			--x;
			break;

		case STATE_HEXAGONAL_W:
			++y;
			x += otherRowEastOffset - 1;
			break;

		case STATE_HEXAGONAL_SW:
			++y;
			x += otherRowEastOffset;
			break;

		case STATE_HEXAGONAL_SE:
			++x;
			break;

		default:
			break;
		}

		break;

	case STATE_HEXAGONAL_SHARPCOUNTER:
		switch (state & MASK_HEXAGONAL_FACING)
		{
		case STATE_HEXAGONAL_E:
			--y;
			x += otherRowEastOffset - 1;
			break;

		case STATE_HEXAGONAL_NE:
			--x;
			break;

		case STATE_HEXAGONAL_NW:
			++y;
			x += otherRowEastOffset - 1;
			break;

		case STATE_HEXAGONAL_W:
			++y;
			x += otherRowEastOffset;
			break;

		case STATE_HEXAGONAL_SW:
			++x;
			break;

		case STATE_HEXAGONAL_SE:
			--y;
			x += otherRowEastOffset;
			break;

		default:
			break;
		}

		break;

	default:
		break;
	}

	return x == 0 && y == 0;
}