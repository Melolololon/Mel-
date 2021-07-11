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

Play::Play(){}


Play::~Play(){}


void Play::Initialize()
{
	ModelData::Load("Resources/boneTest/boneTest.fbx","model");
	ModelObject::Create(ModelData::Get("model"), nullptr, "model");
	ModelObject::Get("model")->SetAnimationFlag(true);
}
void Play::Update()
{


	
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

