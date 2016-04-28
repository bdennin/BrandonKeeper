#include "Engine.h"

typedef map<int, Tile*>::iterator map_iterator;
typedef unordered_set<Tile*>::iterator set_iterator;

const Clock* Engine::GAME_CLOCK = new Clock();

const float Engine::CAMERA_ZOOM_RATIO = .8f;
const float Engine::MAP_ZOOM_RATIO = .6f;

const float Engine::CAMERA_HORIZONTAL_RATIO = 1.0f;
const float Engine::CAMERA_VERTICAL_RATIO = .7f;

const float Engine::GUI_HORIZONTAL_RATIO = 1.0f;
const float Engine::GUI_VERTICAL_RATIO = .3f;
const float Engine::GUI_HORIZONTAL_OFFSET = 0.0f;
const float Engine::GUI_VERTICAL_OFFSET = .7f;

const float Engine::MAP_HORIZONTAL_RATIO = .25f;
const float Engine::MAP_VERTICAL_RATIO = .25f;
const float Engine::MAP_HORIZONTAL_OFFSET = .025f;
const float Engine::MAP_VERTICAL_OFFSET = .725f;

const float Engine::MAP_MIN_X = Engine::MAP_HORIZONTAL_OFFSET;
const float Engine::MAP_MAX_X = Engine::MAP_HORIZONTAL_OFFSET + Engine::MAP_HORIZONTAL_RATIO;
const float Engine::MAP_AVERAGE_X = (Engine::MAP_MIN_X + Engine::MAP_MAX_X) / 2;
const float Engine::MAP_MIN_Y = Engine::MAP_VERTICAL_OFFSET;
const float Engine::MAP_MAX_Y = Engine::MAP_VERTICAL_OFFSET + Engine::MAP_VERTICAL_RATIO;
const float Engine::MAP_AVERAGE_Y = (Engine::MAP_MIN_Y + Engine::MAP_MAX_Y) / 2;

const float Engine::MAP_TILE_RATIO = Engine::TILE_PIXELS / Engine::MAP_TILE_PIXELS;

Engine::Engine(bool loadGame, bool isDebugging, int windowWidth, int windowHeight)
{
	this->isDebugging = isDebugging;
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	this->initializeEngine();

	if (loadGame)
		this->load();

	this->start();
}

Engine::~Engine()
{
	delete GAME_CLOCK;	
	delete jobHandler;
	delete window;
	delete updateThread;
	delete renderThread;
	delete creatureThread;
	delete mapImage;
	delete pathfinder;
	delete processTimer;
	delete renderTimer;
	delete updateTimer;
	delete creatureTimer;
	delete selectTimer;
	delete camera;
	delete GUI;
	delete worldMap;
	delete tileTextures;
	delete GUITextures;
	delete creatureTextures;
	delete mapSprite;
	delete GUISprite;
	delete cameraFrame;
	delete globalMutex;

	for (Tile* tile : *tiles)
		delete tile;

	for (Creature* creature : *creatures)
		delete creature;

	delete tiles;
	delete creatures;
	delete changedTiles;
	delete selectedTiles;
	delete workers;

	creatureThread->terminate();
	renderThread->terminate();
	updateThread->terminate();
	SoundHandler::destroySounds();
}

void Engine::initializeEngine()
{
	//initialize pointers
	this->window = new RenderWindow(VideoMode(windowWidth, windowHeight), "Brandon Keeper");
	this->renderThread = new Thread(&Engine::renderFrame, this);
	this->updateThread = new Thread(&Engine::update, this);
	this->creatureThread = new Thread(&Engine::updateCreatures, this);
	this->mapImage = new RenderTexture();
	this->processTimer = new Clock();
	this->renderTimer = new Clock();
	this->updateTimer = new Clock();
	this->creatureTimer = new Clock();
	this->selectTimer = new Clock();
	this->camera = new View();
	this->GUI = new View();
	this->worldMap = new View();
	this->tileTextures = new Texture();
	this->GUITextures = new Texture();
	this->creatureTextures = new Texture();
	this->mapSprite = new Sprite();
	this->GUISprite = new Sprite();
	this->cameraFrame = new RectangleShape();
	this->globalMutex = new Mutex();
	this->tiles = new vector<Tile*>();
	this->treasuryTiles = new vector<Tile*>();
	this->selectedTiles = new map<int, Tile*>();
	this->changedTiles = new unordered_set<Tile*>();
	this->workers = new vector<Imp*>();
	this->creatures = new vector<Creature*>();
	this->hoveredTile = nullptr;

	//initialize non-pointers
	this->frameCounter = 0;
	this->processPrintCeiling = 241;
	this->renderPrintCeiling = 251;
	this->updatePrintCeiling = 257;
	this->creaturePrintCeiling = 263;
	this->scrollSpeed = 2;
	this->cameraScrolls = 0;
	this->worldMapScrolls = 0;
	this->playerGold = 1000;
	this->isRendering = true;
	this->isUpdating = true;
	this->hasCreatureUpdates = true;
	this->isPaused = false;
	this->isFocused = true;
	this->isMouseInMap = false;
	this->isLeftMousePressed = false;
	this->isHoveredSelected = false;
	this->isMapUpdating = false;

	this->window->setFramerateLimit(120);

	this->initializeViews();
	this->initializeTextures();
	this->initializeSound();
	this->initializeWorld();
	this->initializeMap();

	//advanced objects
	this->jobHandler = new JobHandler(this->tiles, this->treasuryTiles, this->selectedTiles, this->workers, this->pathfinder, 1, Engine::WORLD_TILES, this->isDebugging);
	this->pathfinder = new Grid(this->tiles, this->treasuryTiles, this->changedTiles, 1, Engine::WORLD_TILES, this->isDebugging);

	this->pathfinder->initialize((*tiles)[129]);

	this->lockMouse();
}

void Engine::initializeViews()
{
	Vector2f cameraSize = Vector2f(this->windowWidth, this->windowHeight * Engine::CAMERA_VERTICAL_RATIO);
	Vector2f cameraCenter = cameraSize * .5f;
	FloatRect cameraArea = FloatRect(0, 0, Engine::CAMERA_HORIZONTAL_RATIO, Engine::CAMERA_VERTICAL_RATIO);

	Vector2f mapSize = Vector2f(Engine::MAP_PIXELS, Engine::MAP_PIXELS);
	Vector2f mapCenter = mapSize * .5f;
	FloatRect mapArea = FloatRect(Engine::MAP_HORIZONTAL_OFFSET, Engine::MAP_VERTICAL_OFFSET, Engine::MAP_HORIZONTAL_RATIO, Engine::MAP_VERTICAL_RATIO);

	Vector2f GUISize = Vector2f(windowWidth * Engine::GUI_HORIZONTAL_RATIO, windowHeight * Engine::GUI_VERTICAL_RATIO);
	Vector2f GUICenter = GUISize * .5f;
	FloatRect GUIArea = FloatRect(Engine::GUI_HORIZONTAL_OFFSET, Engine::GUI_VERTICAL_OFFSET, Engine::GUI_HORIZONTAL_RATIO, Engine::GUI_VERTICAL_RATIO);

	this->camera->setSize(cameraSize);
	this->camera->setCenter(cameraCenter);
	this->camera->setViewport(cameraArea);

	this->worldMap->setSize(mapSize);
	this->worldMap->setCenter(mapCenter);
	this->worldMap->setViewport(mapArea);

	this->GUI->setSize(GUISize);
	this->GUI->setCenter(GUICenter);
	this->GUI->setViewport(GUIArea);
}

void Engine::initializeTextures()
{
	IntRect GUIDimensions = IntRect(0, 0, this->windowWidth, this->windowHeight * Engine::GUI_VERTICAL_RATIO);
	IntRect mapDimensions = IntRect(0, 0, Engine::MAP_PIXELS, Engine::MAP_PIXELS);

	this->tileTextures->loadFromFile("Resources/tileTextures.png");
	this->GUITextures->loadFromFile("Resources/background.png");
	this->creatureTextures->loadFromFile("Resources/creatureTextures.png");

	this->mapImage->create(Engine::MAP_PIXELS, Engine::MAP_PIXELS);

	this->GUITextures->setRepeated(true);

	this->GUISprite->setTexture(*GUITextures);
	this->GUISprite->setPosition(0, 0);
	this->GUISprite->setTextureRect(GUIDimensions);

	this->mapSprite->setTexture(mapImage->getTexture());
	this->mapSprite->setPosition(0, 0);
	this->mapSprite->setTextureRect(mapDimensions);

	this->cameraFrame->setFillColor(Color::Transparent);
	this->cameraFrame->setOutlineColor(Color::White);
	this->cameraFrame->setOutlineThickness(Engine::WORLD_TILES / 64);
}

void Engine::initializeSound()
{
	SoundHandler::initializeSounds();
	SoundHandler::startGameMusic();
}

void Engine::initializeWorld()
{
	srand(time(NULL));

	for (int i = 0; i < Engine::TOTAL_TILES; i++)
	{
		Vector2f* position = new Vector2f();
		Tile* tile;
		TileType type;

		position->x = i % Engine::WORLD_TILES * Engine::TILE_PIXELS;
		position->y = i / Engine::WORLD_TILES * Engine::TILE_PIXELS;

		int random = rand() % 10000;

		if (random < 1)
			type = TileType::GEM;
		else if (random < 8000)
			type = TileType::EARTH;
		else if (random < 9000)
			type = TileType::GOLD;
		else
			type = TileType::TREASURY;

		if (i < Engine::WORLD_TILES || i % Engine::WORLD_TILES == 0 || i % Engine::WORLD_TILES == Engine::WORLD_TILES - 1 || i > Engine::TOTAL_TILES - Engine::WORLD_TILES)
			type = TileType::ROCK;

		if (i == 129 || i == 130 || i == 257 || i == 258)
			type = TileType::TREASURY;

		tile = new Tile(this->tileTextures, position, type, i, Engine::TILE_PIXELS, Engine::MAP_TILE_PIXELS);

		this->tiles->push_back(tile);
	}
}

void Engine::initializeMap()
{
	for (int i = 0; i < Engine::TOTAL_TILES; i++)
		this->mapImage->draw(*(*this->tiles)[i]->getMapSprite());

	this->mapImage->display();
}

void Engine::start()
{
	this->renderThread->launch();
	this->updateThread->launch();
	this->creatureThread->launch();
	this->jobHandler->start();

	while (window->isOpen())
		this->processInput();

	exit(0);
}

void Engine::processInput()
{
	Sleep(Engine::PROCESS_SLEEP_TIME);

	if (isDebugging)
		processTimer->restart();

	Event event;

	while (this->window->pollEvent(event))
	{
		switch (event.type)
		{
			case Event::MouseButtonPressed:
			{
				if (event.mouseButton.button == Mouse::Left)
				{
					this->isLeftMousePressed = true;

					if (hoveredTile != nullptr)
					{
						this->hoveredTile->toggleSelected();
						this->isHoveredSelected = hoveredTile->getSelected();

						this->addImp(this->hoveredTile);
					}
					else
					{
						Vector2i windowPosition = Mouse::getPosition(*this->window);

						int windowX = windowPosition.x;
						int windowY = windowPosition.y;

						int minWindowX = this->windowWidth * Engine::MAP_MIN_X;
						int maxWindowX = this->windowWidth * Engine::MAP_MAX_X;
						int minWindowY = this->windowHeight * Engine::MAP_MIN_Y;
						int maxWindowY = this->windowHeight * Engine::MAP_MAX_Y;

						if (windowX > minWindowX && windowX < maxWindowX && windowY > minWindowY && windowY < maxWindowY)
							this->isMouseInMap = true;
						else
							this->isMouseInMap = false;
					}
				}

				break;
			}

			case Event::MouseButtonReleased:
			{
				if (event.mouseButton.button == Mouse::Left)
				{
					this->isLeftMousePressed = false;
					this->isMouseInMap = false;
					this->isHoveredSelected = NULL;
				}

				break;
			}

			case Event::MouseWheelScrolled:
			{
				Vector2i windowPosition = Mouse::getPosition(*this->window);

				int windowX = windowPosition.x;
				int windowY = windowPosition.y;

				int minWindowX = this->windowWidth * Engine::MAP_MIN_X;
				int maxWindowX = this->windowWidth * Engine::MAP_MAX_X;
				int minWindowY = this->windowHeight * Engine::MAP_MIN_Y;
				int maxWindowY = this->windowHeight * Engine::MAP_MAX_Y;

				if (windowX > minWindowX && windowX < maxWindowX && windowY > minWindowY && windowY < maxWindowY)
				{
					this->window->setView(*worldMap);

					if (event.mouseWheelScroll.delta < 0 && worldMapScrolls > Engine::MIN_MAP_ZOOM)
					{
						this->worldMap->zoom(1 / Engine::MAP_ZOOM_RATIO);

						if (this->worldMapScrolls == 1)
						{
							Vector2f mapSize = Vector2f(Engine::MAP_PIXELS, Engine::MAP_PIXELS);
							Vector2f mapCenter = mapSize * .5f;
							this->worldMap->setCenter(mapCenter);

							this->cameraFrame->setOutlineThickness(Engine::WORLD_TILES / 64);
						}
						else
						{
							Vector2f globalPosition = this->window->mapPixelToCoords(windowPosition);
							Vector2f viewSize = this->worldMap->getSize();

							int viewX = viewSize.x / 2 + 2;
							int viewY = viewSize.y / 2 + 2;

							int dx = globalPosition.x - viewX;
							int dy = globalPosition.y - viewY;

							if (dx < 0)
								globalPosition.x -= dx;
							if (dy < 0)
								globalPosition.y -= dy;

							dx = globalPosition.x + viewX;
							dy = globalPosition.y + viewY;

							if (dx >= Engine::MAP_PIXELS)
								globalPosition.x -= dx - Engine::MAP_PIXELS;
							if (dy >= Engine::MAP_PIXELS)
								globalPosition.y -= dy - Engine::MAP_PIXELS;

							this->worldMap->setCenter(globalPosition);

							globalPosition.x *= Engine::MAP_AVERAGE_X;
							globalPosition.y *= Engine::MAP_AVERAGE_Y;

							this->cameraFrame->setOutlineThickness(viewX / 128 + 2);
						}

						--this->worldMapScrolls;
					}
					else if (event.mouseWheelScroll.delta > 0 && worldMapScrolls < MAX_MAP_ZOOM)
					{
						this->worldMap->zoom(Engine::MAP_ZOOM_RATIO);

						Vector2f globalPosition = this->window->mapPixelToCoords(windowPosition);
						Vector2f viewSize = this->worldMap->getSize();

						int viewX = viewSize.x / 2 + 2;
						int viewY = viewSize.y / 2 + 2;

						int dx = globalPosition.x - viewX;
						int dy = globalPosition.y - viewY;

						if (dx < 0)
							globalPosition.x -= dx;
						if (dy < 0)
							globalPosition.y -= dy;

						dx = globalPosition.x + viewX;
						dy = globalPosition.y + viewY;

						if (dx >= Engine::MAP_PIXELS)
							globalPosition.x -= dx - Engine::MAP_PIXELS;
						if (dy >= Engine::MAP_PIXELS)
							globalPosition.y -= dy - Engine::MAP_PIXELS;

						this->worldMap->setCenter(globalPosition);

						this->cameraFrame->setOutlineThickness(viewX / 128 + 2);

						++this->worldMapScrolls;
					}

					this->window->setView(*camera);
				}
				//Zoom camera
				else
				{
					if (event.mouseWheelScroll.delta < 0 && cameraScrolls > Engine::MIN_CAMERA_ZOOM)
					{
						--this->cameraScrolls;
						this->camera->zoom(1 / Engine::CAMERA_ZOOM_RATIO);
						this->refocusCamera();
					}
					else if (event.mouseWheelScroll.delta > 0 && cameraScrolls < Engine::MAX_CAMERA_ZOOM)
					{
						++this->cameraScrolls;
						this->camera->zoom(Engine::CAMERA_ZOOM_RATIO);
						this->refocusCamera();
					}
				}

				break;
			}

			case Event::GainedFocus:
			{
				this->isFocused = true;
				this->lockMouse();

				break;
			}

			case Event::LostFocus:
			{
				this->isFocused = false;
				this->unlockMouse();

				break;
			}

			case Event::Resized:
			{
				this->windowWidth = event.size.width;
				this->windowHeight = event.size.height;

				this->initializeViews();
				this->initializeTextures();
				this->initializeMap();
				this->lockMouse();

				break;
			}

			case Event::KeyPressed:
			{
				Keyboard::Key keyPressed = event.key.code;

				if (keyPressed == Keyboard::Escape)
				{
					this->unlockMouse();
					this->isRendering = false;
					this->isUpdating = false;
					this->window->close();
					exit(0);
				}
				else if (keyPressed == Keyboard::S)
				{
					this->save();
				}
				else if (keyPressed == Keyboard::L)
				{
					this->load();
				}

				break;
			}

			case Event::Closed:
			{
				this->unlockMouse();
				this->isRendering = false;
				this->isUpdating = false;
				this->window->close();
				break;
			}

			default:
			{
				break;
			}
		}
	}

	if (isDebugging)
	{
		if (frameCounter % processPrintCeiling == 0)
		{
			Time elapsed = processTimer->getElapsedTime();
			cout << "Engine::processEvents -> " << elapsed.asMicroseconds() << " microseconds.\n";
		}
	}
}

void Engine::update()
{
	while (window->isOpen())
	{
		Sleep(Engine::UPDATE_SLEEP_TIME);

		if (this->isFocused && this->isUpdating && !this->isPaused)
		{
			this->handleMouseEvents();
			this->updateCamera(this->scrollSpeed);
			this->updateHover();

			if (this->changedTiles->size() > Engine::MAP_REFRESH_SIZE)
			{
				this->updateMap();
			}
		}
	}
}

void Engine::renderFrame()
{
	while (window->isOpen())
	{
		if (this->isFocused && this->isRendering && !this->isPaused)
		{
			Sleep(Engine::RENDER_SLEEP_TIME);

			this->window->clear();

			++this->frameCounter;

			this->drawWorld();
			this->drawCreatures();
			this->drawGUI();
			this->drawMap();
			this->drawFrames();

			this->window->display();
		}
	}
}

void Engine::updateCamera(int scrollSpeed)
{
	if (isDebugging)
		updateTimer->restart();

	if (!this->isMouseInMap)
	{
		Vector2i windowPosition = Mouse::getPosition(*this->window);
		Vector2f cameraCenter = this->camera->getCenter();
		Vector2f cameraBounds = this->camera->getSize();

		int windowX = windowPosition.x;
		int windowY = windowPosition.y;

		int camX = cameraCenter.x;
		int camY = cameraCenter.y;

		float camWidth = cameraBounds.x;
		float camHeight = cameraBounds.y;

		int halfWidth = camWidth / 2 + .5f;
		int halfHeight = camHeight / 2 + .5f;

		//mouse is in left portion of the window
		if (windowX < Engine::SCROLL_ZONE_WIDTH)
		{
			if (camX > halfWidth + scrollSpeed)
			{
				camX -= scrollSpeed;
			}
			else
			{
				camX = halfWidth;
			}
		}
		//mouse is in right portion of the window
		else if (windowX > this->windowWidth - Engine::SCROLL_ZONE_WIDTH)
		{
			if (camX < Engine::WORLD_PIXELS - halfWidth - scrollSpeed)
			{
				camX += scrollSpeed;
			}
			else
			{
				camX = Engine::WORLD_PIXELS - halfWidth;
			}
		}

		//mouse is in top portion of the window
		if (windowY < Engine::SCROLL_ZONE_WIDTH)
		{
			if (camY > halfHeight + scrollSpeed)
			{
				camY -= scrollSpeed;
			}
			else
			{
				camY = halfHeight;
			}
		}
		//mouse is in bottom portion of the window
		else if (windowY > this->windowHeight - Engine::SCROLL_ZONE_WIDTH)
		{
			if (camY < Engine::WORLD_PIXELS - halfHeight - scrollSpeed)
			{
				camY += scrollSpeed;
			}
			else
			{
				camY = Engine::WORLD_PIXELS - halfHeight;
			}
		}

		this->camera->setCenter(camX, camY);
	}

	if (isDebugging)
	{
		if (frameCounter % updatePrintCeiling == 0)
		{
			Time elapsed = updateTimer->getElapsedTime();
			cout << "Engine::updateCamera -> " << elapsed.asMicroseconds() << " microseconds.\n";
		}
	}
}

void Engine::refocusCamera()
{
	if (isDebugging)
		this->processTimer->restart();

	Vector2i firstPosition(0, 0);
	Vector2f cameraPosition = this->camera->getSize();
	Vector2f worldPosition = this->window->mapPixelToCoords(firstPosition);

	int camX = cameraPosition.x / 2;
	int camY = cameraPosition.y / 2;

	int x = worldPosition.x + camX;
	int y = worldPosition.y + camY;

	int maxX = Engine::WORLD_PIXELS - camX;
	int maxY = Engine::WORLD_PIXELS - camY;

	if (x < camX)
		x = camX;
	else if (x > maxX)
		x = maxX;


	if (y < camY)
		y = camY;
	else if (y > maxY)
		y = maxY;

	this->camera->setCenter(x, y);

	if (isDebugging)
	{
		if (frameCounter % processPrintCeiling == 0)
		{
			Time elapsed = processTimer->getElapsedTime();
			cout << "Engine::refocusCamera -> " << elapsed.asMicroseconds() << " microseconds.\n";
		}
	}
}

void Engine::updateHover()
{
	if (isDebugging)
		this->updateTimer->restart();

	Vector2i windowPosition = sf::Mouse::getPosition(*this->window);
	int winY = windowPosition.y;
	int camMaxHeight = this->windowHeight * Engine::CAMERA_VERTICAL_RATIO;

	//mouse is in camera area
	if (winY < camMaxHeight)
	{
		Vector2f globalPosition = this->window->mapPixelToCoords(windowPosition);

		int x = globalPosition.x;
		int y = globalPosition.y;

		int tileX = x / Engine::TILE_PIXELS;
		int tileY = y / Engine::TILE_PIXELS;
		int tileIndex = Engine::WORLD_TILES * tileY + tileX;

		if (tileIndex < Engine::TOTAL_TILES && tileIndex >= 0)
		{
			Tile* focusedTile = (*this->tiles)[tileIndex];

			if (nullptr == this->hoveredTile)
			{
				this->hoveredTile = focusedTile;
				this->hoveredTile->toggleHovered();
			}
			else if (hoveredTile != focusedTile)
			{
				this->hoveredTile->toggleHovered();
				this->hoveredTile = focusedTile;
				this->hoveredTile->toggleHovered();
			}
			else
			{
				//do nothing because focusedTile is already the focus
			}
		}
	}
	//mouse is not on a tile
	else
	{
		if (nullptr == this->hoveredTile)
		{

		}
		else
		{
			this->hoveredTile->toggleHovered();
			this->hoveredTile = nullptr;
		}
	}

	if (isDebugging)
	{
		if (frameCounter % updatePrintCeiling == 0)
		{
			Time elapsed = updateTimer->getElapsedTime();
			cout << "Engine::updateHover -> " << elapsed.asMicroseconds() << " microseconds.\n";
		}
	}
}

void Engine::updateMap()
{
	if (isDebugging)
		this->updateTimer->restart();

	this->mapImage->create(Engine::MAP_PIXELS, Engine::MAP_PIXELS);

	for (set_iterator it = changedTiles->begin(); it != changedTiles->end(); it++)
	{
		this->mapImage->draw(*(*it)->getMapSprite());
	}

	this->mapSprite->setTexture(mapImage->getTexture());

	this->mapImage->display();

	this->changedTiles->clear();

	if (isDebugging)
	{
		if (frameCounter % updatePrintCeiling == 0)
		{
			Time elapsed = updateTimer->getElapsedTime();
			cout << "Engine::updateMap -> " << elapsed.asMicroseconds() << " microseconds.\n";
		}
	}
}

void Engine::updateCreatures()
{
	while (window->isOpen())
	{
		if (this->hasCreatureUpdates && !this->isPaused)
		{
			if (isDebugging)
				this->creatureTimer->restart();

			Sleep(Engine::CREATURE_SLEEP_TIME);

			int length = this->workers->size();

			for (int i = 0; i < length; i++)
			{
				Imp* imp = (*this->workers)[i];
				
				bool playSounds;

				Vector2f position = *imp->getPosition();
				Vector2f impWorld = this->window->mapPixelToCoords((Vector2i)position);
				Vector2f center = this->camera->getCenter();
				Vector2f world = this->window->mapPixelToCoords((Vector2i)center);
				Vector2f size = this->camera->getSize();
				
				size.x /= 2;
				size.y /= 2;

				Vector2f camMax = world + size;
				Vector2f camMin = world - size;

				if (impWorld.x > camMin.x && impWorld.y > camMin.y && impWorld.x < camMax.x && impWorld.y < camMax.y)
					playSounds = true;
				else
					playSounds = false;

				imp->step(playSounds);
			}

			if (isDebugging)
			{
				if (frameCounter % creaturePrintCeiling == 0)
				{
					Time elapsed = creatureTimer->getElapsedTime();
					cout << "Engine::updateCreatures -> " << elapsed.asMicroseconds() << " microseconds.\n";
				}
			}
		}
	}
}

void Engine::handleMouseEvents()
{
	if (isDebugging)
		this->updateTimer->restart();

	if (this->isLeftMousePressed)
	{
		if (this->isMouseInMap)
		{
			this->window->setView(*this->worldMap);
			Vector2i windowPosition = Mouse::getPosition(*this->window);

			int windowX = windowPosition.x;
			int windowY = windowPosition.y;

			int minWindowX = this->windowWidth * Engine::MAP_MIN_X;
			int maxWindowX = this->windowWidth * Engine::MAP_MAX_X;
			int minWindowY = this->windowHeight * Engine::MAP_MIN_Y;
			int maxWindowY = this->windowHeight * Engine::MAP_MAX_Y;

			if (windowX > minWindowX && windowX < maxWindowX && windowY > minWindowY && windowY < maxWindowY)
			{
				this->window->setView(*this->worldMap);
				Vector2f globalPosition = this->window->mapPixelToCoords(windowPosition);

				Vector2i cameraPosition = (Vector2i)this->camera->getSize();

				int x = globalPosition.x * Engine::MAP_TILE_RATIO;
				int y = globalPosition.y * Engine::MAP_TILE_RATIO;

				int camX = cameraPosition.x / 2;
				int camY = cameraPosition.y / 2;

				int maxX = Engine::WORLD_PIXELS - camX;
				int maxY = Engine::WORLD_PIXELS - camY;

				if (x < camX)
					x = camX;
				else if (x > maxX)
					x = maxX;

				if (y < camY)
					y = camY;
				else if (y > maxY)
					y = maxY;

				this->camera->setCenter(x, y);

				this->window->setView(*this->camera);
			}
		}
		//else update tile selection
		else if (this->hoveredTile != nullptr)
		{
			int milli = this->selectTimer->getElapsedTime().asMilliseconds();

			if (milli > Engine::SELECT_COOLDOWN)
			{
				this->hoveredTile->setSelected(this->isHoveredSelected);
				int ID = hoveredTile->getID();

				if (isHoveredSelected)
				{
					(*this->selectedTiles)[ID] = hoveredTile;
				}
				else
				{
					this->selectedTiles->erase(ID);
					this->hoveredTile->setBeingExtracted(false);
				}

				this->selectTimer->restart();
			}
		}
	}

	if (isDebugging)
	{
		if (frameCounter % updatePrintCeiling == 0)
		{
			Time elapsed = updateTimer->getElapsedTime();
			cout << "Engine::handleMouseEvents -> " << elapsed.asMicroseconds() << " microseconds.\n";
		}
	}
}

void Engine::drawWorld()
{
	if (isDebugging)
		this->renderTimer->restart();

	this->window->setView(*this->camera);

	Vector2i firstPosition(0, 0);
	Vector2f globalPosition = this->window->mapPixelToCoords(firstPosition);
	Vector2f cameraBounds = this->camera->getSize();

	int x = globalPosition.x - Engine::TWO_TILE_PIXELS;
	int y = globalPosition.y - Engine::TWO_TILE_PIXELS;

	int maxX = x + cameraBounds.x + Engine::FOUR_TILE_PIXELS;
	int maxY = y + cameraBounds.y + Engine::FOUR_TILE_PIXELS;

	if (x < 0)
		x = 0;
	else if (maxX > Engine::WORLD_PIXELS)
		maxX = Engine::WORLD_PIXELS;

	if (y < 0)
		y = 0;
	else if (maxY > Engine::WORLD_PIXELS)
		maxY = Engine::WORLD_PIXELS;

	for (int i = y; i < maxY; i += Engine::TILE_PIXELS)
	{
		int tileY = i / Engine::TILE_PIXELS;

		for (int j = x; j < maxX; j += Engine::TILE_PIXELS)
		{
			int tileX = j / Engine::TILE_PIXELS;
			int tileIndex = Engine::WORLD_TILES * tileY + tileX;

			this->window->draw(*(*this->tiles)[tileIndex]->getSprite());
		}
	}

	if (isDebugging)
	{
		if (frameCounter % renderPrintCeiling == 0)
		{
			Time elapsed = renderTimer->getElapsedTime();
			cout << "Engine::drawWorld -> " << elapsed.asMicroseconds() << " microseconds.\n";
		}
	}
}

void Engine::drawMap()
{
	if (isDebugging)
		this->renderTimer->restart();

	this->window->setView(*this->worldMap);

	this->window->draw(*this->mapSprite);

	for (set_iterator it = changedTiles->begin(); it != changedTiles->end(); it++)
	{
		Tile* tile = *it;
		this->window->draw(*tile->getMapSprite());
	}

	for (map_iterator it = selectedTiles->begin(); it != selectedTiles->end(); it++)
	{
		if (it->second->getVisible() && !it->second->getMineable())
			this->selectedTiles->erase(it++);
		else
			this->window->draw(*it->second->getMapSprite());
	}

	int length = this->creatures->size();

	for (int i = 0; i < length; i++)
	{
		this->window->draw(*(*this->creatures)[i]->getMapSprite());
	}

	this->window->setView(*this->camera);

	if (isDebugging)
	{
		if (frameCounter % renderPrintCeiling == 0)
		{
			Time elapsed = renderTimer->getElapsedTime();
			cout << "Engine::drawMap -> " << elapsed.asMicroseconds() << " microseconds.\n";
		}
	}
}

void Engine::drawFrames()
{
	if (isDebugging)
		this->renderTimer->restart();

	this->window->setView(*worldMap);

	Vector2f frameSize = this->camera->getSize();
	Vector2f framePosition = this->camera->getCenter();
	float girth = cameraFrame->getOutlineThickness() * 2;

	frameSize.x = frameSize.x / Engine::MAP_TILE_RATIO - girth;
	frameSize.y = frameSize.y / Engine::MAP_TILE_RATIO - girth;

	framePosition.x = framePosition.x / Engine::MAP_TILE_RATIO - frameSize.x / 2;
	framePosition.y = framePosition.y / Engine::MAP_TILE_RATIO - frameSize.y / 2;

	this->cameraFrame->setSize(frameSize);
	this->cameraFrame->setPosition(framePosition);

	this->window->draw(*cameraFrame);
	this->window->setView(*camera);

	if (isDebugging)
	{
		if (frameCounter % renderPrintCeiling == 0)
		{
			Time elapsed = renderTimer->getElapsedTime();
			cout << "Engine::drawFrames -> " << elapsed.asMicroseconds() << " microseconds.\n";
		}
	}
}

void Engine::drawGUI()
{
	if (isDebugging)
		this->renderTimer->restart();

	this->window->setView(*this->GUI);
	this->window->draw(*this->GUISprite);
	this->window->setView(*this->camera);

	if (isDebugging)
	{
		if (frameCounter % renderPrintCeiling == 0)
		{
			Time elapsed = renderTimer->getElapsedTime();
			cout << "Engine::drawGUI -> " << elapsed.asMicroseconds() << " microseconds.\n";
		}
	}
}

void Engine::drawCreatures()
{
	for (Creature* creature : *this->creatures)
		this->window->draw(*creature->getSprite());
}

void Engine::lockMouse()
{
	WindowHandle handle = window->getSystemHandle();
	RECT rectangle;

	GetWindowRect(handle, &rectangle);
	ClipCursor(&rectangle);
}

void Engine::unlockMouse()
{
	RECT screen;
	screen.left = -1920;
	screen.top = 0;
	screen.right = GetSystemMetrics(SM_CXSCREEN);
	screen.bottom = GetSystemMetrics(SM_CYSCREEN);

	ClipCursor(&screen);
}

void Engine::pause()
{
	this->isPaused = true;
}

void Engine::unpause()
{
	this->isPaused = false;
}

void Engine::addRandomCreature(Tile* tile)
{

}

void Engine::addImp(Tile* tile)
{
	if (tile->getTraversable() && tile->getVisible())
	{
		Vector2f* position = new Vector2f();
		Vector2f* tilePosition = tile->getPosition();

		position->x = tilePosition->x;
		position->y = tilePosition->y;

		Imp* imp = new Imp(this->tiles, this->selectedTiles, this->changedTiles, this->workers, this->pathfinder, this->creatureTextures,
						   position, Engine::TILE_PIXELS, Engine::WORLD_TILES, Engine::CREATURE_PIXELS, Engine::CREATURE_MAP_PIXELS);

		this->creatures->push_back(imp);
		this->workers->push_back(imp);

		SoundHandler::playSpellSound("imp_spawn");
	}
}

void Engine::addPlayerGold(int gold)
{
	this->playerGold += gold;
}

void Engine::removePlayerGold(int gold)
{
	this->playerGold -= gold;
}

void Engine::setPlayerGold(int gold)
{
	this->playerGold = gold;
}

int Engine::getPlayerGold()
{
	return this->playerGold;
}

void Engine::save()
{
	ofstream saveFile;
	saveFile.clear();
	saveFile.open("default.save");

	saveFile << "camera\n";

	Vector2f cameraCenter = camera->getCenter();

	int camX = cameraCenter.x;
	int camY = cameraCenter.y;

	saveFile << camX << "\n" << camY << "\n";

	int length = tiles->size();
	
	saveFile << "tiles\n";

	for (int i = 0; i < length; i++)
	{
		Tile* current = (*this->tiles)[i];

		int type = current->getType();
		bool isSelected = current->getSelected();
		bool isVisible = current->getVisible();

		saveFile << i << " " << type << " " << isSelected << " " << isVisible << "\n";
	}

	length = workers->size();

	saveFile << "workers\n";

	for (int i = 0; i < length; i++)
	{
		Imp* imp = (*this->workers)[i];

		Vector2f position = *imp->getPosition();
		int x = position.x;
		int y = position.y;

		saveFile << i << " " << x << " " << y << "\n";
	}

	SoundHandler::playUrgentSound("voice_game_saved");
}

void Engine::load()
{
	creatures->clear();
	workers->clear();
	treasuryTiles->clear();
	selectedTiles->clear();
	pathfinder->clear();

	pause();
	jobHandler->pause();

	string line;
	ifstream saveFile;
	saveFile.open("default.save");

	while (getline(saveFile, line))
	{
		if (line == "camera")
		{
			getline(saveFile, line);

			int x = stoi(line);

			getline(saveFile, line);
			
			int y = stoi(line);

			Vector2f cameraCenter(x, y);
			camera->setCenter(cameraCenter);
		}
		else if (line == "tiles")
		{
			do
			{
				getline(saveFile, line);

				istringstream buffer(line);
				int index, type, isSelected, isVisible;

				buffer >> index >> type >> isSelected >> isVisible;

				Tile* current = (*this->tiles)[index];
				current->reinit();
				TileType tileType = (TileType)type;

				current->setType(tileType);
				current->setSelected(isSelected);
				current->setVisible(isVisible);

				if (tileType == TileType::TREASURY || tileType == TileType::TREASURY_MID || tileType == TileType::TREASURY_FULL)
					treasuryTiles->push_back(current);

				if (isSelected)
					(*selectedTiles)[index] = current;
			}
			while (line != "workers");
		}
		else
		{
			istringstream buffer(line);
			int index, x, y;

			buffer >> index >> x >> y;

			Vector2f* position = new Vector2f();
			position->x = x;
			position->y = y;

			Imp* imp = new Imp(this->tiles, this->selectedTiles, this->changedTiles, this->workers, this->pathfinder, this->creatureTextures,
							   position, Engine::TILE_PIXELS, Engine::WORLD_TILES, Engine::CREATURE_PIXELS, Engine::CREATURE_MAP_PIXELS);

			this->creatures->push_back(imp);
			this->workers->push_back(imp);
		}
	}	

	this->pathfinder->initialize((*tiles)[129]);

	jobHandler->unpause();
	unpause();

	SoundHandler::startGameMusic();
	SoundHandler::playUrgentSound("voice_game_loaded");
}