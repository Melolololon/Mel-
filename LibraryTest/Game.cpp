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
#include"Input.h"

#include"Limit.h"

#include"ImguiManager.h"

Game::Game() {}

Game::~Game() {}
Game* Game::GetInstance()
{
	static Game game;
	return &game;
}


void Game::Run()
{
	//MelLib::ImguiManager::GetInstance()->SetReleaseDrawFlag(false);

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


#pragma region リソース読み込み

	MelLib::ModelData::Load("Resources/Obj/Ball/ball.obj", false,"ball");
	MelLib::ModelData::Load("Resources/Obj/Box/box.obj", false,"box");
	//MelLib::ModelData::Load("Resources/PBRModels/SpiralPBR/SpiralPBR.fbx", false,"pbr");

	MelLib::Texture::Load("Resources/Texture/Tex3D/test_0.png","t");
	std::vector<std::string>path;
	for(int i = 0; i < 1;i++)
	{
		//path.push_back("Resources/Texture/Tex3D/Test_" + std::to_string(i) + ".png");
		path.push_back("Resources/Texture/Tex3D/test_" + std::to_string(i) + ".png");
	}
	MelLib::Texture3D::Load(path, "test");
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
	MelLib::GameObjectManager::GetInstance()->SetMouseCollisionFlag(false);
	MelLib::GameObjectManager::GetInstance()->ReserveObjectArray(100);

	MelLib::SceneManager::GetInstace()->SetStartScene(new Play());
#pragma endregion

	MelLib::TextWrite::CreateFontData(/*L"HGPｺﾞｼｯｸE"*/L"Arial", "test");

	MelLib::TextureFont::Load("Resources/Font/font.png", MelLib::Value2<UINT>(14, 7), "testFont");

}


void Game::Finalize()
{
	MelLib::Library::Finalize();//絶対に最後に書く
}
void Game::Update()
{
	MelLib::SceneManager::GetInstace()->Update();
}

void Game::Draw()
{
	MelLib::SceneManager::GetInstace()->Draw();
}