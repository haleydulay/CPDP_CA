#include "OuterTotalisticCA.h"

//outer totalistic CA constructor
//initializes member variables
OuterTotalisticCA::OuterTotalisticCA(Grid* grid, const int rulesIfOff[13], const int rulesIfOn[13], bool isMooreNeighborhood, bool shouldLoopHorizontally, bool shouldLoopVertically) : GRID(grid), CellularAutomaton(shouldLoopHorizontally, shouldLoopVertically)
{
	this->isMooreNeighborhood = isMooreNeighborhood;

	for (int t = 0; t < 13; ++t)
	{
		this->rulesIfOff[t] = rulesIfOff[t];
		this->rulesIfOn[t] = rulesIfOn[t];
	}
}

//gets width of grid in cells
int OuterTotalisticCA::getWidth()
{
	return GRID->WIDTH;
}

//gets height of grid in cells
int OuterTotalisticCA::getHeight()
{
	return GRID->HEIGHT;
}

//toggles grid
void OuterTotalisticCA::toggleGrid()
{
	GRID->toggleGrid();
}

//does one time step for an outer-totalistic cellular automaton in a region of the grid
void OuterTotalisticCA::update(int minX, int minY, int maxX, int maxY)
{
	for (int x = minX; x < maxX; ++x)
	{
		for (int y = minY; y < maxY; ++y)
		{
			int neighborhood[13] = {0};
			int state = GRID->getCellState(x, y);
			int total = -state;	//subtract center state from total in outer-totalistic

			GRID->getNeighborhood(neighborhood, x, y, isMooreNeighborhood, shouldLoopHorizontally, shouldLoopVertically);

			for (int neighborState : neighborhood)
			{
				total += neighborState;
			}

			if (state == 0)
			{
				GRID->setCellState(rulesIfOff[total], x, y);
			}
			else
			{
				GRID->setCellState(rulesIfOn[total], x, y);
			}
		}
	}
}