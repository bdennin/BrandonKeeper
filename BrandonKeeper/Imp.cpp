#include "Imp.h"

Imp::Imp(vector<Tile*>* tiles, map<int, Tile*>* selectedTiles, unordered_set<Tile*>* changedTiles, vector<Imp*>* workers, Grid* pathfinder, 
		 Texture* textureMap, Vector2f* position, int tileSize, int worldSize, int creatureSize, int creatureMapSize)
{
	//member pointers
	this->tiles = tiles;
	this->selectedTiles = selectedTiles;
	this->changedTiles = changedTiles;
	this->pathfinder = pathfinder;
	this->textureMap = textureMap;
	this->position = position;
	this->moveTimer = new Clock();
	this->mineTimer = new Clock();
	this->stepTimer = new Clock();
	this->jobTimer = new Clock();
	this->sprite = new Sprite();
	this->mapSprite = new Sprite();
	this->job = nullptr;

	//member structures
	this->creatureType = CreatureType::IMP;
	this->spriteType = SpriteType::NATURAL;
	this->mapSpriteType = SpriteType::MAP;
	this->status = Status::IDLE;

	//member variables
	this->tileSize = tileSize;
	this->worldSize = worldSize;
	this->creatureSize = creatureSize;
	this->creatureMapSize = creatureMapSize;
	this->typeName = "Imp";
	this->name = "Rigwarl";
	this->level = 1;
	this->baseHP = maxHP = currentHP = 75;
	this->baseXP = requiredXP = 100;
	this->currentXP = 0;
	this->baseStrength = strength = 5;
	this->baseDexterity = dexterity = 249;
	this->baseWage = wage = 0;
	this->speed = 96;
	this->defense = 5;
	this->trainingCost = 10;
	this->moveSpeed = 1;
	this->hasSound = false;

	this->currentGold = 0;
	this->maxGold = 600;
	this->goldPerSwing = 100;

	Vector2f mapPosition;
	mapPosition.x = (*position).x / creatureSize * creatureMapSize;
	mapPosition.y = (*position).y / creatureSize * creatureMapSize;

	this->sprite->setTexture(*this->textureMap);
	this->sprite->setPosition(*this->position);
	this->mapSprite->setTexture(*this->textureMap);
	this->mapSprite->setPosition(mapPosition);

	this->updateSprite();
	this->updateMapSprite();
}

Imp::~Imp()
{
	delete moveTimer;
	delete mineTimer;
	delete jobTimer;
	delete position;
	delete sprite;
	delete mapSprite;
}

void Imp::giveJob(Imp* imp)
{
	imp->setJob(this->job);

	this->status = Status::IDLE;
}

void Imp::move()
{
	if (this->hasJob() && !this->currentPath.empty())
	{
		int i = this->currentPath.top();
		Tile* target = (*this->tiles)[i];
		Vector2f* moveTarget = target->getPosition();
		Vector2f* position = this->getPosition();

		int dx = moveTarget->x - position->x;
		int dy = moveTarget->y - position->y;

		if (dx != 0)
		{
			if (dx > 0)
			{
				this->moveEast();
			}
			else if (dx < 0)
			{
				this->moveWest();
			}

			if (this->hasSound)
			{
				int milli = this->stepTimer->getElapsedTime().asMilliseconds();

				if (milli < Imp::STEP_COOLDOWN)
				{

				}
				else
				{
					int random = rand() % 4 + 1;
					string name = "imp_step_" + to_string(random);
					SoundHandler::playSound(name);

					this->stepTimer->restart();
				}
			}	
		}
		else if (dy != 0)
		{
			if (dy > 0)
			{
				this->moveNorth();
			}
			else if (dy < 0)
			{
				this->moveSouth();
			}

			if (this->hasSound)
			{
				int milli = this->stepTimer->getElapsedTime().asMilliseconds();

				if (milli < Imp::STEP_COOLDOWN)
				{

				}
				else
				{
					int random = rand() % 4 + 1;
					string name = "imp_step_" + to_string(random);
					SoundHandler::playSound(name);

					this->stepTimer->restart();
				}
			}
		}
		else
		{
			if (target == this->job->destination)
				this->job->destinationReached = true;
			else
				this->currentPath.pop();
		}
	}
}

void Imp::step(bool hasSound)
{
	if (this->hasJob())
	{
		this->hasSound = hasSound;

		switch (this->job->type)
		{
			case MINE_TARGET:
			{
				if (!job->target->getBeingExtracted())
				{
					this->cancelJob();
				}
				else if (!this->job->destinationReached)
				{
					int milli = this->moveTimer->getElapsedTime().asMilliseconds();

					if (milli < Imp::MOVE_COOLDOWN)
					{
						//do nothing
					}
					else
					{
						this->move();
						this->moveTimer->restart();
					}
				}
				else if (!this->job->targetMined)
				{
					int milli = this->mineTimer->getElapsedTime().asMilliseconds();

					if (milli < Imp::MINE_COOLDOWN)
					{
						//do nothing
					}
					else
					{
						this->mine(job->target);
						this->mineTimer->restart();
					}
				}
				else
					this->completeJob();

				break;
			}

			case UNLOAD_GOLD:
			{
				break;
			}

			case MOVE_ORDER:
			{
				break;
			}

			default:
				break;
		}
	}
}

void Imp::mine(Tile* target)
{
	bool mined = false;

	if (this->hasSound)
	{
		int random = rand() % 6 + 1;
		string name = "imp_mine_" + to_string(random);
		SoundHandler::playSound(name);
	}

	if (target->hasGold())
	{
		int tileGold = target->getExtractableGold();
		int extractedGold;
		int netGold;

		if (tileGold <= this->goldPerSwing)
			extractedGold = tileGold;
		else
			extractedGold = this->goldPerSwing;

		if (this->getGoldStorage() < extractedGold)
			extractedGold = this->getGoldStorage();

		netGold = tileGold - extractedGold;
		target->setExtractableGold(netGold);

		if (netGold <= 0)
			mined = true;

		this->currentGold += extractedGold;
	}
	else
	{
		int tileHealth = target->getHealth();
		int tileDamage;
		int netHealth;

		if (tileHealth <= this->goldPerSwing)
			tileDamage = tileHealth;
		else
			tileDamage = this->goldPerSwing;

		netHealth = tileHealth - tileDamage;
		target->setHealth(netHealth);

		if (netHealth <= 0)
			mined = true;
	}

	if (mined)
	{
		target->extract(this->hasSound);

		this->job->targetMined = true;
		this->selectedTiles->erase(target->getID());
		this->changedTiles->insert(target);

		bool playMessage = this->pathfinder->discover(target);
		
		if (playMessage)
			SoundHandler::playMessage("voice_tunneled_new_area");
	}

	if (this->hasMaxGold())
		this->job->targetMined = true;
}

int Imp::getGold()
{
	return this->currentGold;
}

int Imp::getMaxGold()
{
	return this->maxGold;
}

int Imp::getGoldStorage()
{
	return this->maxGold - this->currentGold;
}

bool Imp::hasMaxGold()
{
	return this->currentGold >= this->maxGold;
}