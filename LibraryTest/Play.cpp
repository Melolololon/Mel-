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

Play::Play(){}


Play::~Play(){}

Vector3 angle = 0;
Vector3 mAngle = 0;

void Play::Initialize()
{
	ModelData::Load("Resources/Obj/tank_body.obj","model1");

}

void Play::Update()
{

}

void Play::Draw()
{
	
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

