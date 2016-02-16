#include "Tile.h"
#include "SoundHandler.h"

const Clock Tile::TEXTURE_CLOCK = Clock();

Tile::Tile(Texture* textureMap, Vector2f* position, TileType tileType, int ID, int tileSize, int mapTileSize)
{
	Vector2f mapPosition;
	mapPosition.x = (*position).x / tileSize * mapTileSize;
	mapPosition.y = (*position).y / tileSize * mapTileSize;

	this->textureMap = textureMap;
	this->position = position;
	this->tileSize = tileSize;
	this->mapTileSize = mapTileSize;
	this->ID = ID;

	this->initializeTileType(tileType);
	this->selectedType = SelectedType::NATURAL_A;
	this->mapSelectedType = SelectedType::MAP_A;
	this->isHovered = false;
	this->isSelected = false;
	this->isReachable = false;
	this->isBeingExtracted = false;

	this->sprite = new Sprite();
	this->sprite->setTexture(*this->textureMap);
	this->sprite->setPosition(*this->position);

	this->mapSprite = new Sprite();
	this->mapSprite->setTexture(*this->textureMap);
	this->mapSprite->setPosition(mapPosition);

	this->updateSprite();
	this->updateMapSprite();
}

Tile::~Tile()
{
	delete position;
	delete sprite;
	delete mapSprite;
}

void Tile::initializeTileType(TileType tileType)
{
	switch (tileType)
	{
		case ROCK:
		{
			this->tileType = TileType::ROCK;
			this->buildCost = 0;
			this->health = numeric_limits<int>::max();
			this->extractableGold = 0;
			this->storeableGold = 0;
			this->storedGold = 0;
			this->tileName = "Impenetrable Rock";

			this->isSelectable = false;
			this->isVisible = true;
			this->isNatural = true;
			this->isTraversable = false;
			this->isMineable = false;
			this->hasExtractableGold = false;

			break;
		}

		case EARTH:
		{
			this->tileType = TileType::EARTH;
			this->buildCost = 0;
			this->health = 300;
			this->extractableGold = 0;
			this->storeableGold = 0;
			this->storedGold = 0;
			this->tileName = "Earth";

			this->isSelectable = true;
			this->isVisible = true;
			this->isNatural = true;
			this->isTraversable = false;
			this->isMineable = true;
			this->hasExtractableGold = false;

			break;
		}

		case GOLD:
		{
			this->tileType = TileType::GOLD;
			this->buildCost = 0;
			this->health = 1200;
			this->extractableGold = 1200;
			this->storeableGold = 0;
			this->storedGold = 0;
			this->tileName = "Gold";

			this->isSelectable = true;
			this->isVisible = true;
			this->isNatural = true;
			this->isTraversable = false;
			this->isMineable = true;
			this->hasExtractableGold = true;

			break;
		}

		case GEM:
		{
			this->tileType = TileType::GEM;
			this->buildCost = 0;
			this->health = numeric_limits<int>::max();
			this->extractableGold = numeric_limits<int>::max();
			this->storeableGold = 0;
			this->storedGold = 0;
			this->tileName = "Gem";

			this->isSelectable = true;
			this->isVisible = true;
			this->isNatural = true;
			this->isTraversable = false;
			this->isMineable = true;
			this->hasExtractableGold = true;

			break;
		}

		case DIRT:
		{
			this->tileType = TileType::DIRT;
			this->buildCost = 0;
			this->health = numeric_limits<int>::max();
			this->extractableGold = 0;
			this->storeableGold = 0;
			this->storedGold = 0;
			this->tileName = "Dirt";

			this->isSelectable = false;
			this->isVisible = true;
			this->isNatural = true;
			this->isTraversable = true;
			this->isMineable = false;
			this->hasExtractableGold = false;

			break;
		}

		case TREASURY:
		{
			this->tileType = TileType::TREASURY;
			this->buildCost = 50;
			this->health = 100;
			this->extractableGold = 0;
			this->storeableGold = 500;
			this->storedGold = 0;
			this->tileName = "Treasury";

			this->isSelectable = false;
			this->isVisible = true;
			this->isNatural = false;
			this->isTraversable = true;
			this->isMineable = false;
			this->hasExtractableGold = false;

			break;
		}

		case LAIR:
		{
			this->tileType = TileType::LAIR;
			this->buildCost = 100;
			this->health = 200;
			this->extractableGold = 0;
			this->storeableGold = 0;
			this->storedGold = 0;
			this->tileName = "Lair";

			this->isSelectable = false;
			this->isVisible = true;
			this->isNatural = false;
			this->isTraversable = true;
			this->isMineable = false;
			this->hasExtractableGold = false;

			break;
		}

		case HATCHERY:
		{
			this->tileType = TileType::HATCHERY;
			this->buildCost = 100;
			this->health = 200;
			this->extractableGold = 0;
			this->storeableGold = 0;
			this->storedGold = 0;
			this->tileName = "Hatchery";

			this->isSelectable = false;
			this->isVisible = true;
			this->isNatural = false;
			this->isTraversable = true;
			this->isMineable = false;
			this->hasExtractableGold = false;

			break;
		}

		case TRAINING_ROOM:
		{
			this->tileType = TileType::TRAINING_ROOM;
			this->buildCost = 150;
			this->health = 300;
			this->extractableGold = 0;
			this->storeableGold = 0;
			this->storedGold = 0;
			this->tileName = "Training Room";

			this->isSelectable = false;
			this->isVisible = true;
			this->isNatural = false;
			this->isTraversable = true;
			this->isMineable = false;
			this->hasExtractableGold = false;

			break;
		}

		case LIBRARY:
		{
			this->tileType = TileType::LIBRARY;
			this->buildCost = 200;
			this->health = 400;
			this->extractableGold = 0;
			this->storeableGold = 0;
			this->storedGold = 0;
			this->tileName = "Library";

			this->isSelectable = false;
			this->isVisible = true;
			this->isNatural = false;
			this->isTraversable = true;
			this->isMineable = false;
			this->hasExtractableGold = false;

			break;
		}

		default:
		{
			break;
		}
	}
}

void Tile::setTextureByClock()
{
	int current = Tile::TEXTURE_CLOCK.getElapsedTime().asMilliseconds();
	current %= 1024;

	if (current > 850)
	{
		if (this->isSelected)
		{
			if (this->isHovered)
				this->selectedType = SelectedType::HOVERED_AND_SELECTED_F;
			else
				this->selectedType = SelectedType::SELECTED_F;

			this->mapSelectedType = MAP_F;
		}
		else
		{
			if (this->isHovered)
				this->selectedType = SelectedType::HOVERED_F;
			else
				this->selectedType = SelectedType::NATURAL_A;

			this->mapSelectedType = MAP_A;
		}
	}
	else if (current > 680)
	{
		if (this->isSelected)
		{
			if (this->isHovered)
				this->selectedType = SelectedType::HOVERED_AND_SELECTED_E;
			else
				this->selectedType = SelectedType::SELECTED_E;

			this->mapSelectedType = MAP_E;
		}
		else
		{
			if (this->isHovered)
				this->selectedType = SelectedType::HOVERED_E;
			else
				this->selectedType = SelectedType::NATURAL_A;

			this->mapSelectedType = MAP_A;
		}
	}
	else if (current > 510)
	{
		if (this->isSelected)
		{
			if (this->isHovered)
				this->selectedType = SelectedType::HOVERED_AND_SELECTED_D;
			else
				this->selectedType = SelectedType::SELECTED_D;

			this->mapSelectedType = MAP_D;
		}
		else
		{
			if (this->isHovered)
				this->selectedType = SelectedType::HOVERED_D;
			else
				this->selectedType = SelectedType::NATURAL_A;

			this->mapSelectedType = MAP_A;
		}
	}
	else if (current > 340)
	{
		if (this->isSelected)
		{
			if (this->isHovered)
				this->selectedType = SelectedType::HOVERED_AND_SELECTED_C;
			else
				this->selectedType = SelectedType::SELECTED_C;

			this->mapSelectedType = MAP_C;
		}
		else
		{
			if (this->isHovered)
				this->selectedType = SelectedType::HOVERED_C;
			else
				this->selectedType = SelectedType::NATURAL_A;

			this->mapSelectedType = MAP_A;
		}
	}
	else if (current > 170)
	{
		if (this->isSelected)
		{
			if (this->isHovered)
				this->selectedType = SelectedType::HOVERED_AND_SELECTED_B;
			else
				this->selectedType = SelectedType::SELECTED_B;

			this->mapSelectedType = MAP_B;
		}
		else
		{
			if (this->isHovered)
				this->selectedType = SelectedType::HOVERED_B;
			else
				this->selectedType = SelectedType::NATURAL_A;

			this->mapSelectedType = MAP_A;
		}
	}
	else
	{
		if (this->isSelected)
		{
			if (this->isHovered)
				this->selectedType = SelectedType::HOVERED_AND_SELECTED_A;
			else
				this->selectedType = SelectedType::SELECTED_A;
		}
		else
		{
			if (this->isHovered)
				this->selectedType = SelectedType::HOVERED_A;
			else
				this->selectedType = SelectedType::NATURAL_A;
		}
		this->mapSelectedType = MAP_A;
	}

	this->updateSprite();
	this->updateMapSprite();
}

void Tile::updateSprite()
{
	IntRect spriteDimensions = IntRect(selectedType * tileSize, tileType * tileSize, tileSize, tileSize);
	this->sprite->setTextureRect(spriteDimensions);
}

void Tile::updateMapSprite()
{
	IntRect spriteDimensions = IntRect(mapSelectedType * tileSize + 1, tileType * tileSize + 1, mapTileSize, mapTileSize);
	this->mapSprite->setTextureRect(spriteDimensions);
}

bool Tile::getHovered()
{
	return this->isHovered;
}

void Tile::setHovered(bool isHovered)
{
	if (this->isHovered == isHovered)
	{

	}
	else
	{
		this->isHovered = isHovered;
		this->setTextureByClock();
		this->updateSprite();
	}
}

void Tile::toggleHovered()
{
	this->setHovered(!this->isHovered);
}

bool Tile::getSelected()
{
	return this->isSelected;
}

void Tile::setSelected(bool isSelected)
{
	if (this->isSelectable)
	{
		if (this->isSelected == isSelected)
		{

		}
		else
		{
			this->isSelected = isSelected;
			this->setTextureByClock();

			SoundHandler::playSound("tile_select");

			this->updateSprite();
			this->updateMapSprite();
		}
	}
}

void Tile::toggleSelected()
{
	this->setSelected(!this->isSelected);
}

bool Tile::getSelectable()
{
	return this->isSelectable;
}

bool Tile::getReachable()
{
	return this->isReachable;
}

void Tile::setReachable(bool isReachable)
{
	this->isReachable = isReachable;
}

bool Tile::getTraversable()
{
	return this->isTraversable;
}

int Tile::getHealth()
{
	return this->health;
}

void Tile::setHealth(int health)
{
	this->health = health;
}

void Tile::setBeingExtracted(bool isBeingExtracted)
{
	this->isBeingExtracted = isBeingExtracted;
}

bool Tile::getBeingExtracted()
{
	return this->isBeingExtracted;
}

int Tile::getExtractableGold()
{
	return this->extractableGold;
}

void Tile::setExtractableGold(int gold)
{
	this->extractableGold = gold;
}

void Tile::extract(bool playSound)
{
	this->isSelected = false;

	if (playSound)
	{
		int random = rand() % 3 + 1;
		string name = "tile_cave-in_" + to_string(random);
		SoundHandler::playCriticalSound(name);
	}

	this->setType(TileType::DIRT);
	this->setTextureByClock();
}

int Tile::getStoredGold()
{
	return this->storedGold;
}

void Tile::setStoredGold(int gold)
{
	this->storedGold = gold;
}

bool Tile::isFull()
{
	return this->storedGold >= this->storeableGold;
}

bool Tile::hasGold()
{
	return this->hasExtractableGold;
}

TileType Tile::getType()
{
	return this->tileType;
}

void Tile::setType(TileType type)
{
	this->initializeTileType(type);
}

int Tile::getID()
{
	return this->ID;
}

int Tile::sell()
{
	if (this->isNatural)
	{
		return 0;
	}
	else
	{
		int gold = this->buildCost / 2;
		SoundHandler::playSound("tile_sell");

		this->initializeTileType(TileType::DIRT);
		this->setTextureByClock();
		return gold;
	}
}

Sprite* Tile::getSprite()
{
	if (this->isSelected || this->isHovered)
	{
		this->setTextureByClock();
	}

	return this->sprite;
}

Sprite* Tile::getMapSprite()
{
	if (this->isSelected)
	{
		this->setTextureByClock();
	}

	return this->mapSprite;
}

Vector2f* Tile::getPosition()
{
	return this->position;
}