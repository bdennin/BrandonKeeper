#include "Creature.h"
//unimplemented - this class is for inheritance

const float Creature::LEVEL_ONE = 1.31f;
const float Creature::LEVEL_TWO = 1.62f;
const float Creature::LEVEL_THREE = 1.93f;
const float Creature::LEVEL_FOUR = 2.24f;
const float Creature::LEVEL_FIVE = 2.55f;
const float Creature::LEVEL_SIX = 2.86f;
const float Creature::LEVEL_SEVEN = 3.17f;
const float Creature::LEVEL_EIGHT = 3.48f;
const float Creature::LEVEL_NINE = 3.79f;
const float Creature::LEVEL_TEN = 4.1f;

const float Creature::LEVEL_MAP[10] = {Creature::LEVEL_ONE, Creature::LEVEL_TWO, Creature::LEVEL_THREE, Creature::LEVEL_FOUR, Creature::LEVEL_FIVE,
Creature::LEVEL_SIX, Creature::LEVEL_SEVEN, Creature::LEVEL_EIGHT, Creature::LEVEL_NINE, Creature::LEVEL_TEN};

void Creature::levelUp()
{
	this->level++;
	float modifier = Creature::LEVEL_MAP[level];
	this->maxHP = currentHP = baseHP * modifier + .5f;
	this->strength = baseStrength * modifier + .5f;
	this->dexterity = baseDexterity * modifier + .5f;
	this->wage = baseWage * modifier + .5f;
	this->requiredXP = this->baseXP * modifier + .5f;
	this->currentXP = 0;
}

void Creature::moveWest()
{
	this->position->x -= this->moveSpeed;
	this->sprite->setPosition(*this->position);

	Vector2f mapPosition;
	mapPosition.x = (*position).x / creatureSize * creatureMapSize;
	mapPosition.y = (*position).y / creatureSize * creatureMapSize;
	this->mapSprite->setPosition(mapPosition);
}

void Creature::moveNorth()
{
	this->position->y += this->moveSpeed;
	this->sprite->setPosition(*this->position);

	Vector2f mapPosition;
	mapPosition.x = (*position).x / creatureSize * creatureMapSize;
	mapPosition.y = (*position).y / creatureSize * creatureMapSize;
	this->mapSprite->setPosition(mapPosition);
}

void Creature::moveEast()
{
	this->position->x += this->moveSpeed;
	this->sprite->setPosition(*this->position);

	Vector2f mapPosition;
	mapPosition.x = (*position).x / creatureSize * creatureMapSize;
	mapPosition.y = (*position).y / creatureSize * creatureMapSize;
	this->mapSprite->setPosition(mapPosition);
}

void Creature::moveSouth()
{
	this->position->y -= this->moveSpeed;
	this->sprite->setPosition(*this->position);

	Vector2f mapPosition;
	mapPosition.x = (*position).x / creatureSize * creatureMapSize;
	mapPosition.y = (*position).y / creatureSize * creatureMapSize;
	this->mapSprite->setPosition(mapPosition);
}

void Creature::updateSprite()
{
	IntRect spriteDimensions = IntRect(this->spriteType * creatureSize, this->creatureType * creatureSize, creatureSize, creatureSize);
	this->sprite->setTextureRect(spriteDimensions);
}

void Creature::updateMapSprite()
{
	IntRect spriteDimensions = IntRect(this->mapSpriteType * creatureSize + 1, this->creatureType * creatureSize + 1, creatureMapSize, creatureMapSize);
	this->mapSprite->setTextureRect(spriteDimensions);
}

void Creature::move()
{
	//must be implemented in child.
}

void Creature::step(bool hasSound)
{
	//must be implemented in child.
}

void Creature::setJob(Job* job)
{
	this->job = job;

	Tile* start = this->getOccupiedTile();
	Tile* finish = this->job->destination;

	this->currentPath = this->pathfinder->solve(start, finish);

	if (currentPath.empty())
	{
		this->cancelJob();
	}
	else
	{
		this->status = Status::WORKING;
	}

	this->jobTimer->restart();
}

void Creature::cancelJob()
{
	this->status = Status::IDLE;

	switch (this->job->type)
	{
		case JobType::MINE_TARGET:
		{
			this->job->target->setBeingExtracted(false);
			break;
		}

		case JobType::UNLOAD_GOLD:
		{
			break;
		}

		case JobType::MOVE_ORDER:
		{
			break;
		}

		default:
			break;
	}

	delete job;
}

void Creature::completeJob()
{
	this->cancelJob();
}

bool Creature::hasJob()
{
	return this->status != Status::IDLE;
}

bool Creature::hasJobCooldown()
{
	int milli = this->jobTimer->getElapsedTime().asMilliseconds();

	return milli < Creature::JOB_COOLDOWN;
}

Status Creature::getStatus()
{
	return this->status;
}

void Creature::setStatus(Status status)
{
	this->status = status;
}

bool Creature::isIdle()
{
	return this->status == Status::IDLE;
}

Vector2f* Creature::getPosition()
{
	return this->position;
}

Tile* Creature::getOccupiedTile()
{
	int x = this->position->x;
	int y = this->position->y;

	int index = x / this->tileSize + (y / this->tileSize * this->worldSize);

	return (*this->tiles)[index];
}

Sprite* Creature::getSprite()
{
	return this->sprite;
}

Sprite* Creature::getMapSprite()
{
	return this->mapSprite;
}