#include "ThreadController.h"

std::vector<bool> didStart;	//vector of booleans to control whether threads wait for time step to start
bool didFinish;				//boolean to control whether main loop waits for time step to finish

//outer totalistic CA constructor
//initializes member variables
ThreadController::ThreadController(int numThreads, CellularAutomaton* cellularAutomaton) : CELLULAR_AUTOMATON(cellularAutomaton)
{
	numFinished = 0;
	numStarted = 0;
	didFinish = false;

	shouldThreadsContinue = true;
	this->numThreads = numThreads;
	didStart.resize(numThreads);
	threads = new std::thread*[numThreads];

	for (int n = 0; n < numThreads; ++n)
	{
		didStart[n] = false;
		threads[n] = new std::thread(&ThreadController::wrapper, this, n, (n * CELLULAR_AUTOMATON->getWidth()) / numThreads, 0, ((n + 1) * CELLULAR_AUTOMATON->getWidth()) / numThreads, CELLULAR_AUTOMATON->getHeight());
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
	didStart.resize(numThreads);
	threads = new std::thread*[numThreads];

	for (int n = 0; n < numThreads; ++n)
	{
		didStart[n] = false;
		threads[n] = new std::thread(&ThreadController::wrapper, this, n, 0, 0, CELLULAR_AUTOMATON->getWidth(), CELLULAR_AUTOMATON->getHeight());
	}
}

//tells threads to run time step
//waits for threads to finish
void ThreadController::activate(bool isKill)
{
	if (!isKill)
	{
		CELLULAR_AUTOMATON->toggleGrid();
	}

	numStartedMutex.lock();
	startMutex.lock();
	
	for (int n = 0; n < numThreads; ++n)
	{
		didStart[n] = true;
	}

	start.notify_all();
	numStarted = 0;
	startMutex.unlock();
	numStartedMutex.unlock();

	if (!isKill)
	{
		waitForFinish();
	}

	didFinish = false;
}

//waits until work should start
void ThreadController::waitForStart(int threadIndex)
{
	std::unique_lock<std::mutex> lock(startMutex);

	start.wait(lock, [threadIndex]
	{
		return didStart[threadIndex];
	});
}

//waits until work is finished
void ThreadController::waitForFinish()
{
	std::unique_lock<std::mutex> lock(finishMutex);

	finish.wait(lock, []
	{
		return didFinish;
	});
}

//increments numStarted
//clears start condition for current thread
void ThreadController::incrementNumStarted(int threadIndex)
{
	numStartedMutex.lock();
	++numStarted;
	didStart[threadIndex] = false;
	numStartedMutex.unlock();
}

//increments numFinished
//sets finish condition and resets numFinished when all threads are done with this time step
void ThreadController::incrementNumFinished()
{
	numFinishedMutex.lock();

	if (++numFinished >= numThreads)
	{
		finishMutex.lock();
		didFinish = true;
		finish.notify_all();
		numFinished = 0;
		finishMutex.unlock();
	}

	numFinishedMutex.unlock();
}

//wrapper function for threads to update cellular automaton
//waits for start condition before starting
//increments numFinished after finishing
void ThreadController::wrapper(int threadIndex, int minX, int minY, int maxX, int maxY)
{
	waitForStart(threadIndex);

	while (shouldThreadsContinue)
	{
		incrementNumStarted(threadIndex);
		CELLULAR_AUTOMATON->update(minX, minY, maxX, maxY);
		incrementNumFinished();
		waitForStart(threadIndex);
	}
}