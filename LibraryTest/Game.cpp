#include "Game.h"
#include<Library.h>
#include"SceneManager.h"
#include"GameObjectManager.h"
#include"Play.h"
#include"Camera.h"
#include"ModelData.h"
#include"TextWrite.h"
#include"Texture.h"
#include"TextureFont.h"


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
		MelLib::Library::LoopStartProcess();

		if (MelLib::Input::KeyTrigger(DIK_ESCAPE))MelLib::Library::EndFlagTrue();
		
		

		Update();
		Draw();
		MelLib::Library::LoopEndProcess();


		if (MelLib::Library::GetIsEnd())break;
		
	}

	Finalize();
}

void Game::Initialize()
{
	

	MelLib::Library::Initialize(1280, 720, MelLib::Color(30,30,160,255),L"MELLib");
	MelLib::Library::SetFramesPerSecond60(true);
	
	//�J�����͊e�V�[���Ɉړ����܂���


#pragma region ���\�[�X�ǂݍ���

	MelLib::ModelData::Load("Resources/Obj/Ball/ball.obj", false,"ball");
	MelLib::ModelData::Load("Resources/Obj/Box/box.obj", false,"box");
#pragma endregion

#pragma region �}�l�[�W���[������

	/*CollisionFlag initFlag;
	initFlag.board = true;
	initFlag.box = true;
	initFlag.lineSegment = true;
	initFlag.plane = true;
	initFlag.ray = true;
	initFlag.sphere = true;
	GameObjectManager::GetInstance()->SetCollisionFlag3D(initFlag);*/
	MelLib::GameObjectManager::GetInstance()->SetMouseCollisionFlag(false);
	MelLib::GameObjectManager::GetInstance()->ReserveObjectArray(100);

	MelLib::SceneManager::GetInstace()->SetStartScene(new Play());
#pragma endregion

	MelLib::TextWrite::CreateFontData(/*L"HGP�޼��E"*/L"Arial", "test");

	MelLib::TextureFont::Load("Resources/Font/font.png", MelLib::Value2<UINT>(14, 7), "testFont");

}


void Game::Finalize()
{
	MelLib::Library::Finalize();//��΂ɍŌ�ɏ���
}
void Game::Update()
{
	MelLib::SceneManager::GetInstace()->Update();
}

void Game::Draw()
{
	MelLib::SceneManager::GetInstace()->Draw();
}