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
#include"Collision.h"
#include"RenderTarget.h"

void Play::CollisionTestInitialize()
{

#pragma region ���\�[�X�ǂݍ���

	MelLib::ModelData::Load("Resources/Obj/Ball/ball.obj", false, "ball");
	MelLib::ModelData::Load("Resources/Obj/Box/box.obj", false, "box");
	//MelLib::ModelData::Load("Resources/PBRModels/SpiralPBR/SpiralPBR.fbx", false,"pbr");

	MelLib::Texture::Load("Resources/Texture/Tex3D/test_0.png", "t");
	std::vector<std::string>path;
	for (int i = 0; i < 1; i++)
	{
		//path.push_back("Resources/Texture/Tex3D/Test_" + std::to_string(i) + ".png");
		path.push_back("Resources/Texture/Tex3D/test_" + std::to_string(i) + ".png");
	}
	MelLib::Texture3D::Load(path, "test");
#pragma endregion


	MelLib::CapsuleData cData;
	cData.SetRadius(2.0f);
	cData.GetRefSegment3DData().SetPosition(MelLib::Value2(MelLib::Vector3(0, 1, 0), MelLib::Vector3(0, 5, 0)));

	MelLib::BoardData bData;
	bData.SetPosition(MelLib::Vector3(0, 0, 0));
	bData.SetSize(MelLib::Vector2(4, 4));
	bData.SetAngle(MelLib::Vector3(90, 0, 0));

	bool re = MelLib::Collision::BoardAndCapsule(bData, nullptr, cData, nullptr);


	MelLib::FrustumData fData;
	fData.SetXYAngle(MelLib::Vector2(90, 10));
	fData.SetFar(10);
	fData.SetNear(0.0001f);
	fData.SetAngle(MelLib::Vector3(0, 0, 0));
	bool r = MelLib::Collision::PointAndFrustum
	(
		MelLib::Vector3(0, 0, 5),
		fData
	);



	MelLib::Vector2 pPos(0, 5);
	MelLib::CircularSectorData data;
	data.SetAngle(180);
	data.SetDirection(MelLib::Vector2(1, 0));
	data.GetRefCircleData().SetPosition(0);
	data.GetRefCircleData().SetRadius(3.0f);

	//3D�Ŏ��E�̏�������ꍇ�́A����̌��ʂƁA��ԉ����ꏊ(���a�̈ʒu)�̎��E�̍��������߁A
	//���S�ɋ߂Â��ɉ����ď���������΂����B
	bool res = MelLib::Collision::PointAndCircularSector(pPos, data);


	MelLib::Vector3 n = MelLib::LibMath::RotateVector3(MelLib::Vector3(0, 1, 0), MelLib::Vector3(0.7, 0, 0.7), 90);


	MelLib::Camera::Get()->SetRotateCriteriaPosition(MelLib::Vector3(0, 0, -30));

	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<CollisionTestObject>(0, true));
	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<CollisionTestObject>(MelLib::Vector3(-10, 0, 0), false));

	MelLib::Texture::Load("Resources/Texture/testTexture.png", "test");
	sprite2DTest = std::make_unique<MelLib::Sprite2D>(MelLib::Texture::Get("test"));
	//sprite2DTest->CreateSetColor(Color(255, 255, 255, 255));

	sprite2DTest->SetPosition(MelLib::Vector2(1280 / 2, 720 / 2));


	//�N�������邱��
	//���C�g�𕁒ʂɉ��Z����ƁA�E���̖ʂ�0,0,1�̃��C�g�݂̂̎����Â��Ȃ邩�猸�����Ȃ���
	MelLib::DirectionalLight::Create("test");

	MelLib::DirectionalLight::Get().SetDirection(MelLib::Vector3(0, 0, 1));
	MelLib::DirectionalLight::Get("test").SetDirection(MelLib::Vector3(0, -1, 0));

}
void Play::CollisionTestUpdate()
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


	if (MelLib::Input::KeyTrigger(DIK_1))
	{
		isEnd = true;
	}
}
void Play::CollisionTestDraw()
{

	MelLib::SpriteFont2D::GetInstance()->Draw(0, testScale, MelLib::SpriteFont2D::CharSequence::BESIDE, "A", MelLib::TextureFont::Get("testFont"));

	//sprite2DTest->SetDrawArea(0, MelLib::Vector2(64, 64));
	sprite2DTest->Draw();



	if (MelLib::Input::KeyState(DIK_Z))testScale.x -= 0.025f;
	if (MelLib::Input::KeyState(DIK_X))testScale.x += 0.025f;
}

void Play::RTTestInitialize()
{
	MelLib::RenderTarget::Create(MelLib::Color(255, 0, 0, 255), "test");
	rtTestObject.Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), nullptr);
}

void Play::RTTestUpdate()
{
}

void Play::RTTestDraw()
{
	rtTestObject.Draw("test");
}



Play::Play(){}


Play::~Play(){}


void Play::Initialize()
{
	RTTestInitialize();
}

void Play::Update()
{
	RTTestUpdate();
	MelLib::GameObjectManager::GetInstance()->Update();

	std::vector<MelLib::RenderTarget::RenderTargetDrawData>data(1);
	data[0].rt = MelLib::RenderTarget::Get("test");
	data[0].renderingRT = MelLib::RenderTarget::Get();
	MelLib::RenderTarget::SetRenderingRenderDrawData(data);
}

void Play::Draw()
{
	RTTestDraw();

	MelLib::GameObjectManager::GetInstance()->Draw();
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

