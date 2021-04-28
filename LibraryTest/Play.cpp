#include "Play.h"

#include"Matrix.h"
#include"Quaternion.h"
#include"ObjectManager.h"


#include"XInputManager.h"
#include"FbxModel.h"
#include"ObjModel.h"
std::unique_ptr<FbxModel> fbxModel = std::make_unique<FbxModel>();
Play::Play(){}


Play::~Play(){}

void Play::Initialize()
{
	fbxModel->LoadModel
	(
		"Resources/cube/cube.fbx",
		1
	);

}

Vector3 angle = 0;
void Play::Update()
{

	if (DirectInput::KeyState(DIK_W))
		angle.y += 3.0f;
	if (DirectInput::KeyState(DIK_S))
		angle.y -= 3.0f;
	if (DirectInput::KeyState(DIK_A))
		angle.x += 3.0f;
	if (DirectInput::KeyState(DIK_D))
		angle.x -= 3.0f;
	fbxModel->SetAngle(angle, 0);
}

void Play::Draw()
{
	fbxModel->Draw(0);
}

void Play::end()
{
}

Scene* Play::GetNextScene()
{
	return new Play();
}

void Play::LoadResources()
{
}

