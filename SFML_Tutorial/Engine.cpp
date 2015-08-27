#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include "Engine.hpp"
#include "windows.h"

Engine::Engine(bool isDebugging, float windowWidth, float windowHeight) {

	if (isDebugging)
		this->log.open("log.txt");

	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
}

Engine::~Engine() {

	delete this->window;
	delete this->textureMap;
	delete this->camera;
	delete this->miniMap;
	delete this->GUI;

	for (unsigned int i = 0; i < this->TOTAL_TITLES; i++) {
		delete tiles[i];
	}
}

void Engine::mainLoop() {

	while (this->window->isOpen()) {

		processInput();
		update();
		renderFrame();
	}
}

void Engine::initialize() {

	this->window = new sf::RenderWindow(sf::VideoMode((unsigned int)this->windowWidth, (unsigned int)this->windowHeight), "Brandon Keeper");
	this->textureMap = new sf::Texture();

	this->tiles = std::vector<Tile*>();
	this->cameraScrolls = 0;
	this->focusedTile = NULL;
	this->scrollSpeed = this->SCROLL_ZONE_WIDTH;

	this->window->setVerticalSyncEnabled(true);	
	this->window->setFramerateLimit(120);
	this->textureMap->loadFromFile("globalTextureMap.png");

	this->initializeCamera();
	this->initializeMap();
}

void Engine::initializeCamera() {
	
	this->camera = new sf::View();
	this->miniMap = new sf::View();
	this->GUI = new sf::View();

	sf::Vector2f cameraSize = sf::Vector2f(this->windowWidth, this->windowHeight * .7);
	sf::Vector2f cameraCenter = cameraSize * .5f;
	
	sf::Vector2f mapSize = sf::Vector2f(this->HORIZONTAL_PIXELS, this->HORIZONTAL_PIXELS);
	sf::Vector2f mapCenter = mapSize * .5f;
	
	sf::Vector2f GUISize = sf::Vector2f(this->windowWidth * .7, this->windowHeight * .3);
	sf::Vector2f GUICenter = GUISize * .5f;
	
	sf::FloatRect cameraArea = sf::FloatRect(0, 0, 1, .70f);
	sf::FloatRect mapArea = sf::FloatRect(.025, .725, .25, .25);
	sf::FloatRect GUIArea = sf::FloatRect(0, .7f, .3f, 1);

	this->camera->setSize(cameraSize);
	this->camera->setCenter(cameraCenter);
	this->camera->setViewport(cameraArea);

	this->miniMap->setSize(mapSize);
	this->miniMap->setCenter(mapCenter);
	this->miniMap->setViewport(mapArea);

	this->GUI->setSize(GUISize);
	this->GUI->setCenter(GUICenter);
	this->GUI->setViewport(GUIArea);

	this->lockMouse(true);
}

void Engine::start() {
	
	this->initialize();
	this->mainLoop();
}

void Engine::renderFrame() {

	//this->window->clear();
	this->drawMap();
	this->drawMiniMap();
	this->window->display();
}

void Engine::processInput() {

	sf::Event event;

	while (this->window->pollEvent(event)) {

		switch (event.type) {
			
			case sf::Event::KeyPressed: {

				sf::Keyboard::Key keyPressed = event.key.code;

				if (keyPressed == sf::Keyboard::Escape) {
				
					this->lockMouse(false);
					this->window->close();
				}
				break;
			}
			case sf::Event::MouseButtonPressed: {

				if (event.mouseButton.button == sf::Mouse::Left && this->focusedTile != NULL) {
					this->focusedTile->toggleSelected();
					this->isLeftMousePressed = true;
					this->isTileSelected = this->focusedTile->getSelected();
				}
				break;
			}
			case sf::Event::MouseButtonReleased: {
				
				if (event.mouseButton.button == sf::Mouse::Left) {
					this->isLeftMousePressed = false;
					this->isTileSelected = NULL;
				}
				break;
			}
			case sf::Event::MouseWheelScrolled: {
				
				if (event.mouseWheelScroll.delta < 0 && this->cameraScrolls > 0) {
					
					--this->cameraScrolls;
					this->camera->zoom(1.25f);
				}
				else if (event.mouseWheelScroll.delta > 0 && this->cameraScrolls < 4) {
					++this->cameraScrolls;
					this->camera->zoom(.8f);
				}

				//this event forces the camera to update if areas outside the map are displayed after zooming out : "eliminates black space"
				sf::Vector2i firstPosition(0, 0);
				sf::Vector2i lastPosition(this->HORIZONTAL_PIXELS - 1, this->HORIZONTAL_PIXELS - 1);
				sf::Vector2i currentPosition = sf::Mouse::getPosition();

				sf::Mouse::setPosition(lastPosition, *this->window);
				this->updateCamera(0);
				sf::Mouse::setPosition(firstPosition, *this->window);
				this->updateCamera(0);

				sf::Mouse::setPosition(currentPosition);

				break;
			}
			case sf::Event::GainedFocus: {
				this->lockMouse(true);

				break;
			}
			case sf::Event::LostFocus: {
				this->lockMouse(false);

				break;
			}
			case sf::Event::Resized: {

				this->windowWidth = event.size.width;
				this->windowHeight = event.size.height;

				this->initializeCamera();

				break;
			}
			case sf::Event::Closed: {

				this->lockMouse(false);
				this->window->close();

				break;
			}

			default: 
				break;
		}
	}
}

void Engine::update() {

	this->updateCamera(this->scrollSpeed);
	this->updateHover();
	this->updateSelection();
}

void Engine::updateCamera(int scrollSpeed) {

	sf::Vector2i windowPosition = sf::Mouse::getPosition(*this->window);
	sf::Vector2f cameraCenter = this->camera->getCenter();
	sf::Vector2f cameraBounds = this->camera->getSize();

	int windowX = windowPosition.x;
	int windowY = windowPosition.y;

	int camX = cameraCenter.x;
	int camY = cameraCenter.y;

	float camWidth = cameraBounds.x;
	float camHeight = cameraBounds.y;

	int halfWidth = camWidth / 2 + .5f;
	int halfHeight = camHeight / 2 + .5f;

	//mouse is in left portion of the window
	if (windowX < this->SCROLL_ZONE_WIDTH) {

		if (camX > halfWidth + scrollSpeed) {
			camX -= scrollSpeed;
		}
		else {
			camX = halfWidth;
		}
	}
	//mouse is in right portion of the window
	else if (windowX > this->windowWidth - this->SCROLL_ZONE_WIDTH) {

		if (camX < this->HORIZONTAL_PIXELS - halfWidth - scrollSpeed) {
			camX += scrollSpeed;
		}
		else {
			camX = this->HORIZONTAL_PIXELS - halfWidth ;
		}
	}	
	
	//mouse is in top portion of the window
	if (windowY < this->SCROLL_ZONE_WIDTH) {

		if (camY > halfHeight + scrollSpeed) {
			camY -= scrollSpeed;
		}
		else {
			camY = halfHeight;
		}
	}
	//mouse is in bottom portion of the window
	else if (windowY > this->windowHeight - this->SCROLL_ZONE_WIDTH) {

		if (camY < this->HORIZONTAL_PIXELS - halfHeight - scrollSpeed) {
			camY += scrollSpeed;
		}
		else {
			camY = this->HORIZONTAL_PIXELS - halfHeight;
		}
	}
	this->camera->setCenter(camX, camY);
}

void Engine::updateHover() {
	
	sf::Vector2i windowPosition = sf::Mouse::getPosition(*this->window);
	sf::Vector2f globalPosition = this->window->mapPixelToCoords(windowPosition);

	int winY = windowPosition.y;

	int x = globalPosition.x;
	int y = globalPosition.y;

	int tileX = x / this->TILE_SIZE;
	int tileY = y / this->TILE_SIZE;
	int tileIndex = MAP_SIZE * tileY + tileX;

	Tile* focusedTile = this->tiles[tileIndex];

	if (winY < this->windowHeight * .7) {
		if (tileIndex < this->TOTAL_TITLES && tileIndex >= 0) {

			if (NULL == this->focusedTile) {
				this->focusedTile = focusedTile;
				this->focusedTile->toggleHovered();
			}
			else if (this->focusedTile != focusedTile) {
				this->focusedTile->toggleHovered();
				this->focusedTile = focusedTile;
				this->focusedTile->toggleHovered();
			}
			//else do nothing because focusedTile is already the focus
		}
	}
		//mouse is not on a tile
		else {
			if (NULL == this->focusedTile) {

			}
			else {
				this->focusedTile->toggleHovered();
				this->focusedTile = NULL;
			}
		}
	
}

void Engine::updateSelection() {
	if (this->isLeftMousePressed && this->focusedTile != NULL) {
		this->focusedTile->setSelected(this->isTileSelected);
	}
}

void Engine::lockMouse(bool isLocked) {
	
	if (isLocked) {

		sf::WindowHandle handle = this->window->getSystemHandle();
		RECT rectangle;

		GetWindowRect(handle, &rectangle);
		ClipCursor(&rectangle);
	}
	else {

		RECT screen;
		screen.left = 0;
		screen.top = 0;
		screen.right = GetSystemMetrics(SM_CXSCREEN);
		screen.bottom = GetSystemMetrics(SM_CYSCREEN);

		ClipCursor(&screen);
	}
}

void Engine::initializeMap() {

	for (unsigned int i = 0; i < this->TOTAL_TITLES; i++) {

		TileType type;
		int x = i % MAP_SIZE * TILE_SIZE;
		int y = i / MAP_SIZE * TILE_SIZE;

		if (x == 0 || y == 0 || x ==  HORIZONTAL_PIXELS - TILE_SIZE || y == HORIZONTAL_PIXELS - TILE_SIZE)
			type = TileType::WHITE;
		else
			type = TileType::BLUE;

		this->tiles.push_back(new Tile(textureMap, sf::Vector2f(x, y), type, TILE_SIZE));
	}

	this->window->setView(*miniMap);

	for (unsigned int i = 0; i < this->TOTAL_TITLES; i++) {
		this->window->draw(tiles[i]->getSprite());
	}

	this->window->setView(*camera);
}

void Engine::drawMap() {

	this->window->setView(*camera);

	sf::Vector2i firstPosition(0, 0);
	sf::Vector2i currentPosition = sf::Mouse::getPosition();
	
	sf::Mouse::setPosition(firstPosition, *this->window);
	
	sf::Vector2i windowPosition = sf::Mouse::getPosition(*this->window);
	sf::Vector2f globalPosition = this->window->mapPixelToCoords(windowPosition);
	sf::Vector2f cameraBounds = this->camera->getSize();

	int x = globalPosition.x;
	int y = globalPosition.y;

	int maxX = x + cameraBounds.x + this->TILE_SIZE;
	int maxY = y + cameraBounds.y + this->TILE_SIZE;

	for (int i = y; i <= maxY; i += this->TILE_SIZE) {
		for (int j = x; j <= maxX; j += this->TILE_SIZE) {
			
			int tileX = j / this->TILE_SIZE;
			int tileY = i / this->TILE_SIZE;
			int tileIndex = MAP_SIZE * tileY + tileX;

			if (tileX >= this->MAP_SIZE || tileY >= this->MAP_SIZE) {

			}
			else {
				this->window->draw(tiles[tileIndex]->getSprite());
			}
		}
	}

	sf::Mouse::setPosition(currentPosition);
}

void Engine::drawMiniMap() {

//	if (this->focusedTile != NULL) {
//		this->window->setView(*miniMap);
//		this->window->draw(focusedTile->getSprite());
//		this->window->setView(*camera);
//	}
	this->window->setView(*miniMap);

	for (unsigned int i = 0; i < this->TOTAL_TITLES; i++) {
		this->window->draw(tiles[i]->getSprite());
	}

	this->window->setView(*camera);
}