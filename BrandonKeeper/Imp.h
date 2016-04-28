#ifndef IMP_HPP
#define IMP_HPP

#include "SFML\Graphics.hpp"

#include "Creature.h"

using namespace std;
using namespace sf;

class Imp : public Creature
{

private:

	static const int MINE_COOLDOWN = 750;
	static const int MOVE_COOLDOWN = 5;
	static const int RENEGOTIATE_COOLDOWN = 2000;
	static const int STEP_COOLDOWN = 225;

	Clock* moveTimer;
	Clock* mineTimer;
	Clock* renegotiateTimer;
	Clock* stepTimer;
	map<int, Tile*>* selectedTiles;
	unordered_set<Tile*>* changedTiles;
	vector<Imp*>* workers;

	int currentGold;
	int maxGold;
	int goldPerSwing;

	void renegotiate();

public:

	Imp(vector<Tile*>* tiles, map<int, Tile*>* selectedTiles, unordered_set<Tile*>* changedTiles, vector<Imp*>* workers, Grid* pathfinder,
		Texture* textureMap, Vector2f* position, int tileSize, int worldSize, int creatureSize, int creatureMapSize);
	~Imp();

	virtual void move();
	virtual void step(bool hasSound);

	void mine(Tile* target);
	int getGold();
	int getMaxGold();
	int getGoldStorage();
	bool hasMaxGold();
};

#endif