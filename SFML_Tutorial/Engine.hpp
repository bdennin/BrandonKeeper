#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <SFML\Graphics.hpp>
#include <fstream>
#include "TextureManager.hpp"
#include "Tile.hpp"

class Engine {

private:

	static const int WINDOW_WIDTH = 1024;
	static const int WINDOW_HEIGHT = 768;

	static const int MAP_SIZE = 64;
	static const int TILE_SIZE = 48;
	static const int TOTAL_TITLES = MAP_SIZE * MAP_SIZE;
	static const int HORIZONTAL_PIXELS = TILE_SIZE * MAP_SIZE;

	static const int SCROLL_ZONE_WIDTH = TILE_SIZE / 2;
	static const int SCROLL_SPEED = SCROLL_ZONE_WIDTH;

	sf::RenderWindow* window;
	sf::View* camera;
	sf::Texture* textureMap;
	Tile* focusedTile;

	std::vector<Tile*> tiles;
	std::ofstream log;

	unsigned char cameraScrolls;
	bool isDebugging;

	void mainLoop();
	void initialize();
	void initializeMap();
	void update();
	void updateCamera();
	void updateHover();
	void lockMouse(bool isLocked);
	void renderFrame();
	void drawMap();
	void processInput();

public:

	Engine(bool isDebugging);
	~Engine();
	void start();
};

#endif