#pragma once

#include "CellularAutomaton.h"
#include "Grid.h"

//derived class of cellular automaton for outer totalistic cellular automata
class OuterTotalisticCA : public CellularAutomaton
{
public:
	OuterTotalisticCA(Grid* grid, const int rulesIfOff[13], const int rulesIfOn[13], bool isMooreNeighborhood, bool shouldLoopHorizontally, bool shouldLoopVertically);

	int getWidth();
	int getHeight();

	void setIsMooreNeighborhood(bool is);
	void setBirthRule(int index, int state);
	void setSurvivalRule(int index, int state);

	void toggleGrid();
	void update(int minX, int minY, int maxX, int maxY);

private:
	Grid* const GRID;	//grid of cells

	int rulesIfOff[13];			//mapping from outer total to next state for off cells
	int rulesIfOn[13];			//mapping from outer total to next state for on cells
	bool isMooreNeighborhood;	//boolean controlling whether cells that share only a vertex are considered neighbors
};