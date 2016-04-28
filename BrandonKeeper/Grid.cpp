#include "Grid.h"

typedef map<int, Node*>::iterator node_iterator;

Clock* Grid::timer = new Clock();

Grid::Grid(vector<Tile*>* tiles, vector<Tile*>* treasuryTiles, unordered_set<Tile*>* changedTiles, int xDelta, int yDelta, bool isDebugging)
{
	this->tiles = tiles;
	this->treasuryTiles = treasuryTiles;
	this->changedTiles = changedTiles;
	this->grid = map<int, Node*>();
	this->pathfinder = new 	priority_queue<pqItem, vector<pqItem>, greater<pqItem>>();

	this->xDelta = xDelta;
	this->yDelta = yDelta;
	this->maxY = yDelta * yDelta;
	this->isDebugging = isDebugging;
}

Grid::~Grid()
{
	delete pathfinder;

	for (node_iterator it = grid.begin(); it != grid.end(); it++)
	{
		if (it->second != nullptr)
		{
			delete it->second;
		}
	}
}

void Grid::clear()
{
	this->grid.clear();
}

void Grid::clearSolution()
{
	priority_queue<pqItem, vector<pqItem>, greater<pqItem>> emptyQueue;

	swap(*this->pathfinder, emptyQueue);
}

void Grid::resolveAdjacency(Node* node)
{
	Node* current = this->getNorth(node);

	if (current != nullptr && current->isReachable)
	{
		node->adjacency->push_back(current);
		current->adjacency->push_back(node);
	}

	current = this->getSouth(node);

	if (current != nullptr && current->isReachable)
	{
		node->adjacency->push_back(current);
		current->adjacency->push_back(node);
	}

	current = this->getWest(node);

	if (current != nullptr && current->isReachable)
	{
		node->adjacency->push_back(current);
		current->adjacency->push_back(node);
	}

	current = this->getEast(node);

	if (current != nullptr && current->isReachable)
	{
		node->adjacency->push_back(current);
		current->adjacency->push_back(node);
	}
}

int Grid::estimateCost(Node* current, Node* end)
{
	int a, b, x, y;

	a = current->position.x;
	b = current->position.y;
	x = end->position.x;
	y = end->position.y;

	return abs(a - x) + abs(b - y);
}

int Grid::getNorthID(int nodeID)
{
	return nodeID - this->yDelta;
}

int Grid::getSouthID(int nodeID)
{
	return nodeID + this->yDelta;
}

int Grid::getWestID(int nodeID)
{
	return nodeID - this->xDelta;
}

int Grid::getEastID(int nodeID)
{
	return nodeID + this->xDelta;
}

Node* Grid::getNorth(Node* node)
{
	int northNodeID = node->tileID - this->yDelta;
	return this->grid[northNodeID];
}

Node* Grid::getSouth(Node* node)
{
	int southNodeID = node->tileID + this->yDelta;
	return this->grid[southNodeID];
}

Node* Grid::getWest(Node* node)
{
	int westNodeID = node->tileID - this->xDelta;
	return this->grid[westNodeID];
}

Node* Grid::getEast(Node* node)
{
	int eastNodeID = node->tileID + this->xDelta;
	return this->grid[eastNodeID];
}

void Grid::printNodes()
{
	for (node_iterator it = grid.begin(); it != grid.end(); it++)
	{
		if (it->second == nullptr)
			continue;

		cout << "Node ID: " << it->first << " Adjacency: ";

		vector<Node*>* adjacency = it->second->adjacency;
		int length = adjacency->size();

		for (int i = 0; i < length; i++)
		{
			cout << (*adjacency)[i]->tileID << " ";
		}

		cout << "\n";
	}
}

void Grid::addNode(Tile* tile)
{
	int tileID = tile->getID();
	Node* node = this->grid[tileID];

	if (!node->isAdded)
	{
		node->isAdded = true;
		node->isReachable = true;

		this->resolveAdjacency(node);

		this->changedTiles->insert(tile);

		tile->setSelected(false);
		tile->setVisible(true);
		tile->setReachable(true);

		if (tile->getType() == TileType::TREASURY)
		{
			this->treasuryTiles->push_back(tile);
		}
	}
}

void Grid::initialize(Tile* seed)
{
	int length = this->tiles->size();

	for (int i = 0; i < length; i++)
	{
		Tile* tile = (*this->tiles)[i];
		Vector2f position = *(tile->getPosition());
		int tileID = tile->getID();

		Node* node = new Node(tileID, position);
		this->grid[i] = node;
	}

	this->discover(seed);
}

bool Grid::discover(Tile* seed)
{
	bool outcome = false;
	queue<Node*> nodesToBeAdded;

	Node* node = grid[seed->getID()];
	nodesToBeAdded.push(node);

	while (!nodesToBeAdded.empty())
	{
		node = nodesToBeAdded.front();
		nodesToBeAdded.pop();

		if (node != nullptr)
		{
			int nodeID = node->tileID;

			Tile* tile = (*this->tiles)[nodeID];

			if (tile->getTraversable() && !node->isAdded)
			{
				this->addNode(tile);
				
				if (tile->getID() != seed->getID())
					outcome = true;

				Node* current;

				current = this->getNorth(node);
				nodesToBeAdded.push(current);

				current = this->getSouth(node);
				nodesToBeAdded.push(current);

				current = this->getWest(node);
				nodesToBeAdded.push(current);

				current = this->getEast(node);
				nodesToBeAdded.push(current);
			}
			else
			{
				this->changedTiles->insert(tile);

				tile->setVisible(true);
			}
		}
	}

	return outcome;
}

stack<int> Grid::solve(Tile* start, Tile* end)
{
	if (this->isDebugging)
		timer->restart();

	this->clearSolution();

	int startID = start->getID();
	int endID = end->getID();
	Node* begin = this->grid[startID];
	Node* finish = this->grid[endID];
	stack<int> path = stack<int>();

	//A* algorithm
	if (begin->isReachable && finish->isReachable)
	{
		Node* current = begin;
		bool found = false;

		unordered_map<Node*, int> visited;

		visited[current] = 0;

		this->pathfinder->emplace(current, 0);

		while (!this->pathfinder->empty() && !found)
		{
			current = this->pathfinder->top().first;
			this->pathfinder->pop();

			if (current == finish)
			{
				found = true;
				break;
			}

			vector<Node*> adjacency = *current->adjacency;

			for (Node* neighbor : adjacency)
			{
				int totalCost = visited[current] + 1;

				if (!visited.count(neighbor) || totalCost < visited[neighbor])
				{
					visited[neighbor] = totalCost;
					int estimate = totalCost + this->estimateCost(neighbor, finish);
					neighbor->previous = current;
					this->pathfinder->emplace(neighbor, estimate);
				}
			}
		}

		while (current != begin)
		{
			path.push(current->tileID);
			current = current->previous;
		}

		path.push(current->tileID);
	}

	if (this->isDebugging)
		cout << "Grid::solve -> " << timer->getElapsedTime().asMicroseconds() << " microseconds.\n";

	return path;
}