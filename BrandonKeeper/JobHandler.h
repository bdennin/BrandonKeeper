#ifndef JOBHANDLER_HPP
#define JOBHANDLER_HPP

#include "SFML\Graphics.hpp"

#include "Grid.h"
#include "Imp.h"
#include "Job.h"
#include "windows.h"

#include <thread>
#include <list>
#include <stack>
#include <iostream>

using namespace std;
using namespace sf;

class JobHandler
{

private:

	static const int FIND_SLEEP_TIME = 10;

	list<Job*>* reachableJobs;
	Thread* pathThread;
	Clock* timer;
	
	Grid* pathfinder;
	vector<Tile*>* tiles;
	map<int, Tile*>* selectedTiles;
	vector<Imp*>* workers;

	int xDelta;
	int yDelta;
	int maxTiles;
	int findCounter;
	int printCeiling;
	bool isFinding;
	bool isDebugging;

	void findJobs();
	void addExtractionJobs();
	void assignExtractionJobs();
	void assignUnloadJobs();
	

public:

	void start();

	JobHandler(vector<Tile*>* tiles, map<int, Tile*>* selectedTiles, vector<Imp*>* workers, Grid* pathfinder, int xDelta, int yDelta, bool isDebugging);
	~JobHandler();
};

#endif