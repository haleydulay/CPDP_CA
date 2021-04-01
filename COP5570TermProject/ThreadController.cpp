#include "ThreadController.h"

bool didFinish;	//boolean to control whether main loop waits for time step to finish

//returns didFinish
bool shouldSkipOverWait()
{
	return didFinish;
}

//outer totalistic CA constructor
//initializes member variables
ThreadController::ThreadController(int numThreads, CellularAutomaton* cellularAutomaton) : CELLULAR_AUTOMATON(cellularAutomaton)
{
	control = 0;
	didFinish = false;

	shouldThreadsContinue = true;
	this->numThreads = numThreads;
	threads = new std::thread*[numThreads];

	for (int n = 0; n < numThreads; ++n)
	{
		threads[n] = new std::thread(&ThreadController::wrapper, this, (n * CELLULAR_AUTOMATON->getWidth()) / numThreads, 0, ((n + 1) * CELLULAR_AUTOMATON->getWidth()) / numThreads, CELLULAR_AUTOMATON->getHeight());
	}
}

//hexagonal grid destructor
//stops, joins, and deletes threads
ThreadController::~ThreadController()
{
	shouldThreadsContinue = false;
	activate(true);

	for (int n = 0; n < numThreads; ++n)
	{
		threads[n]->join();
		delete threads[n];
	}

	delete[] threads;
}

//sets number of threads
//stops, joins, and deletes old threads
//creates new threads
void ThreadController::setNumThreads(int numThreads)
{
	shouldThreadsContinue = false;
	activate(true);

	for (int n = 0; n < this->numThreads; ++n)
	{
		threads[n]->join();
		delete threads[n];
	}

	delete[] threads;

	shouldThreadsContinue = true;
	this->numThreads = numThreads;
	threads = new std::thread*[numThreads];

	for (int n = 0; n < numThreads; ++n)
	{
		threads[n] = new std::thread(&ThreadController::wrapper, this, 0, 0, CELLULAR_AUTOMATON->getWidth(), CELLULAR_AUTOMATON->getHeight());
	}
}

//increments control variable
//activates finish condition and resets control variable when all threads are done with this time step
void ThreadController::incrementControl()
{
	controlMutex.lock();

	if (++control >= numThreads)
	{
		finishMutex.lock();
		
		didFinish = true;

		finishMutex.unlock();

		finish.notify_all();
		control = 0;
	}

	controlMutex.unlock();
}

//tells threads to run time step and waits for time step to finish
void ThreadController::activate(bool isKill)
{
	CELLULAR_AUTOMATON->toggleGrid();
	start.notify_all();

	if (!isKill)
	{
		std::unique_lock<std::mutex> lock(finishMutex);
		finish.wait(lock, shouldSkipOverWait);
	}

	didFinish = false;
}

//waits until work should start
void ThreadController::waitForStart()
{
	std::unique_lock<std::mutex> lock(startMutex);
	start.wait(lock);
}

//wrapper function for threads to update cellular automaton
//waits for start condition before starting
//increments control variable after finishing
void ThreadController::wrapper(int minX, int minY, int maxX, int maxY)
{
	waitForStart();

	while (shouldThreadsContinue)
	{
		CELLULAR_AUTOMATON->update(minX, minY, maxX, maxY);
		incrementControl();
		waitForStart();
	}
}