#include "Game.h"
#include<Library.h>
#include"SceneManager.h"
#include"GameObjectManager.h"
#include"Play.h"
#include"Camera.h"
#include"ModelData.h"
#include"PhysicsTest.h"
#include"TextWrite.h"
#include"Texture.h"

#include"HurikoGame.h"

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

	int x = 0;
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
	

	Library::Initialize(1920, 1080, Color(30,30,160,255),L"MyLib");
	Library::SetFramesPerSecond60(true);
	
	//�J�����͊e�V�[���Ɉړ����܂���


#pragma region ���\�[�X�ǂݍ���
	Texture::Load("Resources/Texture/cross.png", "cross");

	ModelData::Load("Resources/Obj/Ball/ball.obj", "ball");
#pragma endregion

#pragma region �}�l�[�W���[������

	CollisionFlag initFlag;
	initFlag.board = false;
	initFlag.box = false;
	initFlag.lineSegment = false;
	initFlag.plane = false;
	initFlag.ray = false;
	initFlag.sphere = true;
	GameObjectManager::GetInstance()->SetCollisionFlag3D(initFlag);
	GameObjectManager::GetInstance()->SetMouseCollisionFlag(false);
	GameObjectManager::GetInstance()->ReserveObjectArray(100);

	SceneManager::GetInstace()->SetStartScene(new HurikoGame());
#pragma endregion



	TextWrite::CreateFontData(/*L"HGP�޼��E"*/L"Arial", "test");

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