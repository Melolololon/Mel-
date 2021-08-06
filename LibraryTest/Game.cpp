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
		melLib::Library::LoopStartProcess();

		if (melLib::Input::KeyTrigger(DIK_SPACE))melLib::Library::EndFlagTrue();
		

		Update();
		Draw();
		melLib::Library::LoopEndProcess();


		if (melLib::Library::GetIsEnd())break;
		
	}

	Finalize();
}

void Game::Initialize()
{
	

	melLib::Library::Initialize(1280, 720, melLib::Color(30,30,160,255),L"MyLib");
	melLib::Library::SetFramesPerSecond60(true);
	
	//カメラは各シーンに移動しました


#pragma region リソース読み込み

	melLib::ModelData::Load("Resources/Obj/Ball/ball.obj", "ball");
	melLib::ModelData::Load("Resources/Obj/Box/box.obj", "box");
#pragma endregion

#pragma region マネージャー初期化

	/*CollisionFlag initFlag;
	initFlag.board = true;
	initFlag.box = true;
	initFlag.lineSegment = true;
	initFlag.plane = true;
	initFlag.ray = true;
	initFlag.sphere = true;
	GameObjectManager::GetInstance()->SetCollisionFlag3D(initFlag);*/
	melLib::GameObjectManager::GetInstance()->SetMouseCollisionFlag(false);
	melLib::GameObjectManager::GetInstance()->ReserveObjectArray(100);

	melLib::SceneManager::GetInstace()->SetStartScene(new Play());
#pragma endregion

	melLib::TextWrite::CreateFontData(/*L"HGPｺﾞｼｯｸE"*/L"Arial", "test");

	melLib::TextureFont::Load("Resources/Font/font.png", melLib::Value2<UINT>(14, 7), "testFont");

}


void Game::Finalize()
{
	melLib::Library::Finalize();//絶対に最後に書く
}
void Game::Update()
{
	melLib::SceneManager::GetInstace()->Update();
}

void Game::Draw()
{
	melLib::SceneManager::GetInstace()->Draw();
}