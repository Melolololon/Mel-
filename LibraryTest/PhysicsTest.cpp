//#include "PhysicsTest.h"
//
//#include"Matrix.h"
//#include"Quaternion.h"
//#include"GameObjectManager.h"
//
//
//#include"XInputManager.h"
//#include"FbxModel.h"
//#include"ObjModel.h"
//#include"PrimitiveModel.h"
//
//#include"FrameTimer.h"
//
//#include"Random.h"
//
//#include"ModelData.h"
//#include"ModelObject.h"
//
//#include"Sound.h"
//#include"SoundData.h"
//#include"DirectionalLight.h"
//
//#include"Values.h"
//#include"TextWrite.h"
//
//#include"Counter.h"
//
//#include"LibMath.h"
//#include"Physics.h"
//#include"PhysicsTestObject.h"
//
//#include"Player.h"
//
//PhysicsTest::PhysicsTest() {}
//
//
//PhysicsTest::~PhysicsTest() {}
//
//
//void PhysicsTest::Initialize()
//{
//	//プレイヤー
//	GameObjectManager::GetInstance()->AddObject(std::make_shared<Player>());
//
//	///チューリングパターン
//	//tPattern.Initialize();
//
//	//ばね
//	springObjects[0] = std::make_shared<SpringTestObject>(nullptr);
//	GameObjectManager::GetInstance()->AddObject(springObjects[0]);
//
//	float dis = LibMath::CalcDistance3D(topPos, topRootPos);
//	for (int i = 1; i < SPRING_OBJECT_NUM; i++) 
//	{
//		Vector3 rootPos = springObjects[i - 1]->GetPosition();
//		springObjects[i] = std::make_shared<SpringTestObject>(rootPos - Vector3(0, dis, 0), rootPos);
//		GameObjectManager::GetInstance()->AddObject(springObjects[i]);
//	}
//
//	addTimer.SetStopFlag(false);
//	addTimer.SetMaxTime(60 * 0.8);
//	addTimer.SetResetFlag(true);
//}
//
//void PhysicsTest::Update()
//{
//
//	
//
//	//if(addTimer.GetSameAsMaxFlag())
//	//{
//
//	//	//反発
//	//	GameObjectManager::GetInstance()->AddObject(std::make_shared<PhysicsTestObject>(Vector3(40, 0, 0), Vector3(-0.02, 0, 0)));
//	//}
//
//
//	//tPattern.Update();
//
//
//	GameObjectManager::GetInstance()->Update();	
//	
//
//	/*if (Input::KeyState(DIK_W))topRootPos.y += 1.0f;
//	if (Input::KeyState(DIK_S))topRootPos.y -= 1.0f;
//	if (Input::KeyState(DIK_A))topRootPos.x -= 1.0f;
//	if (Input::KeyState(DIK_D))topRootPos.x += 1.0f;*/
//
//	springObjects[0]->CalcSpring();
//	springObjects[0]->SetRootPosition(topRootPos);
//	for (int i = 1; i < SPRING_OBJECT_NUM; i++)
//	{
//		springObjects[i]->CalcSpring();
//		Vector3 rootPos = springObjects[i - 1]->GetPosition();
//		springObjects[i]->SetRootPosition(rootPos);
//	}
//
//
//}
//
//void PhysicsTest::Draw()
//{
//	//tPattern.Draw();
//	GameObjectManager::GetInstance()->Draw();
//
//
//
//}
//
//void PhysicsTest::Finitialize()
//{
//}
//
//Scene* PhysicsTest::GetNextScene()
//{
//	return new PhysicsTest();
//}
//
//void PhysicsTest::LoadResources()
//{
//}
//
