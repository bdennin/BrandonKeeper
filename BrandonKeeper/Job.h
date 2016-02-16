#ifndef JOB_HPP
#define JOB_HPP

#include "SFML\Graphics.hpp"

#include "Tile.h"

using namespace std;
using namespace sf;

enum JobType
{
	MOVE_ORDER = 0,
	UNLOAD_GOLD = 1,
	MINE_TARGET = 2
};

struct Job
{
	Tile* destination;
	Tile* target;

	JobType type;

	bool destinationReached;
	bool targetMined;
	bool goldDropped;

	Job(Tile* destination, Tile* target, JobType type)
	{
		this->destination = destination;
		this->target = target;

		this->type = type;

		this->destinationReached = false;
		this->targetMined = false;
		this->goldDropped = false;
	}

	~Job()
	{

	}
};

#endif