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

enum SecondaryType {

	BASE = 0,
	HOVERED = 1,
	SELECTED = 2,
	HOVERED_AND_SELECTED = 3,
};

class Tile {

private:

	sf::Texture* textureMap;
	sf::Sprite* sprite;
	
	sf::Vector2f position;
	
	unsigned char baseType;
	unsigned char secondaryType;
	unsigned char tileSize;
	bool isHovered;
	bool isSelected;

	void setTexture();

public:
	
	Tile(sf::Texture* textureMap, sf::Vector2f& position, unsigned char baseType, unsigned char tileSize);
	~Tile();
	
	bool getSelected();
	void setSelected(bool isSelected);
	void toggleHovered();
	void toggleSelected();
	
	sf::Vector2f& getPosition();
	sf::Sprite& getSprite();
};

#endif