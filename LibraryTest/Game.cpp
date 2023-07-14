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

#include<Emitter3D.h>
#include"TestParticle.h"

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
	// これGuiValueManagerへ移していいかも
	MelLib::GuiOption::GetInstance()->SetGuiDataPath("GameData/GUID/");

	bool SET_EDITER_FLAG = true;

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

	MelLib::TextWrite::CreateFontData(L"Arial", 64.0f, "Arial");

	MelLib::Camera::Get()->SetRotateCriteriaPosition(MelLib::Vector3(0, 0, -5));
	MelLib::Camera::Get()->SetAngle(MelLib::Vector3(0, 0, 0));
	

	//// エディターオン
	MelLib::SceneEditer::GetInstance()->SetEditerFlag(SET_EDITER_FLAG);
	MelLib::SceneEditer::GetInstance()->SetReleaseEditFlag(SET_EDITER_FLAG);
	MelLib::ImguiManager::GetInstance()->SetReleaseDrawFlag(SET_EDITER_FLAG);
	MelLib::SceneEditer::GetInstance()->Initialize();

	// エディターに追加(Unityでいうプレハブ作成)
	MelLib::SceneEditer::GetInstance()->RegisterObject(std::make_shared<Player>(),"Actor");
	/*MelLib::SceneEditer::GetInstance()->RegisterObject(std::make_shared<TestObject>(), "TestActor"); */
	//MelLib::SceneEditer::GetInstance()->RegisterObject(std::make_shared<TestObject>(),"TestActor2");

	// シーン読み込みテスト
	//MelLib::SceneEditer::GetInstance()->LoadEditData("BoxParty");



	// パーティクルテスト
	TestParticle testPar;
	std::shared_ptr<MelLib::Emitter3D> emitterTest = std::make_shared<MelLib::Emitter3D>(testPar,true, 10, 0, 0, "Test");
	emitterTest->SetPosition({ 4,0,0 });
	MelLib::GameObjectManager::GetInstance()->AddObject(emitterTest);
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

	if (MelLib::Input::KeyTrigger(DIK_1)) 
	{
		MelLib::Input::SetDrawCursorFlag(!MelLib::Input::GetDrawCursorFlag());
	}
}

void Game::Draw()
{
	//MelLib::SceneManager::GetInstance()->Draw();
	MelLib::SceneEditer::GetInstance()->Draw();

	MelLib::GameObjectManager::GetInstance()->Draw();

}