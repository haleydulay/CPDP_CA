#pragma once

#include <condition_variable>
#include <mutex>
#include <thread>
#include "CellularAutomaton.h"
#include "Grid.h"

//thread controller class
class ThreadController
{
public:
	ThreadController(int numThreads, CellularAutomaton* cellularAutomaton);
	~ThreadController();

	void setNumThreads(int numThreads);

	void incrementControl();

	void activate(bool isKill = false);

private:
	CellularAutomaton* const CELLULAR_AUTOMATON;	//cellular automaton that threads will run
	
	int control;					//number of threads that have finished their work for this time step
	int numThreads;					//number of threads
	std::thread** threads;			//array of threads
	std::mutex controlMutex;		//mutex to protect control variable
	std::mutex startMutex;			//mutex to protect start condition
	std::mutex finishMutex;			//mutex to protect finish condition
	std::condition_variable start;	//condition variable to synchronize time step starts
	std::condition_variable finish;	//condition variable to synchronize time step finishes
	bool shouldThreadsContinue;		//boolean to control when threads stop so they can be joined
	void waitForStart();

	void wrapper(int minX, int minY, int maxX, int maxY);
};