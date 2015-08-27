#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <SFML\Graphics.hpp>
#include <fstream>
#include "TextureManager.hpp"
#include "Tile.hpp"

class Engine {

private:

	static const int MAP_SIZE = 64;
	static const int TILE_SIZE = 48;
	static const int TOTAL_TITLES = MAP_SIZE * MAP_SIZE;
	static const int HORIZONTAL_PIXELS = TILE_SIZE * MAP_SIZE;
	static const int SCROLL_ZONE_WIDTH = TILE_SIZE / 2;
	
	sf::RenderWindow* window;
	sf::View* camera;
	sf::View* miniMap;
	sf::View* GUI;
	sf::Texture* textureMap;
	Tile* focusedTile;

	std::vector<Tile*> tiles;
	std::ofstream log;

	float windowWidth;
	float windowHeight;
	int scrollSpeed;
	int cameraScrolls;
	bool isDebugging;
	bool isLeftMousePressed;
	bool isTileSelected;

	void mainLoop();
	void initialize();
	void initializeMap();
	void initializeCamera();
	void update();
	void updateCamera(int scrollSpeed);
	void updateHover();
	void updateSelection();
	void renderFrame();
	void drawMap();
	void drawMiniMap();
	void drawGUI();
	void processInput();
	
	void lockMouse(bool isLocked);

public:

	Engine(bool isDebugging, float windowWidth = 1024, float windowHeight = 768);
	~Engine();
	void start();
};

#endif