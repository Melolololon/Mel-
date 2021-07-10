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

#include"Input.h"

Play::Play(){}


Play::~Play(){}

Vector3 angle = 0;
Vector3 mAngle = 0;

void Play::Initialize()
{ 
	bool result = false;
	result = ModelData::Load("Resources/Obj/tank_body.obj","model1");
	result = ModelObject::Create(ModelData::Get("model1"), nullptr, "model1");

	result = ModelData::Load("Resources/boneTest/boneTest.fbx", "model2");
	result = ModelObject::Create(ModelData::Get("model2"), nullptr, "model2");

}

std::string getStr = "";
void Play::Update()
{
	if (Input::KeyState(DIK_SPACE))
	{
  		int z = 0;
	}
	getStr += Input::GetTriggerKeyChars();
	
}

void Play::Draw()
{

	ModelObject::Get("model1")->Draw();

	ModelObject::Get("model2")->SetAnimationFlag(true);
	ModelObject::Get("model2")->SetAnimationSpeedMagnification(10);
	ModelObject::Get("model2")->Draw();
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

