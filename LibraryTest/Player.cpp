#include "Player.h"
#include"TestObject.h"
#include<Input.h>
#include<LibMath.h>


void Player::Move()
{
	// 移動ベクトル
	MelLib::Vector3 moveVector;
	// 移動速度
	static const float MOVE_SPEED = 10.0f;
	
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

Player::Player()
	:hp(10, "Player", "HP", 0, 100)
	,power(10, "Player", "Power", 1, 100)
{
	// MelLib;;ModelObjectの配列
	// 四角形をセット
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	// 初期位置を0,0,5に
	SetPosition(MelLib::Vector3(-5, 0, 0));

	// 当たり判定の作成(球)
	// Playerの座標を取得し、それをセット
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(GetPosition());
	sphereDatas["main"][0].SetRadius(0.5f);

	sphereFrameHitCheckNum = 100;
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
	}


}

std::shared_ptr<MelLib::GameObject> Player::GetNewPtr()
{
	return std::make_shared<Player>();
}
