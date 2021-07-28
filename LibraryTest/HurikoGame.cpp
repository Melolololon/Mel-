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

HurikoGame::HurikoGame() {}


HurikoGame::~HurikoGame() {}


void HurikoGame::Initialize()
{
	//プレイヤー
	player = std::make_shared<Player>();
	GameObjectManager::GetInstance()->AddObject(player);

	///チューリングパターン
	//tPattern.Initialize();

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

	addTimer.SetStopFlag(false);
	addTimer.SetMaxTime(60 * 0.8);
	addTimer.SetResetFlag(true);

	SpringTestObject::ResetScore();
}

void HurikoGame::Update()
{



	//if(addTimer.GetSameAsMaxFlag())
	//{

	//	//反発
	//	GameObjectManager::GetInstance()->AddObject(std::make_shared<PhysicsTestObject>(Vector3(40, 0, 0), Vector3(-0.02, 0, 0)));
	//}


	//tPattern.Update();


	GameObjectManager::GetInstance()->Update();


	
	/*for (int i = 0; i < SPRING_OBJECT_NUM; i++)
	{
		springObjects[i]->CalcSpring();
	}*/

	//Camera::Get()->SetRotateCriteriaPosition( Vector3(0, 0, -100));
	//Camera::Get()->SetRotatePoint(Camera::RotatePoint::ROTATE_POINT_TARGET_POSITION);
	//Camera::Get()->SetCameraToTargetDistance(100.0f);

}

int num = 0;
void HurikoGame::Draw()
{
	//tPattern.Draw();
	GameObjectManager::GetInstance()->Draw();

	TextWrite::Draw(std::to_wstring(num), "test");
	num++;
	player->DrawCross();
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

