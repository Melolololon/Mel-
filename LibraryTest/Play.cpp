#include "Play.h"

#include"CollisionTestObject.h"

#include"MeshCatObject.h"

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

#include<Emitter3D.h>
#include"TestParticle.h"

void Play::CollisionTestInitialize()
{

#pragma region リソース読み込み

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

	//3Dで視界の処理する場合は、これの結果と、一番遠い場所(半径の位置)の視界の高さを求め、
	//中心に近づくに応じて小さくすればいい。
	bool res = MelLib::Collision::PointAndCircularSector(pPos, data);


	MelLib::Vector3 n = MelLib::LibMath::RotateVector3(MelLib::Vector3(0, 1, 0), MelLib::Vector3(0.7, 0, 0.7), 90);


	MelLib::Camera::Get()->SetRotateCriteriaPosition(MelLib::Vector3(0, 0, -30));

	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<CollisionTestObject>(0, true));
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

void Play::CatInitialize()
{
	//rtTestObject.Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOARD), nullptr);

	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<MeshCatObject>(0,nullptr));


	MeshCatObject::SetPlaneData(&planeData);
}

void Play::CatTestUpdate()
{	//std::vector<MelLib::RenderTarget::RenderTargetDrawData>data(2);
	//data[0].rt = MelLib::RenderTarget::Get("test");
	//data[0].renderingRT = MelLib::RenderTarget::Get();

	//data[1].rt = MelLib::RenderTarget::Get("test2");
	//data[1].renderingRT = MelLib::RenderTarget::Get();

	//MelLib::RenderTarget::SetRenderingRenderDrawData(data);


	MelLib::ImguiManager::GetInstance()->BeginDrawWindow("Plane");

	MelLib::Vector3 pos = planeData.GetPosition();
	MelLib::ImguiManager::GetInstance()->DrawSliderVector3("pos", pos, -5, 5);


	MelLib::Vector3 normal = planeData.GetNormal();
	MelLib::ImguiManager::GetInstance()->DrawSliderVector3("normal", normal, -1, 1);

	
	MelLib::ImguiManager::GetInstance()->DrawCheckBox("normalize", isNormalize);
	if (isNormalize)
	{
		normal = normal.Normalize();
		isNormalize = false;
	}

	MelLib::ImguiManager::GetInstance()->DrawCheckBox("reset", isReset);
	if (isReset)
	{
		normal = 0;
		isReset = false;
	}


	MelLib::ImguiManager::GetInstance()->EndDrawWindow();

	planeData.SetNormal(normal);
	planeData.SetPosition(pos);
	



	if(MelLib::Input::KeyTrigger(DIK_Z))
	{
		isEnd = true;
	}


	if(MelLib::Input::KeyState(DIK_UP))
	{
		angle.x -= 2;
	}
	if (MelLib::Input::KeyState(DIK_DOWN))
	{
		angle.x += 2;
	}
	if (MelLib::Input::KeyState(DIK_LEFT))
	{
		angle.y -= 2;
	}
	if (MelLib::Input::KeyState(DIK_RIGHT))
	{
		angle.y += 2;
	}
	MelLib::Camera::Get()->SetAngle(angle);
	MelLib::Camera::Get()->SetRotatePoint(MelLib::Camera::RotatePoint::ROTATE_POINT_TARGET_POSITION);
	MelLib::Camera::Get()->SetCameraToTargetDistance(10.0f);
	MelLib::Camera::Get()->SetRotateCriteriaPosition(MelLib::Vector3(0,0,0));


}

void Play::CatTestDraw()
{
	
	//if (isCat)
	//{
	//	frontObj.Draw();
	//	backObj.Draw();
	//}
	//else
	//{
	//	rtTestObject.Draw();
	//}
}



void Play::ParticleTestInitialize()
{
	// パーティクルの作成
	std::vector<std::shared_ptr<MelLib::Particle3D>> particle(100);
	for(auto& p : particle)
	{
		p = std::make_shared<TestParticle>();
	}

	// エミッターの作成
	std::shared_ptr <MelLib::Emitter3D>e = std::make_shared<MelLib::Emitter3D>(particle, 0, 1);
	
	// エミッターの追加
	MelLib::GameObjectManager::GetInstance()->AddObject(e);
}

void Play::ParticleTestUpdate()
{
}

void Play::ParticleTestDraw()
{

}

MelLib::ModelObject aniTest;
MelLib::ModelObject aniCollTest;
MelLib::Vector3 aniCollInitPos = MelLib::Vector3(-1.0, 0.5, 0.25);
//MelLib::Vector3 aniCollInitPos = MelLib::Vector3(-3 ,3,3);
void Play::AniTestInitialize(){

	//MelLib::ModelData::Load("Resources/boneTest/boneTest.fbx", false, "test");
	//MelLib::ModelData::Load("Resources/ani_test.fbx", false, "test");
	bool res = MelLib::ModelData::Load("Resources/Player_Test.fbx", false, "test");
	//bool res = MelLib::ModelData::Load("Resources/Player_Test_Low.fbx", false, "test");
	//bool res = MelLib::ModelData::Load("Resources/testBox.fbx", false, "test");
	//bool res = MelLib::ModelData::Load("Resources/test.obj", false, "test");
	//MelLib::ModelData::Load("Resources/Player_Test_No_Ani.fbx", false, "test");
	int c = 0;


	aniTest.Create(MelLib::ModelData::Get("test"), nullptr);
	aniTest.SetScale(0.2);
	

	aniCollTest.Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), nullptr);
	aniCollTest.SetScale(0.25);
	aniCollTest.SetPosition(aniCollInitPos);

	MelLib::Camera::Get()->SetRotateCriteriaPosition(MelLib::Vector3(0, 0, 0));
	MelLib::Camera::Get()->SetRotatePoint(MelLib::Camera::RotatePoint::ROTATE_POINT_TARGET_POSITION);
	MelLib::Camera::Get()->SetCameraToTargetDistance(7.0f);

}

void Play::AniTestUpdate() {

	aniTest.Update();

	if (MelLib::Input::KeyState(DIK_1))aniTest.SetAnimation("Attack_Normal_1");
	if (MelLib::Input::KeyState(DIK_2))aniTest.SetAnimation("No_Cont");
	if (MelLib::Input::KeyState(DIK_3))aniTest.SetAnimation("_T_02");
	if (MelLib::Input::KeyState(DIK_4))aniTest.SetAnimation("Dash");

	MelLib::Vector3 cameraAngle = MelLib::Camera::Get()->GetAngle();
	if (MelLib::Input::KeyState(DIK_A)) {
		cameraAngle.y += 3;
		MelLib::Camera::Get()->SetAngle(cameraAngle);
	}
	if (MelLib::Input::KeyState(DIK_D)) {

		cameraAngle.y -= 3;
		MelLib::Camera::Get()->SetAngle(cameraAngle);

	}


	if (MelLib::Input::KeyState(DIK_W)) {

		cameraAngle.x += 3;
		MelLib::Camera::Get()->SetAngle(cameraAngle);

	}
	if (MelLib::Input::KeyState(DIK_S)) {

		cameraAngle.x -= 3;
		MelLib::Camera::Get()->SetAngle(cameraAngle);

	}

	MelLib::Vector3 cameraPos = MelLib::Camera::Get()->GetTargetPosition();
	if (MelLib::Input::KeyState(DIK_UP)) {

		cameraPos.y += 0.1;
		MelLib::Camera::Get()->SetRotateCriteriaPosition(cameraPos);
	}
	if (MelLib::Input::KeyState(DIK_DOWN)) {

		cameraPos.y -= 0.1;
		MelLib::Camera::Get()->SetRotateCriteriaPosition(cameraPos);
	}



	MelLib::Vector3 calcPos = aniCollInitPos;
	calcPos = aniTest.CalcAnimationPosition(calcPos,1, "Bone_R.003", "Body");
	aniCollTest.SetPosition(calcPos);

	aniTest.SetAnimationPlayFlag(true);

}

void Play::AniTestDraw(){
	aniCollTest.Draw();
	aniTest.Draw();

}

Play::Play(){}


Play::~Play(){}


void Play::Initialize()
{
	//CatInitialize();
	//ParticleTestInitialize();
	AniTestInitialize();
	

}

void Play::Update()
{


	//CatTestUpdate();
	//ParticleTestUpdate();
	AniTestUpdate();

	MelLib::GameObjectManager::GetInstance()->Update();
	int z = 0;
}

void Play::Draw()
{
	//CatTestDraw();
	//ParticleTestDraw();
	AniTestDraw();

	MelLib::GameObjectManager::GetInstance()->Draw();
}

void Play::Finalize()
{
	MelLib::GameObjectManager::GetInstance()->AllEraseObject();
}

MelLib::Scene* Play::GetNextScene()
{
	return new Play();
}

void Play::LoadResources()
{
}

