#include "Player.h"
#include"TestObject.h"
#include<Input.h>
#include<LibMath.h>


void Player::Move()
{
	// 移動ベクトル
	MelLib::Vector3 moveVector;
	// 移動速度
	static const float MOVE_SPEED = 0.2f;
	
	if (MelLib::Input::KeyState(DIK_LEFT))
	{
		moveVector.x -= MOVE_SPEED;
	}
	else if (MelLib::Input::KeyState(DIK_RIGHT))
	{
		moveVector.x += MOVE_SPEED;
	}
	else if (MelLib::Input::KeyState(DIK_UP))
	{
		moveVector.z += MOVE_SPEED;
	}
	else if (MelLib::Input::KeyState(DIK_DOWN))
	{
		moveVector.z -= MOVE_SPEED;
	}

	if (MelLib::Input::KeyTrigger(DIK_SPACE)) {
		moveVector.x += MOVE_SPEED;
	}
	float angle = MelLib::Input::LeftStickAngle(30);
	if (angle != -1.0f) 
	{
		MelLib::Vector2 v = MelLib::LibMath::AngleToVector2(angle, true);
		moveVector.x = v.x * MOVE_SPEED;
		moveVector.z = v.y * MOVE_SPEED;
	}

	// 加算
	// AddPosition、SetPositionは当たり判定も一緒に動く
	AddPosition(moveVector);
}

void Player::LoadRes()
{
	bool res = MelLib::Texture::Load("Resources/Texture/testTexture.png","test");
	bool res2 = MelLib::Texture::Load("Resources/Texture/testTexture2.png","test2");
	int x = 0;
}

Player::Player()
	:hp(10, "Player", "HP", 0, 100)
	,power(10, "Player", "Power", 1, 100)
{
	// MelLib;;ModelObjectの配列
	// 四角形をセット
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	// 初期位置を0,0,5に
	SetPosition(MelLib::Vector3(0, 0, 0));

	// 当たり判定の作成(球)
	// Playerの座標を取得し、それをセット
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(GetPosition());
	sphereDatas["main"][0].SetRadius(0.5f);
	modelObjects["main"].SetScale(1);

	sphereFrameHitCheckNum = 1;


	// 描画設定
	MelLib::DrawOption data;
	// モデルの初期設定をセット
	data.SetModelDefData();

	// (設定構造体,テクスチャ数(ディスクリプタヒープのサイズ))
	testMtl.Create(data, 2);

	// (テクスチャ,名前(後々変更したりするときにこの名前で呼び出す))
	// セットした順にレジスター0から使える
	testMtl.SetTexture(MelLib::Texture::Get("test"),"main");
	testMtl.SetTexture(MelLib::Texture::Get("test2"),"NormalMap");

	modelObjects["main"].SetMaterial(&testMtl);
}

void Player::Update()
{
	Move();

	// SPACE押したらモデル管理クラスから削除(メモリ解放)
	if(MelLib::Input::KeyTrigger(DIK_SPACE))
	{
		//eraseManager = true;
	}

	modelObjects["main"].SetMulColor(MelLib::Color(255, 255, 255, 255));



	collisionCheckDistance = 40.0f;
}

void Player::Draw()
{
	// ModelObjectsに追加されているModelObjectをすべて描画
	AllDraw();
}

void Player::Hit
(
	const GameObject& object, 
	const MelLib::ShapeType3D shapeType, 
	const std::string& shapeName, 
	const MelLib::ShapeType3D hitObjShapeType,
	const std::string& hitShapeName
)
{
	if (typeid(object) == typeid(TestObject)) 
	{
		modelObjects["main"].SetMulColor(MelLib::Color(111, 111, 111, 255));
		/*MelLib::Vector3 vec = GetSphereCalcResult().GetOBBHitSurfaceNormal();
		AddPosition(vec * 0.3f);*/

		//MelLib::Vector3 getPos = GetLerpPosition();
		//MelLib::Vector3 movePos = GetLerpMovePosition();
		//SetPosition(GetLerpExtrudePosition());

		//MelLib::Vector3 myPos = GetPosition();
		//int z = 90;
	}


}

std::shared_ptr<MelLib::GameObject> Player::GetNewPtr()
{
	return std::make_shared<Player>();
}
