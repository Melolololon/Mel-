#include<Windows.h>
#include"Game.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Game::getInstance()->run();
	return 0;
}