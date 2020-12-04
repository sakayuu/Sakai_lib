#include <Windows.h>
#include <iostream>
#include "Engine/Engine.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	auto& engine = Engine::Instance();
	if (!engine.Init())
		return -1;

	engine.Run();
	engine.Terminate();

	return 0;
}
