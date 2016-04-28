#ifndef CREATURE_HPP
#define CREATURE_HPP

#include "SFML\Graphics.hpp"

#include "Job.h"
#include "Tile.h"
#include "Grid.h"
#include "SoundHandler.h"

#include <string>
#include <stack>
#include <iostream>
#include <unordered_set>

using namespace std;
using namespace sf;

enum Status
{
	IDLE = 0,
	WORKING = 1,
	TRAINING = 2,
	FIGHTING = 3
};

enum CreatureType
{
	IMP = 0
};

enum SpriteType
{
	WEST = 0,
	NORTH = 1,
	EAST = 2,
	SOUTH = 3,
	MAP = 4
};

class Creature
{

protected:

	static const int JOB_COOLDOWN = 10;

	static const float LEVEL_MAP[10];
	static const float LEVEL_ONE;
	static const float LEVEL_TWO;
	static const float LEVEL_THREE;
	static const float LEVEL_FOUR;
	static const float LEVEL_FIVE;
	static const float LEVEL_SIX;
	static const float LEVEL_SEVEN;
	static const float LEVEL_EIGHT;
	static const float LEVEL_NINE;
	static const float LEVEL_TEN;

	Clock* jobTimer;
	Texture* textureMap;
	Vector2f* position;
	Job* job;
	Sprite* sprite;
	Sprite* mapSprite;
	Grid* pathfinder;
	vector<Tile*>* tiles;

	CreatureType creatureType;
	SpriteType spriteType;
	SpriteType mapSpriteType;
	Status status;

	stack<int> currentPath;
	string typeName;
	string name;
	int creatureSize;
	int creatureMapSize;
	int level;
	int currentHP;
	int maxHP;
	int baseHP;
	int currentXP;
	int requiredXP;
	int baseXP;
	int strength;
	int baseStrength;
	int dexterity;
	int baseDexterity;
	int wage;
	int baseWage;
	int defense;
	int speed;
	int trainingCost;
	int moveSpeed;
	int tileSize;
	int worldSize;
	bool hasSound;

	void levelUp();
	void moveWest();
	void moveNorth();
	void moveEast();
	void moveSouth();
	void faceWest();
	void faceNorth();
	void faceEast();
	void faceSouth();
	void updateSprite();
	void updateMapSprite();

public:

	virtual void move();
	virtual void step(bool hasSound);

	void addXP();
	void setJob(Job* job);
	void cancelJob();
	void completeJob();
	bool hasJob();
	bool hasJobCooldown();
	void setStatus(Status status);
	bool isIdle();

	Status getStatus();
	Tile* getOccupiedTile();
	Vector2f* getPosition();
	Sprite* getSprite();
	Sprite* getMapSprite();
};

#endif