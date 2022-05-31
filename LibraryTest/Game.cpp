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
		

		Update();
		Draw();
		MelLib::Library::LoopEndProcess();

	}

	Finalize();
}

MelLib::Sprite2D sprite;
MelLib::ModelObject obj;
MelLib::ModelObject rayObj[10];
MelLib::RayData ray;
MelLib::OBBData obb;
MelLib::SphereData sphere;
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

	//for (int i = 0; i < 1; i++)MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<TestObject>());
	//MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<Player>());
	//MelLib::SceneManager::GetInstance()->SetStartScene(new Play());
#pragma endregion



	sprite.Create(MelLib::Color(255,255,0,255));
	sprite.SetScale(MelLib::Vector2(256, 512));


	MelLib::GuiValueManager::GetInstance()->Initialize();

	//MelLib::Camera::Get()->SetAngle(MelLib::Vector3(90, 0, 0));
	//MelLib::Camera::Get()->SetRotateCriteriaPosition(MelLib::Vector3(0,10,0));


	
	ray.SetPosition(MelLib::Vector3(-5, 0, -5));
	ray.SetDirection(MelLib::Vector3(1, 0, 1).Normalize());

	obb.SetPosition(MelLib::Vector3(0, 0, 0));
	obb.SetSize(MelLib::Vector3(5, 3,5));
	obb.SetAngle(MelLib::Vector3(0, 0, 0));

	sphere.SetPosition(0);
	sphere.SetRadius(1.0f);

	obj.Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));
	for (int i = 0; i < _countof(rayObj); i++) {

		rayObj[i].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));
		rayObj[i].SetScale(0.25);
	}

	obj.SetScale(sphere.GetRadius() * 2);
	obj.SetAngle(0);
	obj.SetPosition(sphere.GetPosition());

	MelLib::Camera::Get()->SetRotateCriteriaPosition(MelLib::Vector3(0, 20, 0));
	MelLib::Camera::Get()->SetAngle(MelLib::Vector3(90, 0, 0));
}


void Game::Finalize()
{
	MelLib::Library::Finalize();//絶対に最後に書く
}

MelLib::Vector3 rayAngle = 0;

void Game::Update()
{
	if (MelLib::Input::KeyState(DIK_W))
	{
		sphere.SetPosition({ sphere.GetPosition().x,0, sphere.GetPosition().z + 0.3f });
	}
	if (MelLib::Input::KeyState(DIK_S))sphere.SetPosition({ sphere.GetPosition().x,0, sphere.GetPosition().z - 0.3f });
	if (MelLib::Input::KeyState(DIK_A))sphere.SetPosition({ sphere.GetPosition().x - 0.3f,0, sphere.GetPosition().z });
	if (MelLib::Input::KeyState(DIK_D))sphere.SetPosition({sphere.GetPosition().x + 0.3f, 0,  sphere.GetPosition().z });
	obj.SetPosition(sphere.GetPosition());

	MelLib::RayData rotRay = ray;

	if (MelLib::Input::KeyState(DIK_T))
	{
		ray.SetPosition({ ray.GetPosition().x,0, ray.GetPosition().z + 0.3f });
	}
	if (MelLib::Input::KeyState(DIK_G))ray.SetPosition({ ray.GetPosition().x,0, ray.GetPosition().z - 0.3f });
	if (MelLib::Input::KeyState(DIK_F))ray.SetPosition({ ray.GetPosition().x - 0.3f,0, ray.GetPosition().z });
	if (MelLib::Input::KeyState(DIK_H))ray.SetPosition({ ray.GetPosition().x + 0.3f, 0,  ray.GetPosition().z });


	MelLib::Vector3 rotRayPos = ray.GetPosition();
	rotRayPos -= obb.GetPosition();
	rotRayPos = MelLib::LibMath::RotateZXYVector3(rotRayPos, rayAngle * -1);
	rotRayPos += obb.GetPosition();
	//rotRay.SetPosition(rotRayPos);

	// ベクトルを回転
	rotRay.SetDirection(MelLib::LibMath::RotateZXYVector3(ray.GetDirection(), rayAngle * -1));
	for (int i = 0; i < _countof(rayObj); i++)rayObj[i].SetPosition(rotRay.GetPosition() + rotRay.GetDirection() * i);



	obj.SetMulColor(MelLib::Color(255, 255, 255, 255));

	const MelLib::Vector3 ADD_ANGLE = MelLib::Vector3(0, 3, 0);
	if (MelLib::Input::KeyState(DIK_LEFT))
	{
		//obb.SetAngle(obb.GetAngle() + ADD_ANGLE);
		//obj.SetAngle(obb.GetAngle());
		rayAngle += ADD_ANGLE;
	}
	if (MelLib::Input::KeyState(DIK_RIGHT))
	{
		//obb.SetAngle(obb.GetAngle() - ADD_ANGLE);
		//obj.SetAngle(obb.GetAngle());

		rayAngle -= ADD_ANGLE;
	}

	MelLib::RayCalcResult re;
	bool flag = MelLib::Collision::SphereAndRay(sphere,nullptr, rotRay, &re);
	if (flag) 
	{
		obj.SetMulColor(MelLib::Color(0, 0, 255, 255));
	}
	//MelLib::SceneManager::GetInstance()->Update();
	MelLib::SceneEditer::GetInstance()->Update();

	MelLib::GameObjectManager::GetInstance()->Update();
	MelLib::GuiValueManager::GetInstance()->Update();


}

void Game::Draw()
{
	rayObj[0].SetMulColor(MelLib::Color(255, 0, 0, 0));
	for (int i = 0; i < _countof(rayObj); i++)rayObj[i].Draw();

	obj.Draw();

	//MelLib::SceneManager::GetInstance()->Draw();
	MelLib::SceneEditer::GetInstance()->Draw();

	MelLib::GameObjectManager::GetInstance()->Draw();

	sprite.Draw();
}