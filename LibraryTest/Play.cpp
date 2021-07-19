#include "Play.h"

#include"Matrix.h"
#include"Quaternion.h"
#include"ObjectManager.h"


#include"XInputManager.h"
#include"FbxModel.h"
#include"ObjModel.h"
#include"PrimitiveModel.h"

#include"FreamTimer.h"

#include"Random.h"

#include"ModelData.h"
#include"ModelObject.h"

#include"Sound.h"
#include"SoundData.h"
#include"DirectionalLight.h"

#include"Values.h"
#include"TextWrite.h"
Play::Play(){}


Play::~Play(){}


void Play::Initialize()
{
	ModelData::Load("Resources/PBRModels/SpiralPBR/SpiralPBR.fbx","model");
	//ModelData::Load("Resources/boneTest/boneTest.fbx","model");
	ModelObject::Create(ModelData::Get("model"), nullptr, "model");
	ModelObject::Get("model")->SetAnimationFlag(true);

	//SoundData::Load("Resources/Sound/EnemyLostSE.wav", "test");

	//PlaySoundData data;
	//data.volume = 100;
	//Sound::PlayLoadSound(SoundData::Get("test"), 1, data,"test");

	Value2<float> v1(4,4);
	Value2<float> v2(3,2);
	Value2<float>res = v1 * v2;
	

	int z = 0;

	
	TextWrite::CreateFontData("test");
}
int testT = 0;

Vector3 angle = 0;
void Play::Update()
{
	Vector3 pos1 = Vector3(0, 5, 0);
	Vector3 pos2 = Vector3(5, 0, 0);
	Vector3 pos3 = Vector3(0, 0, 0);
	Vector3 test = LibMath::CalcNormal(pos1, pos2, pos3);


	PlaySoundData data;

	data.volume = 100;

	//Sound::PlayLoadSound(SoundData::Get("test"),1, data );


	if(Input::KeyState(DIK_A))
	{
		angle.y += 3.0f;
		
	}
	if (Input::KeyState(DIK_D))
	{
		angle.y -= 3.0f;
	}
	if (Input::KeyState(DIK_W))
	{
		angle.x += 3.0f;

	}
	if (Input::KeyState(DIK_S))
	{
		angle.x -= 3.0f;
	}
	Camera::Get()->SetRotateCriteriaPosition(0);
	Camera::Get()->SetAngle(angle);
	Camera::Get()->SetRotatePoint(Camera::RotatePoint::ROTATE_POINT_TARGET_POSITION);
	Camera::Get()->SetCameraToTargetDistance(5.0f);
	DirectionalLight::Get().SetDirection(Vector3(0, -1, 0));

}

void Play::Draw()
{
	ModelObject::Get("model")->Draw();
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

