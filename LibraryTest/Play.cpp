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

Play::Play(){}


Play::~Play(){}


void Play::Initialize()
{
	ModelData::Load("Resources/PBRModels/SpherePBR/spherePBR.fbx","model");
	//ModelData::Load("Resources/boneTest/boneTest.fbx","model");
	ModelObject::Create(ModelData::Get("model"), nullptr, "model");
	ModelObject::Get("model")->SetAnimationFlag(true);

	//SoundData::Load("Resources/Sound/EnemyLostSE.wav", "test");

	//PlaySoundData data;
	//data.volume = 100;
	//Sound::PlayLoadSound(SoundData::Get("test"), 1, data,"test");
}
int testT = 0;

float angle = 0;
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
		angle += 3.0f;
		
	}
	if (Input::KeyState(DIK_D))
	{
		angle -= 3.0f;
	}
	Camera::Get()->SetRotateCriteriaPosition(0);
	Camera::Get()->SetAngle(Vector3(0, angle, 0));
	Camera::Get()->SetRotatePoint(Camera::RotatePoint::ROTATE_POINT_TARGET_POSITION);
	Camera::Get()->SetCameraToTargetDistance(5.0f);

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

