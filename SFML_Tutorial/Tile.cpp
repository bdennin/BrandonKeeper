#include <SFML/Graphics.hpp>
#include "AnimationHandler.hpp"
#include "Tile.hpp"

Tile::Tile(sf::Texture* textureMap, sf::Vector2f& position, unsigned char baseType, unsigned char tileSize) {

	this->textureMap = textureMap;
	this->tileSize = tileSize;
	
	this->baseType = baseType;
	this->secondaryType = this->BASE_TYPE_INDEX;

	this->isHovered = false;
	this->isSelected = false;

	this->sprite = new sf::Sprite();
	this->sprite->setTexture(*this->textureMap);
	this->sprite->setPosition(position);
	this->setTexture();
}

Tile::~Tile() {

	delete sprite;
	delete textureMap;
}

void Tile::setTexture() {

	//Sprite are kept in a two-dimensional array; secondaryType is the column, baseType is the row
	sf::IntRect spriteDimensions = sf::IntRect(this->secondaryType * tileSize, this->baseType * tileSize, tileSize, tileSize);
	
	this->sprite->setTextureRect(spriteDimensions);
}

void Tile::toggleHovered() {

	if (this->isHovered) {
		--this->secondaryType;
		this->isHovered = false;
	}
	else {
		++this->secondaryType;
		this->isHovered = true;
	}

	this->setTexture();
}

bool Tile::getSelected() {
	return this->isSelected;
}

void Tile::setSelected(bool isSelected) {
	
	if (isSelected) {
		this->secondaryType = this->HOVERED_SELECTED_TYPE_INDEX;
		this->isSelected = true;
	}
	else {
		this->secondaryType = this->HOVERED_TYPE_INDEX;
		this->isSelected = false;
	}

	this->setTexture();
}

void Tile::toggleSelected() {
	this->setSelected(!this->isSelected);
}

sf::Sprite& Tile::getSprite() {
	return *this->sprite;
}
