/*
https://natureofcode.com/book/chapter-7-cellular-automata/

The following program implements a one dimentional CA based off of the
findings in the above website. Need to implement this to be multi-
dimentional and parallelized, but this is an idea of its functionality
*/

#include <iostream>
#include <string>
using namespace std;

class CA{
    int * cells;    //array of cells for generation
    int ruleset[8];    //array of rulesets to process for the cells
    int generation;     //keep track of how many generations there are
    int length;
    CA()            //default constructor
    {
        cells = new int[100];        //need to figure out how many cells are acctually needed
	    //cells = {0};	//initialize to an empty set for now
        ruleset = {0,1,1,1,1,0,1,1};	//rule 22, in reverse order 
        generation = 0;
        length = sizeof(cells)/sizeof(cells[0]);

	    for(int i = 0; i < length; i++)	//prepopulating the cells to hold 0
	    {
	        cells[i] = 0;
	    }
	    cells[length/2] = 1;	//all cells start with state 0, except the center cell has state 1
    }
	void create()
	{
	    int * nextgen = new int[length];	//compute the next generation
	    for(int i = 1; i < length-1; i++) //range is to avoid conflicts with the first and last cells and its neighbors
	    {
            int left = cells[i-1];
            int middle = cells[i];
            int right = cells[i+1];
            nextgen[i] = rules(left, middle, right);
	    }
        cells = nextgen;
        generation++;
	}


    int rules(int x, int y, int z)      //this function converts the neighborhood configuration into a regular integer to use that value as the index into the ruleset array
    {                                   //looks up new state from ruleset array
        string s = "" + x + y + z;      //join 3 bits into a string
        int index = Integer.parseInt(s,2);  //'2' indicates that we intend to parse a binary number (base 2)
        return ruleset[index];
    }
/*    void fill()         //function to output the visual CA... need to modify to work with c++
    {
        for(int i = 0; i < length; i++)
        {
            if(cells[i] == 1)
            {
                fill(0);    //black
            }
            else
            {
                fill(255);  //white
            }
//            rect(i*w, generation*w, w, w);      //set the y-location according to the generation 
        }
    }*/
};
