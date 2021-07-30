#include "HurikoGame.h"

#include"Matrix.h"
#include"Quaternion.h"
#include"GameObjectManager.h"


#include"XInputManager.h"
#include"FbxModel.h"
#include"ObjModel.h"
#include"PrimitiveModel.h"

#include"FrameTimer.h"

#include"Random.h"

#include"ModelData.h"
#include"ModelObject.h"

#include"Sound.h"
#include"SoundData.h"
#include"DirectionalLight.h"

#include"Values.h"
#include"TextWrite.h"

#include"Counter.h"

#include"LibMath.h"
#include"Physics.h"
#include"PhysicsTestObject.h"

#include"Player.h"
#include"Easing.h"

HurikoGame::GameState HurikoGame::gameState = HurikoGame::GameState::TITLE;
const Vector3 HurikoGame::FIELD_SIZE = Vector3(250 * 2, 0, 250 * 2);


HurikoGame::HurikoGame() {}


HurikoGame::~HurikoGame() {}


void HurikoGame::Initialize()
{
	//プレイヤー
	player = std::make_shared<Player>();
	GameObjectManager::GetInstance()->AddObject(player);

	///チューリングパターン
	tPattern.Initialize();

	//ばね
	springObjects[0] = std::make_shared<SpringTestObject>(nullptr);
	GameObjectManager::GetInstance()->AddObject(springObjects[0]);

	for (int i = 1; i < SPRING_OBJECT_NUM; i++)
	{
		springObjects[i] = std::make_shared<SpringTestObject>(springObjects[i - 1].get());
		GameObjectManager::GetInstance()->AddObject(springObjects[i]);
	}
	for (int i = 0; i < SPRING_OBJECT_NUM - 1; i++)
	{
		springObjects[i]->SetNextObject(springObjects[i + 1].get());
	}




	SpringTestObject::ResetScore();

	for(auto& s : wallSpr)
	{
		s = std::make_unique<Sprite3D>(Color(255, 0, 0, Color::ParToUChar(50)));
		s->SetScale(Vector2(FIELD_SIZE.x, 30));
	}

	const float WALL_Y = -3.0f;
	wallSpr[0]->SetPosition(Vector3(0, WALL_Y, FIELD_SIZE.z / 2));
	wallSpr[1]->SetPosition(Vector3(0, WALL_Y, -FIELD_SIZE.z / 2));
	wallSpr[2]->SetPosition(Vector3(FIELD_SIZE.x / 2, WALL_Y, 0));
	wallSpr[3]->SetPosition(Vector3(-FIELD_SIZE.x / 2, WALL_Y, 0));

	wallSpr[2]->SetAngle(Vector3(0,90, 0));
	wallSpr[3]->SetAngle(Vector3(0,90, 0));

	gameState = GameState::TITLE;

}

void HurikoGame::Update()
{

	tPattern.Update();


	GameObjectManager::GetInstance()->Update();

	switch (gameState)
	{
	case GameState::TITLE:
		if (Input::KeyTrigger(DIK_SPACE))
		{
			gameState = GameState::PLAY_PRE;
			gameTimer.SetStopFlag(false);
			gameTimer.SetNowTime(GAME_TIME + START_PRE_TIME);
			gameTimer.SetResetFlag(false);
			gameTimer.SetDecrementFlag(true);
		}

		break;

	case GameState::PLAY_PRE:
		if (gameTimer.GetNowTime() <= GAME_TIME)
		{
			gameState = GameState::PLAY;

		}
		break;

	case GameState::PLAY:
		if (gameTimer.GetNowTime() <= 0)gameState = GameState::RESULT;
		break;

	case GameState::RESULT:
		resultPushTimer.SetStopFlag(false);
		if (Input::KeyTrigger(DIK_SPACE)
			&& resultPushTimer.GetNowTime() >= 60 * 0.8)
		{
			isEnd = true;
		}

		gameTimer.SetStopFlag(true);
		
		if (resultPushTimer.GetNowTime() >= 60 * 0.8)resultPushTimer.SetStopFlag(true);
		
		break;
	}

	if (titleT <= 1.0f)
	{
		titleT += 0.0005f;
	}else
	{
		titleT = 1.0f;
	}

	if(Input::KeyTrigger(DIK_RETURN) && gameState != GameState::TITLE)
	{
		isEnd = true;
	}
}

void HurikoGame::Draw()
{
	tPattern.Draw();
	GameObjectManager::GetInstance()->Draw();
	const int SCORE = SpringTestObject::GetScore();
	Vector2 scoeNumPos = Vector2(311, 90);
	if(SCORE >= 10)
	{
		scoeNumPos -= Vector2(38, 0);
	}

	TextWrite::Draw(Vector2(0, 90),Color(255,255,255,255), L"SCORE", "test");
	TextWrite::Draw(scoeNumPos,Color(255,255,255,255), std::to_wstring(SCORE), "test");
	
	
	 int drawTime = gameTimer.GetNowTime();
	if (gameTimer.GetNowTime() > GAME_TIME)
	{
		drawTime = 60 * 30;
	}
	else if(gameTimer.GetNowTime() <= 0)
	{
		drawTime = 0;
	}

	Vector2 timeNumPos = Vector2(270, 0);
	if (gameTimer.GetNowTime() < 60 * 10)
	{
		timeNumPos += Vector2(40, 0);
	}
	TextWrite::Draw(0, Color(255, 255, 255, 255), L"TIME", "test");
	TextWrite::Draw(timeNumPos, Color(255, 255, 255, 255),  std::to_wstring(drawTime / 60), "test");



	static const Vector2 CENTER_TEXT_POS = Vector2(Library::GetWindowWidth() / 2, Library::GetWindowHeight() / 2);

	if (gameTimer.GetNowTime() > GAME_TIME)
	{
		TextWrite::Draw(CENTER_TEXT_POS - Vector2(110, 35), Color(255, 255, 255, 255), L"READY", "test");
	}
	else if (gameTimer.GetNowTime() < GAME_TIME && gameTimer.GetNowTime() > GAME_TIME - START_PRE_TIME)
	{
		TextWrite::Draw(CENTER_TEXT_POS - Vector2(65, 35), Color(255, 255, 255, 255), L"GO !", "test");
	}

	if (gameState == GameState::TITLE)
	{
		titlePos = Easing::EaseOut(titlePos, Vector2(titlePos.x, 450), titleT);
		TextWrite::Draw(titlePos, Color(255, 255, 255, 255), L"物理数学課題", "test");

		TextWrite::Draw(CENTER_TEXT_POS - Vector2(190, -150), Color(255, 255, 255, 255), L"SPACEで開始", "test");


		TextWrite::Draw(Vector2(0,600 - 20), Color(255, 255, 255, 255), L"WSAD 移動", "test");
		TextWrite::Draw(Vector2(0,700 - 20), Color(255, 255, 255, 255), L"↑↓←→ カメラ操作", "test");
		TextWrite::Draw(Vector2(0,800 - 20), Color(255, 255, 255, 255), L"SPACE 物体発射", "test");
		TextWrite::Draw(Vector2(0,900 - 20), Color(255, 255, 255, 255), L"F 視点切替", "test");
		TextWrite::Draw(Vector2(0,1000 - 20), Color(255, 255, 255, 255), L"物体を発射して浮いている物体に当てると得点", "test");
	}
	if(gameState == GameState::RESULT)
	{
		TextWrite::Draw(CENTER_TEXT_POS - Vector2(124, 35), Color(255, 255, 255, 255), L"FINISH !", "test");
		TextWrite::Draw(CENTER_TEXT_POS - Vector2(350, -150), Color(255, 255, 255, 255), L"SPACEでタイトルに戻る", "test");

	}

	if (gameState != GameState::TITLE) 
	{
		TextWrite::Draw(Vector2(1200, 980), Color(255, 255, 255, 255), L"ENTERでタイトルに戻る", "test");
	}



	for (auto& s : wallSpr)
	{
		s->Draw();
	}

	if (gameState != GameState::TITLE) 
	{
		player->DrawCross();
	}
}

void HurikoGame::Finitialize()
{
	GameObjectManager::GetInstance()->AllEraseObject();
}

Scene* HurikoGame::GetNextScene()
{
	return new HurikoGame();
}

void HurikoGame::LoadResources()
{
}

