#include "Play.h"

#include"CollisionTestObject.h"

#include"GameObjectManager.h"
#include"Camera.h"
#include"SpriteFont2D.h"
#include"TextureFont.h"
#include"DirectionalLight.h"
#include"Input.h"
#include"LibMath.h"
#include"ImguiManager.h"
Play::Play(){}


Play::~Play(){}

#include"Collision.h"

void Play::Initialize()
{
	MelLib::CapsuleData cData;
	cData.SetRadius(2.0f);
	cData.GetRefSegment3DData().SetPosition(MelLib::Value2(MelLib::Vector3(0, 1, 0), MelLib::Vector3(0, 5, 0)));

	MelLib::BoardData bData;
	bData.SetPosition(MelLib::Vector3(0,0,0));
	bData.SetSize(MelLib::Vector2(4,4));
	bData.SetAngle(MelLib::Vector3(90, 0, 0));
	
	bool re = MelLib::Collision::BoardAndCapsule(bData, nullptr, cData, nullptr);


	MelLib::FrustumData fData;
	fData.SetXYAngle(MelLib::Vector2(90,10));
	fData.SetFar(10);
	fData.SetNear(0.0001f);
	fData.SetAngle(MelLib::Vector3(0, 0, 0));
	bool r = MelLib::Collision::PointAndFrustum
	(
		MelLib::Vector3(0,0,5),
		fData
	);



	MelLib::Vector2 pPos(0, 5);
	MelLib::CircularSectorData data;
	data.SetAngle(180);
	data.SetDirection(MelLib::Vector2(1, 0));
	data.GetRefCircleData().SetPosition(0);
	data.GetRefCircleData().SetRadius(3.0f);

	//3Dで視界の処理する場合は、これの結果と、一番遠い場所(半径の位置)の視界の高さを求め、
	//中心に近づくに応じて小さくすればいい。
	bool res = MelLib::Collision::PointAndCircularSector(pPos, data);


	MelLib::Vector3 n = MelLib::LibMath::RotateVector3(MelLib::Vector3(0, 1, 0), MelLib::Vector3(0.7, 0,0.7), 90);


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

bool flag = false;
void Play::Update()
{
	MelLib::ImguiManager::GetInstance()->BeginDrawWindow
	(
		"TestWindow",
		MelLib::Vector2(40, 40),
		MelLib::Vector2(300, 400)
	);

	ImGui::Checkbox("TestCheckBox", &flag);

	MelLib::ImguiManager::GetInstance()->EndDrawWindow();


	/*MelLib::ImguiManager::GetInstance()->DrawWindow
	(
		"eryu6tyutyfu",
		MelLib::Vector2(600, 50),
		MelLib::Vector2(300, 400)
	);*/

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

