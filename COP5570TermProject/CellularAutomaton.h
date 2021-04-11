#pragma once

//cellular automaton class
//base class for cellular automata
//virtual methods for getting grid dimensions and doing a time step
class CellularAutomaton
{
public:
	CellularAutomaton(bool shouldLoopHorizontally, bool shouldLoopVertically);

	virtual int getWidth() = 0;
	virtual int getHeight() = 0;

	virtual void toggleGrid() = 0;
	virtual void update(int minX, int minY, int maxX, int maxY) = 0;

	void setShouldLoopHorizontally(bool should);
	void setShouldLoopVertically(bool should);

protected:
	bool shouldLoopHorizontally;	//boolean to control if left and right edges connect
	bool shouldLoopVertically;		//boolean to control if top and bottom edges connect
};