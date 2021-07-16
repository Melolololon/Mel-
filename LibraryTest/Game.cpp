#include "Game.h"
#include<Library.h>
#include"SceneManager.h"
#include"ObjectManager.h"
#include"Play.h"
#include"Camera.h"


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
	while (1)
	{
		Library::LoopStartProcess();
		Update();
		Draw();
		Library::LoopEndProcess();
		if (Library::GetIsEnd())
			break;
	}

	Finalize();
}

void Game::Initialize()
{
	

	Library::Initialize(1280, 720, {200,200,200,255},L"MyLib");
	Library::SetFramesPerSecond60(true);
	
	Camera::Get()->SetRotateCriteriaPosition(Vector3(0, 0, -5));

#pragma region �}�l�[�W���[������

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

	SceneManager::GetInstace()->SetStartScene(new Play());
#pragma endregion



}


void Game::Finalize()
{
	Library::Finalize();//��΂ɍŌ�ɏ���
}
void Game::Update()
{
	SceneManager::GetInstace()->Update();
}

void Game::Draw()
{
	SceneManager::GetInstace()->Draw();
}