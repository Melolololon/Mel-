#include "Play.h"

#include"CollisionTestObject.h"

#include"GameObjectManager.h"
#include"Camera.h"
#include"SpriteFont2D.h"
#include"TextureFont.h"
#include"DirectionalLight.h"

Play::Play(){}


Play::~Play(){}


void Play::Initialize()
{
	MelLib::Camera::Get()->SetRotateCriteriaPosition(MelLib::Vector3(0, 0, -30));

	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<CollisionTestObject>(0,true));
	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<CollisionTestObject>(MelLib::Vector3(-10, 0, 0), false));

	MelLib::Texture::Load("Resources/Texture/testTexture.png", "test");
	sprite2DTest = std::make_unique<MelLib::Sprite2D>(MelLib::Texture::Get("test"));
	//sprite2DTest->CreateSetColor(Color(255, 255, 255, 255));
	
	sprite2DTest->SetPosition(MelLib::Vector2(1280 / 2, 720 / 2));


	//起きたらやること
	//ライトを普通に加算すると、右側の面が0,0,1のライトのみの時より暗くなるから減衰をなくす
	MelLib::DirectionalLight::Create("test");
	
	MelLib::DirectionalLight::Get().SetDirection(MelLib::Vector3(0, 0, 1));
	MelLib::DirectionalLight::Get("test").SetDirection(MelLib::Vector3(0, -1, 0));
	
}

void Play::Update()
{
	/*if(MelLib::Input::KeyState(DIK_R))
	{

		MelLib::DirectionalLight::Get("test").SetDirection(MelLib::Vector3(1, 0, 0));
	}
	else
	{
		MelLib::DirectionalLight::Get("test").SetDirection(MelLib::Vector3(0, 0, 0));
	}*/

	if (MelLib::Input::KeyState(DIK_Q))scale -= 0.05;
	if (MelLib::Input::KeyState(DIK_E))scale += 0.05;
	sprite2DTest->SetScale(scale);

	MelLib::GameObjectManager::GetInstance()->Update();

	if (MelLib::Input::KeyTrigger(DIK_1))
	{
		isEnd = true;
	}
}

MelLib::Vector2 testScale = 1;
void Play::Draw()
{
	MelLib::GameObjectManager::GetInstance()->Draw();
	
	MelLib::SpriteFont2D::GetInstance()->Draw(0, testScale, MelLib::SpriteFont2D::CharSequence::BESIDE, "A", MelLib::TextureFont::Get("testFont"));

	//sprite2DTest->SetDrawArea(0, MelLib::Vector2(64, 64));
	sprite2DTest->Draw();
	


	if (MelLib::Input::KeyState(DIK_Z))testScale.x -= 0.025f;
	if (MelLib::Input::KeyState(DIK_X))testScale.x += 0.025f;

}

void Play::Finalize()
{
}

MelLib::Scene* Play::GetNextScene()
{
	return new Play();
}

void Play::LoadResources()
{
}

