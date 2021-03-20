#include <SFML/Graphics.hpp>
#include "SquareGrid.h"

//TODO: give this function its own class?
//TODO: parallelize this (that's the whole point of the assignment :P)
//does one time step for an outer-totalistic cellular automaton using the Moore neighborhood
//on a square grid, the Moore neighborhood of a cell (square) is the cell and the 8 cells around it
//in a totalistic CA, the next state of a cell depends solely on the total of the states of the cells in the cell's neighborhood
//for this kind of CA, cell states are typically limited to just 0 and 1, so the total is just the number of cells with state 1
//in an outer-totalistic CA, the center cell's state is not counted in the total, leaving just the outer cells' states
//in an outer-totalistic CA, the center cell's state is instead used to decide which ruleset to use to set the cell's next state
//if the possible states are just 0 and 1, then there are 2 rulesets to choose from: what to do if the cell is off (state 0) and what to do if it's on (state 1)
//the particular rules used here are the rules for Conway's Game of Life, which can be abbreviated B3/S23
//B3/S23 means a cell is born (goes from state 0 to state 1) if the outer total is 3 and a cell survives (goes from state 1 to state 1) if the outer total is 2 or 3
//under B3/S23, a cell will enter or stay in state 0 in any other situation
void updateMooreOuterTotalisticCellularAutomaton(Grid* grid, const int rulesIfOff[13], const int rulesIfOn[13], bool shouldLoopHorizontally, bool shouldLoopVertically)
{
	grid->toggleGrid();

	for (int x = 0; x < grid->WIDTH; ++x)
	{
		for (int y = 0; y < grid->HEIGHT; ++y)
		{
			int neighborhood[13] = {0};
			int state = grid->getCellState(x, y);
			int total = -state;

			grid->getMooreNeighborhood(neighborhood, x, y, shouldLoopHorizontally, shouldLoopVertically);

			for (int neighborState : neighborhood)
			{
				total += neighborState;
			}

			if (state == 0)
			{
				grid->setCellState(rulesIfOff[total], x, y);
			}
			else
			{
				grid->setCellState(rulesIfOn[total], x, y);
			}
		}
	}
}

//main function
//sets program up and handles events and drawing
int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "COP5570 Term Project");
	sf::Color colors[2] = {sf::Color(0, 0, 0), sf::Color(255, 255, 255)};
	SquareGrid* grid = new SquareGrid(&window, 64, 64, colors);
	int rulesIfOff[13] = {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};	//rules for birth in Conway's Game of Life (a cell is born if it has 3 live neighbors)
	int rulesIfOn[13] = {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};	//rules for survival in Conway's Game of Life (a cell survives if it has 2 or 3 live neighbors)
	bool shouldLoopHorizontally = true;
	bool shouldLoopVertically = true;

	//initialize the grid with a SE-bound glider centered at (11, 1)
	grid->setCellState(1, 11, 0);
	grid->setCellState(1, 12, 1);
	grid->setCellState(1, 10, 2);
	grid->setCellState(1, 11, 2);
	grid->setCellState(1, 12, 2);

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		updateMooreOuterTotalisticCellularAutomaton((Grid*)grid, rulesIfOff, rulesIfOn, shouldLoopHorizontally, shouldLoopVertically);

		window.clear();
		grid->draw();
		window.display();
	}

	delete grid;
	return 0;
}