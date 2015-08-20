#include <SFML/Graphics.hpp>
#include "AnimationHandler.hpp"
#include "Tile.hpp"

Tile::Tile(sf::Texture* textureMap, sf::Vector2f& position, unsigned char type, unsigned char tileSize) {

	this->textureMap = textureMap;
	this->tileSize = tileSize;
	this->type = type;
	this->previousType = type;
	this->isHovered = false;
	this->isSelected = false;

	this->sprite = new sf::Sprite();
	this->sprite->setTexture(*this->textureMap);
	this->sprite->setPosition(position);
	this->setTexture(this->type);
}

Tile::~Tile() {

	delete sprite;
	delete textureMap;
}

void Tile::setTexture(unsigned char type) {

	sf::IntRect spriteImage = sf::IntRect(0, type * tileSize, tileSize, tileSize);
	
	this->sprite->setTextureRect(spriteImage);
	this->type = type;
}

void Tile::toggleHovered() {

	if (this->isHovered) {
		this->type -= 5;
		this->isHovered = false;
	}
	else {
		this->type += 5;
		this->isHovered = true;
	}

	this->setTexture(this->type);
}

void Tile::toggleSelected() {

	if (this->isSelected) {
		this->setTexture(this->previousType);
		this->isSelected = false;
	}
	else {
		this->previousType = this->type;
		this->setTexture(4);
		this->isSelected = true;
	}
}

sf::Sprite& Tile::getSprite() {
	return *this->sprite;
}
