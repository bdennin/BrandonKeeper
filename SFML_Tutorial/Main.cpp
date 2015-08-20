#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "Engine.hpp"

int main() {
	
	Engine* engine = new Engine(true);
	engine->start();

	_CrtDumpMemoryLeaks();
}