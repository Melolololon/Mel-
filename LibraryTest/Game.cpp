#include "Game.h"
#include<Library.h>
#include"SceneManager.h"
#include"ObjectManager.h"
#include"Play.h"


Game::Game() {}

Game::~Game() {}

Game* Game::GetInstance()
{
	static Game game;
	return &game;
}

void Game::Run()
{
	Initialize();
	Library::SetFramesPerSecond60(true);
	while (1)
	{
		Library::LoopStartProcess();
		Update();
		Draw();
		Library::LoopEndProcess();
		if (Library::GetIsEnd())break;
	}

	end();
}
void Game::Initialize()
{

	Library::Initialize(1280, 720, {0,0,0,255},L"MEL_Magical_Engene_Library(仮)");
	Library::SetCamera(Vector3( 0,0,-200 ), Vector3( 0,40,0 ), Vector3(0,1,0 ));
	Library::SetCameraNearAndFar(0.1, 1000.0f);
	Library::SetLightVector({ 0,0,1 });

#pragma region マネージャー初期化

	ObjectManager::GetInstance()->Initialize();
	CollisionFlag initFlag;
	initFlag.board = false;
	initFlag.box = false;
	initFlag.lineSegment = false;
	initFlag.plane = false;
	initFlag.ray = false;
	initFlag.sphere = true;
	ObjectManager::GetInstance()->SetCollisionFlag3D(initFlag);
	ObjectManager::GetInstance()->SetMouseCollisionFlag(false);
	ObjectManager::GetInstance()->ReserveObjectArray(100);

	SceneManager::getInstace()->Initialize(new Play());
#pragma endregion



}


void Game::end()
{
#pragma region マネージャー終了
	ObjectManager::GetInstance()->end();

	SceneManager::getInstace()->end();
#pragma endregion


	Library::Finalize();//絶対に最後に書く
}
void Game::Update()
{
	SceneManager::getInstace()->Update();
}

void Game::Draw()
{
	SceneManager::getInstace()->Draw();
}