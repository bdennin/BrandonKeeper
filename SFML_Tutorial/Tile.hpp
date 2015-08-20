#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include "AnimationHandler.hpp"

class Tile {

private:

	sf::Texture* textureMap;
	sf::Sprite* sprite;

	unsigned char type;
	unsigned char previousType;
	unsigned char tileSize;
	bool isHovered;
	bool isSelected;

	void setTexture(unsigned char type);

public:
	
	Tile(sf::Texture* textureMap, sf::Vector2f& position, unsigned char type, unsigned char tileSize);
	~Tile();
	
	void toggleHovered();
	void toggleSelected();
	
	sf::Sprite& getSprite();
};

#endif