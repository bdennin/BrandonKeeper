#ifndef GRID_HPP
#define GRID_HPP

#include "Tile.h"

#include <functional>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

using namespace std;
using namespace sf;

struct Node
{
	bool isAdded;
	bool isReachable;
	int tileID;
	Node* previous;
	Vector2f position;
	vector<Node*>* adjacency;

	Node(int tileID, Vector2f position)
	{
		this->tileID = tileID;
		this->position = position;
		this->adjacency = new vector<Node*>();
		this->isAdded = false;
		this->isReachable = false;
	}

	~Node()
	{
		delete adjacency;
	}
};

typedef pair<Node*, int> pqItem;

class Grid
{

private:

	static Clock* Grid::timer;

	priority_queue<pqItem, vector<pqItem>, greater<pqItem>>* pathfinder;
	vector<Tile*>* tiles;
	vector<Tile*>* treasuryTiles;
	unordered_set<Tile*>* changedTiles;

	map<int, Node*> grid;

	int xDelta;
	int yDelta;
	int maxY;
	bool isDebugging;

	void clearSolution();
	void resolveAdjacency(Node* node);
	int estimateCost(Node* current, Node* end);
	int getNorthID(int nodeID);
	int getSouthID(int nodeID);
	int getWestID(int nodeID);
	int getEastID(int nodeID);
	Node* getNorth(Node* node);
	Node* getSouth(Node* node);
	Node* getWest(Node* node);
	Node* getEast(Node* node);

public:

	Grid(vector<Tile*>* tiles, vector<Tile*>* treasuryTiles, unordered_set<Tile*>* changedTiles, int xDelta, int yDelta, bool isDebugging);
	~Grid();

	void clear();
	void printNodes();
	void addNode(Tile* tile);
	void initialize(Tile* seed);
	bool discover(Tile* seed);
	stack<int> solve(Tile* start, Tile* end);
};

#endif