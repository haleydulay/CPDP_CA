#include "CellularAutomaton.h"

CellularAutomaton::CellularAutomaton(bool shouldLoopHorizontally, bool shouldLoopVertically)
{
	this->shouldLoopHorizontally = shouldLoopHorizontally;
	this->shouldLoopVertically = shouldLoopVertically;
}

void CellularAutomaton::setShouldLoopHorizontally(bool should)
{
	shouldLoopHorizontally = should;
}

void CellularAutomaton::setShouldLoopVertically(bool should)
{
	shouldLoopVertically = should;
}
