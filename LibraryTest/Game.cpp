#include "Game.h"
#include<Library.h>
#include"SceneManager.h"
#include"GameObjectManager.h"

#include"Camera.h"
#include"ModelData.h"
#include"TextWrite.h"
#include"Texture.h"
#include"TextureFont.h"
#include"Input.h"

#include"Limit.h"

#include"ImguiManager.h"
#include "ErrorProcess.h"

#include"SceneEditer.h"
#include"Player.h"

Game::Game() {}

Game::~Game() {}
Game* Game::GetInstance()
{
	static Game game;
	return &game;
}


void Game::Run()
{

	MelLib::ImguiManager::GetInstance()->SetReleaseDrawFlag(true);

	Initialize();

	while (1)
	{
		MelLib::Library::LoopStartProcess();

		if (MelLib::Input::KeyTrigger(DIK_ESCAPE))MelLib::Library::EndFlagTrue();
		if (MelLib::Library::GetIsEnd())break;
		

		Update();
		Draw();
		MelLib::Library::LoopEndProcess();

	}

	Finalize();
}

void Game::Initialize()
{
	

	MelLib::Library::Initialize(1280, 720, MelLib::Color(30,30,160,255),L"MELLib");
	MelLib::Library::SetFramesPerSecond60(true);
	
	//カメラは各シーンに移動しました



#pragma region マネージャー初期化

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

	//MelLib::SceneManager::GetInstance()->SetStartScene(new Play());
#pragma endregion

	MelLib::TextWrite::CreateFontData(/*L"HGPｺﾞｼｯｸE"*/L"Arial",2, "test");

	MelLib::TextureFont::Load("Resources/Font/font.png", MelLib::Value2<UINT>(14, 7), "testFont");

	// 登録
	MelLib::SceneEditer::GetInstance()->RegisterObject(std::make_shared<Player>(),"Actor");
	int z = 0;

}


void Game::Finalize()
{
	MelLib::Library::Finalize();//絶対に最後に書く
}
void Game::Update()
{
	//MelLib::SceneManager::GetInstance()->Update();
	MelLib::SceneEditer::GetInstance()->Update();

	MelLib::GameObjectManager::GetInstance()->Update();
}

void Game::Draw()
{
	//MelLib::SceneManager::GetInstance()->Draw();
	MelLib::SceneEditer::GetInstance()->Draw();

	MelLib::GameObjectManager::GetInstance()->Draw();
}