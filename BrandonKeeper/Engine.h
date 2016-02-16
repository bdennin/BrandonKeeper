#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <SFML\Graphics.hpp>

#include "Imp.h"
#include "Creature.h"
#include "Grid.h"
#include "Tile.h"
#include "SoundHandler.h"
#include "JobHandler.h"
#include "windows.h"

#include <iostream>
#include <thread>
#include <unordered_set>

using namespace std;
using namespace sf;

class Engine
{

private:

	//const int vars
	static const int Engine::WORLD_TILES = 128;
	static const int Engine::TOTAL_TILES = WORLD_TILES * WORLD_TILES;
	static const int Engine::TILE_PIXELS = 48;
	static const int Engine::WORLD_PIXELS = TILE_PIXELS * WORLD_TILES;
	static const int Engine::TWO_TILE_PIXELS = 2 * TILE_PIXELS;
	static const int Engine::FOUR_TILE_PIXELS = 4 * TILE_PIXELS;
	static const int Engine::MAP_TILE_PIXELS = 3;
	static const int Engine::MAP_PIXELS = MAP_TILE_PIXELS * WORLD_TILES;
	static const int Engine::CREATURE_PIXELS = 48;
	static const int Engine::CREATURE_MAP_PIXELS = 3;
	static const int Engine::SCROLL_ZONE_WIDTH = 4;

	static const int Engine::MAX_CAMERA_ZOOM = 4;
	static const int Engine::MIN_CAMERA_ZOOM = 0;
	static const int Engine::MAX_MAP_ZOOM = WORLD_TILES / 256 + 4;
	static const int Engine::MIN_MAP_ZOOM = 0;

	static const int Engine::PROCESS_SLEEP_TIME = 100;
	static const int Engine::UPDATE_SLEEP_TIME = 8;
	static const int Engine::RENDER_SLEEP_TIME = 2;
	static const int Engine::CREATURE_SLEEP_TIME = 10;

	static const int Engine::MAP_REFRESH_SIZE = 120;

	//const float vars
	static const float Engine::CAMERA_ZOOM_RATIO;
	static const float Engine::MAP_ZOOM_RATIO;

	static const float Engine::CAMERA_HORIZONTAL_RATIO;
	static const float Engine::CAMERA_VERTICAL_RATIO;

	static const float Engine::GUI_HORIZONTAL_RATIO;
	static const float Engine::GUI_VERTICAL_RATIO;
	static const float Engine::GUI_HORIZONTAL_OFFSET;
	static const float Engine::GUI_VERTICAL_OFFSET;

	static const float Engine::MAP_HORIZONTAL_RATIO;
	static const float Engine::MAP_VERTICAL_RATIO;
	static const float Engine::MAP_HORIZONTAL_OFFSET;
	static const float Engine::MAP_VERTICAL_OFFSET;

	static const float Engine::MAP_MIN_X;
	static const float Engine::MAP_MAX_X;
	static const float Engine::MAP_AVERAGE_X;
	static const float Engine::MAP_MIN_Y;
	static const float Engine::MAP_MAX_Y;
	static const float Engine::MAP_AVERAGE_Y;

	static const float Engine::MAP_TILE_RATIO;

	RenderWindow* window;
	Thread* renderThread;
	Thread* updateThread;
	Thread* creatureThread;
	RenderTexture* mapImage;
	Grid* pathfinder;
	JobHandler* jobHandler;
	Clock* processTimer;
	Clock* renderTimer;
	Clock* updateTimer;
	Clock* creatureTimer;
	View* camera;
	View* GUI;
	View* worldMap;
	Texture* tileTextures;
	Texture* GUITextures;
	Texture* creatureTextures;
	Sprite* mapSprite;
	Sprite* GUISprite;
	Tile* hoveredTile;
	RectangleShape* cameraFrame;
	Mutex* globalMutex;

	vector<Tile*>* tiles;
	map<int, Tile*>* selectedTiles;
	unordered_set<Tile*>* changedTiles;
	vector<Imp*>* workers;
	vector<Creature*>* creatures;

	int windowWidth;
	int windowHeight;
	int frameCounter;
	int processPrintCeiling;
	int renderPrintCeiling;
	int updatePrintCeiling;
	int creaturePrintCeiling;
	int scrollSpeed;
	int cameraScrolls;
	int worldMapScrolls;
	bool isRendering;
	bool isUpdating;
	bool hasCreatureUpdates;
	bool isDebugging;
	bool isPaused;
	bool isFocused;
	bool isMouseInMap;
	bool isLeftMousePressed;
	bool isHoveredSelected;
	bool isMapUpdating;

	void initializeEngine();
	void initializeViews();
	void initializeTextures();
	void initializeSound();
	void initializeWorld();
	void initializeMap();
	void start();
	void processInput();
	void update();
	void updateCamera(int scrollSpeed);
	void refocusCamera();
	void updateHover();
	void updateMap();
	void updateCreatures();
	void handleMouseEvents();
	void renderFrame();
	void drawWorld();
	void drawMap();
	void drawMapTexture();
	void drawFrames();
	void drawCreatures();
	void drawGUI();

	void lockMouse();
	void unlockMouse();
	void pause();
	void unpause();

	void addRandomCreature(Tile* tile);
	void addImp(Tile* tile);

public:

	static const Clock* Engine::GAME_CLOCK;

	Engine(bool isDebugging, int windowWidth = 1024, int windowHeight = 768);
	~Engine();
};

#endif