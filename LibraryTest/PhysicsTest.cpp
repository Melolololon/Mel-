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

#include"Physics.h"
#include"PhysicsTestObject.h"
PhysicsTest::PhysicsTest() {}


PhysicsTest::~PhysicsTest() {}


void PhysicsTest::Initialize()
{
#pragma region ƒ‚ƒfƒ‹“Ç‚Ýž‚Ý
	ModelData::Load("Resources/Obj/Ball/ball.obj", "ball");
#pragma endregion
	
	Camera::Get()->SetRotateCriteriaPosition(Vector3(0, 0, -100));

	GameObjectManager::GetInstance()->AddObject(std::make_shared<PhysicsTestObject>(Vector3(-40, 40, 0), Vector3(0.3, -0.3, 0)));
	GameObjectManager::GetInstance()->AddObject(std::make_shared<PhysicsTestObject>(Vector3(40, 0, 0), Vector3(-0.3, 0, 0)));
	GameObjectManager::GetInstance()->AddObject(std::make_shared<PhysicsTestObject>(Vector3(0, 41, 0), Vector3(0, -0.3, 0)));
	GameObjectManager::GetInstance()->AddObject(std::make_shared<PhysicsTestObject>(Vector3(0, -38, 0), Vector3(0, 0.3, 0)));

	//tPattern.Initialize();



}

void PhysicsTest::Update()
{
	if (Input::KeyState(DIK_Z) || Input::KeyTrigger(DIK_Z)) 
	{
		//tPattern.Update();
	}
	GameObjectManager::GetInstance()->Update();
}

void PhysicsTest::Draw()
{
	//tPattern.Draw();
	GameObjectManager::GetInstance()->Draw();
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

