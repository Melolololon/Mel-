#include "Play.h"

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

#include"TextWrite.h"

Play::Play(){}


Play::~Play(){}


void Play::Initialize()
{
	Camera::Get()->SetRotateCriteriaPosition(Vector3(0, 0, -5));
	/*Vector3 v = Physics::CalcCoefficientOfRestitution
	(
		Value2<Vector3>()
	);*/


	ModelData::Load("Resources/PBRModels/SpiralPBR/SpiralPBR.fbx","model");
	//ModelData::Load("Resources/boneTest/boneTest.fbx","model");

	pbrModel = std::make_unique<ModelObject>(ModelData::Get("model"), nullptr);



	TextWrite::CreateFontData(L"HGPºÞ¼¯¸E","test");



	SoundData::Load("Resources/Sound/StageSelect.wav", "test");
	PlaySoundData data;
	data.volume = 30;
	Sound::PlayLoadSound(SoundData::Get("test"), Sound::LOOP_INFINITY, data,"test");
}

void Play::Update()
{
	if (Input::KeyTrigger(DIK_SPACE)) 
	{
		Sound::Get("test")->ResetSound();
	}


	Vector3 pos1 = Vector3(0, 5, 0);
	Vector3 pos2 = Vector3(5, 0, 0);
	Vector3 pos3 = Vector3(0, 0, 0);
	Vector3 test = LibMath::CalcNormal(pos1, pos2, pos3);


	PlaySoundData data;

	data.volume = 100;

	//Sound::PlayLoadSound(SoundData::Get("test"),1, data );


	if(Input::KeyState(DIK_A))
	{
		modelAngle.y += 3.0f;
		
	}
	if (Input::KeyState(DIK_D))
	{
		modelAngle.y -= 3.0f;
	}
	if (Input::KeyState(DIK_W))
	{
		modelAngle.x += 3.0f;

	}
	if (Input::KeyState(DIK_S))
	{
		modelAngle.x -= 3.0f;
	}
	/*Camera::Get()->SetRotateCriteriaPosition(0);
	Camera::Get()->SetAngle(angle);
	Camera::Get()->SetRotatePoint(Camera::RotatePoint::ROTATE_POINT_TARGET_POSITION);
	Camera::Get()->SetCameraToTargetDistance(5.0f);*/
	DirectionalLight::Get().SetDirection(Vector3(0, -1, 0));


	pbrModel->SetAngle(modelAngle);

	Vector3 v1 = -7;
	Vector3 v2 = 1;
	Vector3 v3 = (v1 + v2).Abs();
	int z = 0;


	GameObjectManager::GetInstance()->Update();
}

int count = 0;
void Play::Draw()
{
	pbrModel->Draw();

	GameObjectManager::GetInstance()->Draw();

	//TextWrite::Draw(L"1234567890_0987654321_3142536475869708", "test");
	count++;		
	TextWrite::Draw(Vector2(0, 600 - 20), Color(255, 255, 255, 255), L"WSAD ˆÚ“®", "test");
}

void Play::Finitialize()
{
}

Scene* Play::GetNextScene()
{
	return new Play();
}

void Play::LoadResources()
{
}

