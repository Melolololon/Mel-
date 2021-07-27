#include "PhysicsTest.h"

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



PhysicsTest::PhysicsTest() {}


PhysicsTest::~PhysicsTest() {}


void PhysicsTest::Initialize()
{
#pragma region モデル読み込み
	ModelData::Load("Resources/Obj/Ball/ball.obj", "ball");
#pragma endregion
	
	Camera::Get()->SetRotateCriteriaPosition(Vector3(0, 0, -100));

	//反発
	//GameObjectManager::GetInstance()->AddObject(std::make_shared<PhysicsTestObject>(Vector3(-40, 40, 0), Vector3(0.3, -0.3, 0)));
	//GameObjectManager::GetInstance()->AddObject(std::make_shared<PhysicsTestObject>(Vector3(40, 0, 0), Vector3(-0.3, 0, 0)));
	//GameObjectManager::GetInstance()->AddObject(std::make_shared<PhysicsTestObject>(Vector3(0, 41, 0), Vector3(0, -0.3, 0)));
	//GameObjectManager::GetInstance()->AddObject(std::make_shared<PhysicsTestObject>(Vector3(0, -38, 0), Vector3(0, 0.3, 0)));

	//チューリングパターン
	//tPattern.Initialize();

	//ばね
	springObjects[0] = std::make_shared<SpringTestObject>(topPos, topRootPos);
	GameObjectManager::GetInstance()->AddObject(springObjects[0]);

	float dis = LibMath::CalcDistance3D(topPos, topRootPos);
	for (int i = 1; i < SPRING_OBJECT_NUM; i++) 
	{
		Vector3 rootPos = springObjects[i - 1]->GetPosition();
		springObjects[i] = std::make_shared<SpringTestObject>(rootPos - Vector3(0, dis, 0), rootPos);
		GameObjectManager::GetInstance()->AddObject(springObjects[i]);
	}

	TextWrite::CreateFontData(L"HGPｺﾞｼｯｸE","test");
}

void PhysicsTest::Update()
{
	if (Input::KeyState(DIK_Z) || Input::KeyTrigger(DIK_Z)) 
	{
		//tPattern.Update();
	}



	GameObjectManager::GetInstance()->Update();	
	

	if (Input::KeyState(DIK_W))topRootPos.y += 1.0f;
	if (Input::KeyState(DIK_S))topRootPos.y -= 1.0f;
	if (Input::KeyState(DIK_A))topRootPos.x -= 1.0f;
	if (Input::KeyState(DIK_D))topRootPos.x += 1.0f;
	springObjects[0]->SetRootPosition(topRootPos);
	for (int i = 1; i < SPRING_OBJECT_NUM; i++)
	{
		Vector3 rootPos = springObjects[i - 1]->GetPosition();
		springObjects[i]->SetRootPosition(rootPos);
	}
}

int num = 0;
void PhysicsTest::Draw()
{
	//tPattern.Draw();
	GameObjectManager::GetInstance()->Draw();

	TextWrite::Draw(std::to_wstring(num), "test");
	num++;
}

void PhysicsTest::Finitialize()
{
}

Scene* PhysicsTest::GetNextScene()
{
	return new PhysicsTest();
}

void PhysicsTest::LoadResources()
{
}

