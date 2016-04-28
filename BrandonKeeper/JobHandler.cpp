#include "JobHandler.h"

typedef map<int, Tile*>::iterator map_iterator;

JobHandler::JobHandler(vector<Tile*>* tiles, vector<Tile*>* treasuryTiles, map<int, Tile*>* selectedTiles, vector<Imp*>* workers, Grid* pathfinder, int xDelta, int yDelta, bool isDebugging)
{
	this->reachableJobs = new unordered_set<Job*>();
	this->pathThread = new Thread(&JobHandler::findJobs, this);
	this->timer = new Clock();

	this->tiles = tiles;
	this->treasuryTiles = treasuryTiles;
	this->selectedTiles = selectedTiles;
	this->workers = workers;
	this->pathfinder = pathfinder;

	this->findCounter = 0;
	this->printCeiling = 31;
	this->xDelta = xDelta;
	this->yDelta = yDelta;
	this->isPaused = false;
	this->isDebugging = isDebugging;

	this->maxTiles = tiles->size();
}

JobHandler::~JobHandler()
{
	this->pathThread->terminate();

	delete timer;
	delete reachableJobs;
	delete pathThread;
}

void JobHandler::findJobs()
{
	while (true)
	{
		Sleep(JobHandler::FIND_SLEEP_TIME);

		if (!isPaused)
		{
			if (isDebugging)
				timer->restart();

			this->addExtractionJobs();
			this->assignExtractionJobs();
			this->assignUnloadJobs();

			if (isDebugging)
			{
				if (findCounter++ % printCeiling == 0)
				{
					Time elapsed = timer->getElapsedTime();
					cout << "JobHandler::findJobs -> " << elapsed.asMicroseconds() << " microseconds.\n";
				}
			}
		}
	}
}

void JobHandler::addExtractionJobs()
{
	for (map_iterator it = selectedTiles->begin(); it != selectedTiles->end(); it++)
	{
		if (it->second->getMineable() && !it->second->getBeingExtracted())
		{
			vector<int> neighbors;
			int ID = it->first;
			Tile* neighbor;

			neighbors.push_back(ID - xDelta);
			neighbors.push_back(ID + xDelta);
			neighbors.push_back(ID - yDelta);
			neighbors.push_back(ID + yDelta);

			for (int i : neighbors)
			{
				if (i >= 0 && i < maxTiles)
				{
					neighbor = (*tiles)[i];

					if (neighbor->getReachable() && !neighbor->getOccupied())
					{
						Job* job = new Job(neighbor, it->second, JobType::MINE_TARGET);

						it->second->setBeingExtracted(true);
						neighbor->setOccupied(true);

						this->reachableJobs->insert(job);
					}
				}
			}
		}
	}
}

void JobHandler::assignExtractionJobs()
{
	int length = this->workers->size();
	int distance = numeric_limits<int>::max();

	for (int i = 0; i < length; i++)
	{
		Imp* imp = (*this->workers)[i];
		Job* work = nullptr;

		if (imp->isIdle() && !imp->hasMaxGold() && !imp->hasJobCooldown())
		{
			Vector2f* impPosition = imp->getPosition();

			for (Job* job : *this->reachableJobs)
			{
				Vector2f* jobPosition = job->destination->getPosition();

				int x = (jobPosition->x - impPosition->x);
				int y = (jobPosition->y - impPosition->y);
				
				int difference = x * x + y * y;

				if (difference < distance)
				{
					distance = difference;
					work = job;
				}
			}

			if (work != nullptr)
			{
				imp->setJob(work);
				this->reachableJobs->erase(work);
			}
		}
	}
}

void JobHandler::assignUnloadJobs()
{
	if (this->treasuryTiles->empty())
	{
		SoundHandler::playMessage("voice_need_treasury");
	}
	else
	{
		int length = this->workers->size();
		Imp* imp;

		for (int i = 0; i < length; i++)
		{
			imp = (*workers)[i];
			Job* job = nullptr;

			if (imp->isIdle() && imp->getGold() > 0)
			{
				int maxGoldDrop = Tile::TREASURY_MAX_GOLD;
				int distance = numeric_limits<int>::max();
				Tile* dropOff = nullptr;


				for (Tile* tile : *this->treasuryTiles)
				{
					if (!tile->getDropOff())
					{
						Vector2f* impPosition = imp->getPosition();
						Vector2f* tilePosition = tile->getPosition();

						int x = (tilePosition->x - impPosition->x);
						int y = (tilePosition->y - impPosition->y);

						int difference = x * x + y * y;

						if (tile->getStoredGold() < maxGoldDrop && difference < distance)
						{
							maxGoldDrop = tile->getStoredGold();
							dropOff = tile;
							distance = difference;
						}
					}
				}

				if (dropOff == nullptr)
				{
					SoundHandler::playMessage("voice_bigger_treasury");
				}
				else
				{
					job = new Job(dropOff, nullptr, JobType::UNLOAD_GOLD);
				}
			}

			if (job != nullptr)
			{
				job->destination->setDropOff(true);
				imp->setJob(job);
			}
		}
	}
}

void JobHandler::start()
{
	this->pathThread->launch();
}

void JobHandler::pause()
{
	this->isPaused = true;
}

void JobHandler::unpause()
{
	this->isPaused = false;
}