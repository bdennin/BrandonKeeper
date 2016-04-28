#ifndef JOBHANDLER_HPP
#define JOBHANDLER_HPP

#include "SFML\Graphics.hpp"

#include "Grid.h"
#include "Imp.h"
#include "Job.h"
#include "windows.h"

#include <thread>
#include <stack>
#include <iostream>

using namespace std;
using namespace sf;

class JobHandler
{

private:

	static const int FIND_SLEEP_TIME = 50;

	unordered_set<Job*>* reachableJobs;
	Thread* pathThread;
	Clock* timer;
	
	Grid* pathfinder;
	vector<Tile*>* tiles;
	vector<Tile*>* treasuryTiles;
	map<int, Tile*>* selectedTiles;
	vector<Imp*>* workers;

	int xDelta;
	int yDelta;
	int maxTiles;
	int findCounter;
	int printCeiling;
	bool isPaused;
	bool isDebugging;

	void findJobs();
	void addExtractionJobs();
	void assignExtractionJobs();
	void assignUnloadJobs();

public:

	void start();
	void pause();
	void unpause();

	JobHandler(vector<Tile*>* tiles, vector<Tile*>* treasuryTiles, map<int, Tile*>* selectedTiles, vector<Imp*>* workers, Grid* pathfinder, int xDelta, int yDelta, bool isDebugging);
	~JobHandler();
};

#endif