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
#include<GuiValueManager.h>

#include"Player.h"
#include"TestObject.h"

#include"Collision.h"
#include"LibMath.h"

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
		
		//ImGui::ShowDemoWindow();

		Update();
		Draw();
		MelLib::Library::LoopEndProcess();

	}

	Finalize();
}

void Game::Initialize()
{

	MelLib::Library::Initialize(1920, 1080, MelLib::Color(30,30,160,255),L"MELLib");
	MelLib::Library::SetFramesPerSecond60(true);

	//カメラは各シーンに移動しました

	Player::LoadRes();
#pragma region マネージャー初期化

	/*CollisionFlag initFlag;
	initFlag.board = true;
	initFlag.box = true;
	initFlag.lineSegment = true;
	initFlag.plane = true;
	initFlag.ray = true;
	initFlag.sphere = true;
	GameObjectManager::GetInstance()->SetCollisionFlag3D(initFlag);*/
	MelLib::GameObjectManager::GetInstance()->SetMouseCollisionFlag(true);
	MelLib::GameObjectManager::GetInstance()->ReserveObjectArray(100);

	//for (int i = 0; i < 1; i++)MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<TestObject>());
    //MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<Player>());
    //MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<Player>());
	//MelLib::SceneManager::GetInstance()->SetStartScene(new Play());
#pragma endregion


	//MelLib::Camera::Get()->SetAngle(MelLib::Vector3(90, 0, 0));
	//MelLib::Camera::Get()->SetRotateCriteriaPosition(MelLib::Vector3(0,10,0));



	MelLib::Camera::Get()->SetRotateCriteriaPosition(MelLib::Vector3(10, 0, -1));
	MelLib::Camera::Get()->SetAngle(MelLib::Vector3(0, -90, 0));
	

	//// エディターオン
	MelLib::SceneEditer::GetInstance()->SetEditerFlag(true);
	MelLib::SceneEditer::GetInstance()->Initialize();

	// エディターに追加(Unityでいうプレハブ作成)
	MelLib::SceneEditer::GetInstance()->RegisterObject(std::make_shared<Player>(),"Actor");

	// シーン読み込みテスト
	MelLib::SceneEditer::GetInstance()->LoadEditData("_TestData");
}


void Game::Finalize()
{
	MelLib::Library::Finalize();//絶対に最後に書く
}


void Game::Update()
{

	//MelLib::SceneManager::GetInstance()->Update();
	MelLib::SceneEditer::GetInstance()->Update();
	//MelLib::GameObjectManager::GetInstance()->Update();
}

void Game::Draw()
{
	

	//MelLib::SceneManager::GetInstance()->Draw();
	MelLib::SceneEditer::GetInstance()->Draw();

	//MelLib::GameObjectManager::GetInstance()->Draw();

}