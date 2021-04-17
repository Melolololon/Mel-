#include "Game.h"
#include<Library.h>
#include"SceneManager.h"
#include"ObjectManager.h"
#include"Play.h"

#include"FbxLoader.h"


Game::Game() {}

Game::~Game() {}

Game* Game::getInstance()
{
	static Game game;
	return &game;
}

void Game::run()
{
	initialize();
	Library::setFramesPerSecond60(true);
	while (1)
	{
		Library::loopStartProcess();
		update();
		draw();
		Library::loopEndProcess();
		if (Library::getIsEnd())break;
	}

	end();
}
void Game::initialize()
{

	Library::initialize(1280, 720, {0,0,0,255},L"MEL_Magical_Engene_Library(仮)");
	Library::setCamera({ 0,0,-10 }, { 0,0,0 }, { 0,1,0 });

#pragma region マネージャー初期化

	ObjectManager::getInstance()->initialize();
	CollisionFlag initFlag;
	initFlag.board = false;
	initFlag.box = false;
	initFlag.lineSegment = false;
	initFlag.plane = false;
	initFlag.ray = false;
	initFlag.sphere = true;
	ObjectManager::getInstance()->setCollisionFlag3D(initFlag);
	ObjectManager::getInstance()->setMouseCollisionFlag(false);
	ObjectManager::getInstance()->reserveObjectArray(100);

	SceneManager::getInstace()->initialize(new Play());
#pragma endregion

	FbxLoader::getInstance()->loadFbxModel("Resources/cube/cube.fbx");
}


void Game::end()
{
#pragma region マネージャー終了
	ObjectManager::getInstance()->end();

	SceneManager::getInstace()->end();
#pragma endregion


	Library::end();//絶対に最後に書く
}

void Game::update()
{
	SceneManager::getInstace()->update();
}

void Game::draw()
{
	SceneManager::getInstace()->draw();
}