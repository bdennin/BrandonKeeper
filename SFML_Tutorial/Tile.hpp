#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "AnimationHandler.hpp"

enum TileType {

	WHITE = 0,
	BLUE = 1,
	GREEN = 2,
	RED = 3
};

class Tile {

private:

	static const unsigned char BASE_TYPE_INDEX = 0;
	static const unsigned char HOVERED_TYPE_INDEX = 1;
	static const unsigned char SELECTED_TYPE_INDEX = 2;
	static const unsigned char HOVERED_SELECTED_TYPE_INDEX = 3;

	sf::Texture* textureMap;
	sf::Sprite* sprite;

	TileType baseType;
	unsigned char secondaryType;
	unsigned char tileSize;
	bool isHovered;
	bool isSelected;

	void setTexture();

public:
	
	Tile(sf::Texture* textureMap, sf::Vector2f& position, TileType baseType, unsigned char tileSize);
	~Tile();
	
	bool getSelected();
	void setSelected(bool isSelected);
	void toggleHovered();
	void toggleSelected();
	
	sf::Sprite& getSprite();
};

#endif