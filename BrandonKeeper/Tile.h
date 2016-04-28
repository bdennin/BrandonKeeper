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
	TREASURY_MID = 6,
	TREASURY_FULL = 7,
	LAIR = 8,
	HATCHERY = 9,
	TRAINING_ROOM = 10,
	LIBRARY = 11,
};

enum SelectedType
{
	INVISIBLE = 0,
	INVISIBLE_AND_HOVERED_A = 1,
	INVISIBLE_AND_HOVERED_B = 2,
	INVISIBLE_AND_HOVERED_C = 3,
	INVISIBLE_AND_HOVERED_D = 4,
	INVISIBLE_AND_HOVERED_E = 5,
	INVISIBLE_AND_HOVERED_F = 6,
	INVISIBLE_AND_SELECTED_A = 7,
	INVISIBLE_AND_SELECTED_B = 8, 
	INVISIBLE_AND_SELECTED_C = 9, 
	INVISIBLE_AND_SELECTED_D = 10, 
	INVISIBLE_AND_SELECTED_E = 11, 
	INVISIBLE_AND_SELECTED_F = 12,
	INVISIBLE_AND_HOVERED_AND_SELECTED_A = 13,
	INVISIBLE_AND_HOVERED_AND_SELECTED_B = 14,
	INVISIBLE_AND_HOVERED_AND_SELECTED_C = 15,
	INVISIBLE_AND_HOVERED_AND_SELECTED_D = 16,
	INVISIBLE_AND_HOVERED_AND_SELECTED_E = 17,
	INVISIBLE_AND_HOVERED_AND_SELECTED_F = 18,
	VISIBLE = 19,
	VISIBLE_AND_HOVERED_A = 20,
	VISIBLE_AND_HOVERED_B = 21,
	VISIBLE_AND_HOVERED_C = 22,
	VISIBLE_AND_HOVERED_D = 23,
	VISIBLE_AND_HOVERED_E = 24,
	VISIBLE_AND_HOVERED_F = 25,
	VISIBLE_AND_SELECTED_A = 26,
	VISIBLE_AND_SELECTED_B = 27,
	VISIBLE_AND_SELECTED_C = 28,
	VISIBLE_AND_SELECTED_D = 29,
	VISIBLE_AND_SELECTED_E = 30,
	VISIBLE_AND_SELECTED_F = 31,
	VISIBLE_AND_HOVERED_AND_SELECTED_A = 32,
	VISIBLE_AND_HOVERED_AND_SELECTED_B = 33,
	VISIBLE_AND_HOVERED_AND_SELECTED_C = 34,
	VISIBLE_AND_HOVERED_AND_SELECTED_D = 35,
	VISIBLE_AND_HOVERED_AND_SELECTED_E = 36,
	VISIBLE_AND_HOVERED_AND_SELECTED_F = 37,
	MAP_INVISIBLE_A = 38,
	MAP_INVISIBLE_B = 39,
	MAP_INVISIBLE_C = 40,
	MAP_INVISIBLE_D = 41,
	MAP_INVISIBLE_E = 42,
	MAP_INVISIBLE_F = 43,
	MAP_A = 44,
	MAP_B = 45,
	MAP_C = 46,
	MAP_D = 47,
	MAP_E = 48,
	MAP_F = 49,
};

class Tile
{

private:

	static Clock textureClock;

	Texture* textures;
	Vector2f* position;
	Sprite* sprite;
	Sprite* mapSprite;

	TileType type;
	SelectedType selectedType;
	SelectedType mapSelectedType;
	string tileName;

	int tileSize;
	int mapTileSize;
	int ID;
	int health;
	int cost;
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
	bool isOccupied;
	bool isBeingExtracted;
	bool hasExtractableGold;
	bool hasDropOff;
	bool isTreasuryType;

	void initializeTileType(TileType tileType);
	void setTextureByClock();
	void updateSprite();
	void updateMapSprite();

public:

	static const int TREASURY_MAX_GOLD = 1200;

	Tile(Texture* textures, Vector2f* position, TileType type, int ID, int tileSize, int mapTileSize);
	~Tile();

	bool getHovered();
	void setHovered(bool isHovered);
	void toggleHovered();
	bool getSelected();
	void setSelected(bool isSelected);
	void toggleSelected();
	bool getSelectable();
	bool getVisible();
	void setVisible(bool isVisible);
	bool getReachable();
	void setReachable(bool isReachable);
	bool getTraversable();
	bool getMineable();
	int getHealth();
	void setHealth(int health);
	bool getOccupied();
	void setOccupied(bool isOccupied);
	void setBeingExtracted(bool isBeingExtracted);
	bool getBeingExtracted();
	int getExtractableGold();
	void setExtractableGold(int gold);
	void extract(bool playSound);
	int getStoredGold();
	void setStoredGold(int gold);
	int getStoreableGold();
	void addGold(int gold);
	void removeGold(int gold);
	bool hasMaxGold();
	bool containsGold();
	bool getDropOff();
	void setDropOff(bool hasDropOff);
	void reinit();

	TileType getType();
	void setType(TileType type);
	SelectedType getSelectedType();
	SelectedType getMapSelectedType();

	int getID();
	int sell();

	Vector2f* getPosition();
	Sprite* getSprite();
	Sprite* getMapSprite();
};

#endif