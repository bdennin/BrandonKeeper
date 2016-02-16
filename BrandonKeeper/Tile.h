#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>

#include <string>
#include <iostream>

using namespace sf;
using namespace std;

enum TileType
{
	ROCK = 0,
	EARTH = 1,
	GOLD = 2,
	GEM = 3,
	DIRT = 4,
	TREASURY = 5,
	LAIR = 6,
	HATCHERY = 7,
	TRAINING_ROOM = 8,
	LIBRARY = 9,
};

enum SelectedType
{
	NATURAL_A = 0,
	NATURAL_B = 1,
	NATURAL_C = 2,
	NATURAL_D = 3,
	NATURAL_E = 4,
	NATURAL_F = 5,
	HOVERED_A = 6,
	HOVERED_B = 7,
	HOVERED_C = 8,
	HOVERED_D = 9,
	HOVERED_E = 10,
	HOVERED_F = 11,
	SELECTED_A = 12,
	SELECTED_B = 13,
	SELECTED_C = 14,
	SELECTED_D = 15,
	SELECTED_E = 16,
	SELECTED_F = 17,
	HOVERED_AND_SELECTED_A = 18,
	HOVERED_AND_SELECTED_B = 19,
	HOVERED_AND_SELECTED_C = 20,
	HOVERED_AND_SELECTED_D = 21,
	HOVERED_AND_SELECTED_E = 22,
	HOVERED_AND_SELECTED_F = 23,
	MAP_A = 24,
	MAP_B = 25,
	MAP_C = 26,
	MAP_D = 27,
	MAP_E = 28,
	MAP_F = 29,
};

class Tile
{

private:

	static const Clock TEXTURE_CLOCK;

	Texture* textureMap;
	Vector2f* position;
	Sprite* sprite;
	Sprite* mapSprite;

	TileType tileType;
	SelectedType selectedType;
	SelectedType mapSelectedType;
	string tileName;

	int tileSize;
	int mapTileSize;
	int ID;
	int health;
	int buildCost;
	int extractableGold;
	int storeableGold;
	int storedGold;

	bool isHovered;
	bool isSelected;
	bool isSelectable;
	bool isVisible;
	bool isNatural;
	bool isTraversable;
	bool isMineable;
	bool isReachable;
	bool isBeingExtracted;
	bool hasExtractableGold;

	void initializeTileType(TileType tileType);
	void setTextureByClock();
	void updateSprite();
	void updateMapSprite();

public:

	Tile(Texture* textureMap, Vector2f* position, TileType tileType, int ID, int tileSize, int mapTileSize);
	~Tile();

	bool getHovered();
	void setHovered(bool isHovered);
	void toggleHovered();
	bool getSelected();
	void setSelected(bool isSelected);
	void toggleSelected();
	bool getSelectable();
	bool getReachable();
	void setReachable(bool isReachable);
	bool getTraversable();
	int getHealth();
	void setHealth(int health);
	void setBeingExtracted(bool isBeingExtracted);
	bool getBeingExtracted();
	int getExtractableGold();
	void setExtractableGold(int gold);
	void extract(bool playSound);
	int getStoredGold();
	void setStoredGold(int gold);
	bool isFull();
	bool hasGold();
	TileType getType();
	void setType(TileType type);

	int getID();
	int sell();

	Vector2f* getPosition();
	Sprite* getSprite();
	Sprite* getMapSprite();
};

#endif