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

	void activate(bool isKill = false);

private:
	CellularAutomaton* const CELLULAR_AUTOMATON;	//cellular automaton that threads will run

	int numStarted;					//number of threads that have started their work for this time step
	int numFinished;				//number of threads that have finished their work for this time step
	int numThreads;					//number of threads
	std::thread** threads;			//array of threads
	std::mutex numStartedMutex;		//mutex to protect numStarted
	std::mutex numFinishedMutex;	//mutex to protect numFinished
	std::mutex startMutex;			//mutex to protect start condition
	std::mutex finishMutex;			//mutex to protect finish condition
	std::condition_variable start;	//condition variable to synchronize time step starts
	std::condition_variable finish;	//condition variable to synchronize time step finishes
	bool shouldThreadsContinue;		//boolean to control when threads stop so they can be joined
	
	void waitForStart(int threadIndex);
	void waitForFinish();

	void incrementNumStarted(int threadIndex);
	void incrementNumFinished();

	void wrapper(int threadIndex, int minX, int minY, int maxX, int maxY);
};