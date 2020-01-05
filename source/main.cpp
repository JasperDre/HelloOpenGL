#include "Engine.h"

int main(int argc, char** argv)
{
	Engine engine;
	engine.Setup();
	engine.Loop();
	engine.Destroy();

	return 0;
}
