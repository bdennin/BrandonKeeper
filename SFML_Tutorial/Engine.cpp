#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include "Engine.hpp"
#include "windows.h"

Engine::Engine(bool isDebugging) {

	if (isDebugging)
		this->log.open("log.txt");
}

Engine::~Engine() {
	
	delete this->camera;
	delete this->window;
	delete this->textureMap;

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

	sf::Vector2f cameraSize = sf::Vector2f(this->WINDOW_WIDTH, this->WINDOW_HEIGHT * 3 / 4);
	sf::Vector2f cameraCenter = cameraSize * .5f;
	sf::FloatRect graphicArea = sf::FloatRect(0, 0, 1, .75f);

	this->window = new sf::RenderWindow(sf::VideoMode(this->WINDOW_WIDTH, this->WINDOW_HEIGHT), "Brandon Keeper");
	this->camera = new sf::View();
	this->textureMap = new sf::Texture();
	
	this->tiles = std::vector<Tile*>();
	this->cameraScrolls = 1;
	this->focusedTile = nullptr;

	this->window->setVerticalSyncEnabled(true);	
	this->window->setFramerateLimit(120);
	this->camera->setSize(cameraSize);
	this->camera->setCenter(cameraCenter);
	this->camera->setViewport(graphicArea);
	this->textureMap->loadFromFile("globalTextureMap.png");

	this->initializeMap();
	this->lockMouse(true);
}

void Engine::start() {
	
	this->initialize();
	this->mainLoop();
}

void Engine::renderFrame() {

	this->window->clear(sf::Color::Black);
	this->window->setView(*camera);
	this->drawMap();
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

				if (event.mouseButton.button == sf::Mouse::Left)
					this->focusedTile->toggleSelected();

				sf::Vector2i windowPosition = sf::Mouse::getPosition(*this->window);
				sf::Vector2f globalPosition = this->window->mapPixelToCoords(windowPosition);

				int x = globalPosition.x;
				int y = globalPosition.y;

				int windowX = windowPosition.x;
				int windowY = windowPosition.y;

				log << "world x: " << x << " world y: " << y << " window x: " << windowX << " window y: " << windowY << "\n";
				break;
			}
			case sf::Event::MouseWheelScrolled: {
				
				sf::Vector2f cameraBounds = this->camera->getSize();
				int camWidth = cameraBounds.x;
				int camHeight = cameraBounds.y;
				
				if (event.mouseWheelScroll.delta < 0 && this->cameraScrolls > 0) {
					--(this->cameraScrolls);
					this->camera->zoom(2.0f);
					log << camWidth << " = camWidth; " << camHeight << " = camHeight;\n";
				}
				else if (event.mouseWheelScroll.delta > 0 && this->cameraScrolls < 2) {
					++(this->cameraScrolls);
					this->camera->zoom(.5f);
					log << camWidth << " = camWidth; " << camHeight << " = camHeight;\n";
				}
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

				float x = event.size.width;
				float y = event.size.height;

				this->camera->setSize(x, y);
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

	this->updateCamera();
	this->updateHover();
}

void Engine::updateCamera() {

	sf::Vector2i windowPosition = sf::Mouse::getPosition(*this->window);
	sf::Vector2f globalPosition = this->window->mapPixelToCoords(windowPosition);
	sf::Vector2f cameraCenter = this->camera->getCenter();
	sf::Vector2f cameraBounds = this->camera->getSize();

	int x = globalPosition.x;
	int y = globalPosition.y;

	int windowX = windowPosition.x;
	int windowY = windowPosition.y;

	int camX = cameraCenter.x;
	int camY = cameraCenter.y;

	int camWidth = cameraBounds.x;
	int camHeight = cameraBounds.y;

	int halfWidth = camWidth / 2;
	int halfHeight = camHeight / 2;

	//mouse is in left portion of the window
	if (windowX < this->SCROLL_ZONE_WIDTH) {

		if (camX > halfWidth + this->SCROLL_SPEED) {
			camX -= this->SCROLL_SPEED;
		}
		else {
			camX = halfWidth;
		}
	}
	//mouse is in right portion of the window
	else if (windowX > this->WINDOW_WIDTH - this->SCROLL_ZONE_WIDTH) {

		if (camX < this->HORIZONTAL_PIXELS - halfWidth - this->SCROLL_SPEED) {
			camX += this->SCROLL_SPEED;
		}
		else {
			camX = this->HORIZONTAL_PIXELS - halfWidth;
		}
	}	
	
	//mouse is in top portion of the window
	if (windowY < this->SCROLL_ZONE_WIDTH) {

		if (camY > halfHeight + this->SCROLL_SPEED) {
			camY -= this->SCROLL_SPEED;
		}
		else {
			camY = halfHeight;
		}
	}
	//mouse is in bottom portion of the window
	else if (windowY > this->WINDOW_HEIGHT - this->SCROLL_ZONE_WIDTH) {

		if (camY < this->HORIZONTAL_PIXELS - halfHeight - this->SCROLL_SPEED) {
			camY += this->SCROLL_SPEED;
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

	int x = globalPosition.x;
	int y = globalPosition.y;

	int tileX = x / this->TILE_SIZE;
	int tileY = y / this->TILE_SIZE;
	int tileIndex = MAP_SIZE * tileY + tileX;

	Tile* focusedTile = this->tiles[tileIndex];

	if (tileIndex < this->TOTAL_TITLES && tileIndex >= 0) {

		if (nullptr == this->focusedTile) {
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
	//mouse is not on a tile
	else
		if (nullptr == this->focusedTile) {

		}
		else {
			this->focusedTile->toggleHovered();
			this->focusedTile = nullptr;
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

		int type;
		int x = i % MAP_SIZE * TILE_SIZE;
		int y = i / MAP_SIZE * TILE_SIZE;

		if (x == 0 || y == 0 || x ==  HORIZONTAL_PIXELS - TILE_SIZE || y == HORIZONTAL_PIXELS - TILE_SIZE)
			type = 0;
		else
			type = 1;

		this->tiles.push_back(new Tile(textureMap, sf::Vector2f(x, y), type, TILE_SIZE));
	}
}

void Engine::drawMap() {

	for (unsigned int i = 0; i < this->TOTAL_TITLES; i++) {
		this->window->draw(tiles[i]->getSprite());
	}
}