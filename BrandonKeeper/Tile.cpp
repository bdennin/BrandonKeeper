#include "Tile.h"
#include "SoundHandler.h"

Clock Tile::textureClock;

Tile::Tile(Texture* textures, Vector2f* position, TileType tileType, int ID, int tileSize, int mapTileSize)
{
	Vector2f mapPosition;
	mapPosition.x = (*position).x / tileSize * mapTileSize;
	mapPosition.y = (*position).y / tileSize * mapTileSize;

	this->textures = textures;
	this->position = position;
	this->tileSize = tileSize;
	this->mapTileSize = mapTileSize;
	this->ID = ID;

	this->type = tileType;
	this->selectedType = SelectedType::INVISIBLE;
	this->mapSelectedType = SelectedType::MAP_INVISIBLE_A;
	this->isHovered = false;
	this->isSelected = false;
	this->isVisible = false;
	this->isReachable = false;
	this->isBeingExtracted = false;
	this->isOccupied = false;
	this->hasDropOff = false;
	this->isTreasuryType = false;

	this->sprite = new Sprite();
	this->sprite->setTexture(*this->textures);
	this->sprite->setPosition(*this->position);

	this->mapSprite = new Sprite();
	this->mapSprite->setTexture(*this->textures);
	this->mapSprite->setPosition(mapPosition);

	this->initializeTileType(tileType);
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
			this->type = TileType::ROCK;
			this->cost = 0;
			this->health = numeric_limits<int>::max();
			this->extractableGold = 0;
			this->storeableGold = 0;
			this->storedGold = 0;
			this->tileName = "Rock";

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
			this->type = TileType::EARTH;
			this->cost = 0;
			this->health = 300;
			this->extractableGold = 0;
			this->storeableGold = 0;
			this->storedGold = 0;
			this->tileName = "Earth";

			this->isSelectable = true;
			this->isNatural = true;
			this->isTraversable = false;
			this->isMineable = true;
			this->hasExtractableGold = false;

			break;
		}

		case GOLD:
		{
			this->type = TileType::GOLD;
			this->cost = 0;
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
			this->type = TileType::GEM;
			this->cost = 0;
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
			this->type = TileType::DIRT;
			this->cost = 0;
			this->health = numeric_limits<int>::max();
			this->extractableGold = 0;
			this->storeableGold = 0;
			this->storedGold = 0;
			this->tileName = "Dirt";

			this->isSelectable = false;
			this->isNatural = true;
			this->isTraversable = true;
			this->isMineable = false;
			this->hasExtractableGold = false;

			break;
		}

		case TREASURY:
		{
			this->type = TileType::TREASURY;
			this->cost = 50;
			this->health = 100;
			this->extractableGold = 0;
			this->storeableGold = Tile::TREASURY_MAX_GOLD;
			this->storedGold = 0;
			this->tileName = "Treasury";

			this->isSelectable = false;
			this->isNatural = false;
			this->isTraversable = true;
			this->isMineable = false;
			this->hasExtractableGold = false;
			this->isTreasuryType = true;

			break;
		}

		case LAIR:
		{
			this->type = TileType::LAIR;
			this->cost = 100;
			this->health = 200;
			this->extractableGold = 0;
			this->storeableGold = 0;
			this->storedGold = 0;
			this->tileName = "Lair";

			this->isSelectable = false;
			this->isNatural = false;
			this->isTraversable = true;
			this->isMineable = false;
			this->hasExtractableGold = false;

			break;
		}

		case HATCHERY:
		{
			this->type = TileType::HATCHERY;
			this->cost = 100;
			this->health = 200;
			this->extractableGold = 0;
			this->storeableGold = 0;
			this->storedGold = 0;
			this->tileName = "Hatchery";

			this->isSelectable = false;
			this->isNatural = false;
			this->isTraversable = true;
			this->isMineable = false;
			this->hasExtractableGold = false;

			break;
		}

		case TRAINING_ROOM:
		{
			this->type = TileType::TRAINING_ROOM;
			this->cost = 150;
			this->health = 300;
			this->extractableGold = 0;
			this->storeableGold = 0;
			this->storedGold = 0;
			this->tileName = "Training Room";

			this->isSelectable = false;
			this->isNatural = false;
			this->isTraversable = true;
			this->isMineable = false;
			this->hasExtractableGold = false;

			break;
		}

		case LIBRARY:
		{
			this->type = TileType::LIBRARY;
			this->cost = 200;
			this->health = 400;
			this->extractableGold = 0;
			this->storeableGold = 0;
			this->storedGold = 0;
			this->tileName = "Library";

			this->isSelectable = false;
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
	int current = Tile::textureClock.getElapsedTime().asMilliseconds();

	if (current > 600)
		Tile::textureClock.restart();

	if (current > 500)
	{
		if (this->isVisible)
		{
			if (this->isSelected)
			{
				if (this->isHovered)
					this->selectedType = SelectedType::VISIBLE_AND_HOVERED_AND_SELECTED_F;
				else
					this->selectedType = SelectedType::VISIBLE_AND_SELECTED_F;

				this->mapSelectedType = MAP_F;
			}
			else
			{
				if (this->isHovered)
				{
					this->selectedType = SelectedType::VISIBLE_AND_HOVERED_F;
				}
				else
				{
					this->selectedType = SelectedType::VISIBLE;
				}

				this->mapSelectedType = MAP_A;
			}
		}
		else
		{
			if (this->isSelected)
			{
				if (this->isHovered)
					this->selectedType = SelectedType::INVISIBLE_AND_HOVERED_AND_SELECTED_F;
				else
					this->selectedType = SelectedType::INVISIBLE_AND_SELECTED_F;

				this->mapSelectedType = MAP_INVISIBLE_F;
			}
			else
			{
				if (this->isHovered)
					this->selectedType = SelectedType::INVISIBLE_AND_HOVERED_F;
				else
					this->selectedType = SelectedType::INVISIBLE;

				this->mapSelectedType = MAP_INVISIBLE_A;
			}
		}
	}
	else if (current > 400)
	{
		if (this->isVisible)
		{
			if (this->isSelected)
			{
				if (this->isHovered)
					this->selectedType = SelectedType::VISIBLE_AND_HOVERED_AND_SELECTED_E;
				else
					this->selectedType = SelectedType::VISIBLE_AND_SELECTED_E;

				this->mapSelectedType = MAP_E;
			}
			else
			{
				if (this->isHovered)
				{
					this->selectedType = SelectedType::VISIBLE_AND_HOVERED_E;
				}
				else
				{
					this->selectedType = SelectedType::VISIBLE;
				}

				this->mapSelectedType = MAP_A;
			}
		}
		else
		{
			if (this->isSelected)
			{
				if (this->isHovered)
					this->selectedType = SelectedType::INVISIBLE_AND_HOVERED_AND_SELECTED_E;
				else
					this->selectedType = SelectedType::INVISIBLE_AND_SELECTED_E;
				
				this->mapSelectedType = MAP_INVISIBLE_E;
			}
			else
			{
				if (this->isHovered)
					this->selectedType = SelectedType::INVISIBLE_AND_HOVERED_E;
				else
					this->selectedType = SelectedType::INVISIBLE;
				
				this->mapSelectedType = MAP_INVISIBLE_A;
			}
		}
	}
	else if (current > 300)
	{
		if (this->isVisible)
		{
			if (this->isSelected)
			{
				if (this->isHovered)
					this->selectedType = SelectedType::VISIBLE_AND_HOVERED_AND_SELECTED_D;
				else
					this->selectedType = SelectedType::VISIBLE_AND_SELECTED_D;

				this->mapSelectedType = MAP_D;
			}
			else
			{
				if (this->isHovered)
				{
					this->selectedType = SelectedType::VISIBLE_AND_HOVERED_D;
				}
				else
				{
					this->selectedType = SelectedType::VISIBLE;
				}

				this->mapSelectedType = MAP_A;
			}
		}
		else
		{
			if (this->isSelected)
			{
				if (this->isHovered)
					this->selectedType = SelectedType::INVISIBLE_AND_HOVERED_AND_SELECTED_D;
				else
					this->selectedType = SelectedType::INVISIBLE_AND_SELECTED_D;

				this->mapSelectedType = MAP_INVISIBLE_D;
			}
			else
			{
				if (this->isHovered)
					this->selectedType = SelectedType::INVISIBLE_AND_HOVERED_D;
				else
					this->selectedType = SelectedType::INVISIBLE;

				this->mapSelectedType = MAP_INVISIBLE_A;
			}
		}
	}
	else if (current > 200)
	{
		if (this->isVisible)
		{
			if (this->isSelected)
			{
				if (this->isHovered)
					this->selectedType = SelectedType::VISIBLE_AND_HOVERED_AND_SELECTED_C;
				else
					this->selectedType = SelectedType::VISIBLE_AND_SELECTED_C;

				this->mapSelectedType = MAP_C;
			}
			else
			{
				if (this->isHovered)
				{
					this->selectedType = SelectedType::VISIBLE_AND_HOVERED_C;
				}
				else
				{
					this->selectedType = SelectedType::VISIBLE;
				}

				this->mapSelectedType = MAP_A;
			}
		}
		else
		{
			if (this->isSelected)
			{
				if (this->isHovered)
					this->selectedType = SelectedType::INVISIBLE_AND_HOVERED_AND_SELECTED_C;
				else
					this->selectedType = SelectedType::INVISIBLE_AND_SELECTED_C;

				this->mapSelectedType = MAP_INVISIBLE_C;
			}
			else
			{
				if (this->isHovered)
					this->selectedType = SelectedType::INVISIBLE_AND_HOVERED_C;
				else
					this->selectedType = SelectedType::INVISIBLE;

				this->mapSelectedType = MAP_INVISIBLE_A;
			}
		}
	}
	else if (current > 100)
	{
		if (this->isVisible)
		{
			if (this->isSelected)
			{
				if (this->isHovered)
					this->selectedType = SelectedType::VISIBLE_AND_HOVERED_AND_SELECTED_B;
				else
					this->selectedType = SelectedType::VISIBLE_AND_SELECTED_B;

				this->mapSelectedType = MAP_B;
			}
			else
			{
				if (this->isHovered)
				{
					this->selectedType = SelectedType::VISIBLE_AND_HOVERED_B;
				}
				else
				{
					this->selectedType = SelectedType::VISIBLE;
				}

				this->mapSelectedType = MAP_A;
			}
		}
		else
		{
			if (this->isSelected)
			{
				if (this->isHovered)
					this->selectedType = SelectedType::INVISIBLE_AND_HOVERED_AND_SELECTED_B;
				else
					this->selectedType = SelectedType::INVISIBLE_AND_SELECTED_B;

				this->mapSelectedType = MAP_INVISIBLE_B;
			}
			else
			{
				if (this->isHovered)
					this->selectedType = SelectedType::INVISIBLE_AND_HOVERED_B;
				else
					this->selectedType = SelectedType::INVISIBLE;

				this->mapSelectedType = MAP_INVISIBLE_A;
			}
		}
	}
	else
	{
		if (this->isVisible)
		{
			if (this->isSelected)
			{
				if (this->isHovered)
					this->selectedType = SelectedType::VISIBLE_AND_HOVERED_AND_SELECTED_A;
				else
					this->selectedType = SelectedType::VISIBLE_AND_SELECTED_A;

				this->mapSelectedType = MAP_A;
			}
			else
			{
				if (this->isHovered)
				{
					this->selectedType = SelectedType::VISIBLE_AND_HOVERED_A;
				}
				else
				{
					this->selectedType = SelectedType::VISIBLE;
				}

				this->mapSelectedType = MAP_A;
			}
		}
		else
		{
			if (this->isSelected)
			{
				if (this->isHovered)
					this->selectedType = SelectedType::INVISIBLE_AND_HOVERED_AND_SELECTED_A;
				else
					this->selectedType = SelectedType::INVISIBLE_AND_SELECTED_A;

				this->mapSelectedType = MAP_INVISIBLE_A;
			}
			else
			{
				if (this->isHovered)
					this->selectedType = SelectedType::INVISIBLE_AND_HOVERED_A;
				else
					this->selectedType = SelectedType::INVISIBLE;

				this->mapSelectedType = MAP_INVISIBLE_A;
			}
		}
	}

	if (this->isTreasuryType)
	{
		if (this->storedGold > Tile::TREASURY_MAX_GOLD / 2)
			this->type = TileType::TREASURY_FULL;
		else if (this->storedGold > 0)
			this->type = TileType::TREASURY_MID;
		else
			this->type = TileType::TREASURY;
	}

	this->updateSprite();
	this->updateMapSprite();
}

void Tile::updateSprite()
{
	IntRect spriteDimensions = IntRect(selectedType * tileSize, type * tileSize, tileSize, tileSize);
	this->sprite->setTextureRect(spriteDimensions);
}

void Tile::updateMapSprite()
{
	IntRect spriteDimensions = IntRect(mapSelectedType * tileSize + 1, type * tileSize + 1, mapTileSize, mapTileSize);
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
	if (this->isSelectable || !this->isVisible)
	{
		if (this->isSelected == isSelected)
		{

		}
		else
		{
			this->isSelected = isSelected;
			this->setTextureByClock();

			SoundHandler::playSound("tile_select");
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

bool Tile::getVisible()
{
	return this->isVisible;
}

void Tile::setVisible(bool isVisible)
{
	this->isVisible = isVisible;
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

bool Tile::getMineable()
{
	return this->isMineable;
}

int Tile::getHealth()
{
	return this->health;
}

void Tile::setHealth(int health)
{
	this->health = health;
}

bool Tile::getOccupied()
{
	return this->isOccupied;
}

void Tile::setOccupied(bool isOccupied)
{
	this->isOccupied = isOccupied;
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
	if (playSound)
	{
		int random = rand() % 3 + 1;
		string name = "tile_cave-in_" + to_string(random);
		SoundHandler::playCriticalSound(name);
	}

	this->isSelected = false;

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

int Tile::getStoreableGold()
{
	return Tile::TREASURY_MAX_GOLD - this->storedGold;
}

void Tile::addGold(int gold)
{
	this->storedGold += gold;
}

void Tile::removeGold(int gold)
{
	this->storedGold -= gold;
}

bool Tile::hasMaxGold()
{
	return this->storedGold >= Tile::TREASURY_MAX_GOLD;
}

bool Tile::containsGold()
{
	return this->hasExtractableGold;
}

bool Tile::getDropOff()
{
	return this->hasDropOff;
}

void Tile::setDropOff(bool hasDropOff)
{
	this->hasDropOff = hasDropOff;
}

void Tile::reinit()
{
	this->selectedType = SelectedType::INVISIBLE;
	this->mapSelectedType = SelectedType::MAP_INVISIBLE_A;

	this->isSelected = false;
	this->isVisible = false;
	this->isReachable = false;
	this->isBeingExtracted = false;
	this->isOccupied = false;
	this->hasDropOff = false;
	this->isTreasuryType = false;
}

TileType Tile::getType()
{
	return this->type;
}

void Tile::setType(TileType type)
{
	this->initializeTileType(type);
}

SelectedType Tile::getSelectedType()
{
	return this->selectedType;
}

SelectedType Tile::getMapSelectedType()
{
	return this->mapSelectedType;
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
		int gold = this->cost / 2;
		SoundHandler::playSound("tile_sell");

		this->setType(TileType::DIRT);
		this->setTextureByClock();
		return gold;
	}
}

Vector2f* Tile::getPosition()
{
	return this->position;
}

Sprite* Tile::getSprite()
{
	this->setTextureByClock();

	return this->sprite;
}

Sprite* Tile::getMapSprite()
{
	this->setTextureByClock();

	return this->mapSprite;
}