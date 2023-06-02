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
	bool res = MelLib::ModelData::Load("Resources/Player/Player.fbx", false,"test");

}

// 名前を指定するときにobjectNameを渡さないと番号が反映されない
// あと名前変更した後にGUIの変数初期化しないといけない
// 追加関数じゃなくてGameObjectのコンストラクタで番号付けないといけない
// ObjectManagerに存在するか確認して番号付けると追加せずにEnemy5つ作ったら全部同じになっちゃう
// GameObjectにstaticのumap持たせて番号管理したほうがよさそう
Player::Player()
	:GameObject("Player")
{
	// 読み込んだパラメーターを自動的にモデルにも適応させる
	// modelObjectや当たり判定もGUIでいじれるようにする

	// MelLib;;ModelObjectの配列
	// 四角形をセット
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX),GetObjectName());

	// 今読み込んだ値をモデルオブジェクトに反映できてないから
	// 初期位置を0,0,5に
	//SetPosition(GetPosition());

	modelObjects["main"].SetPosition(GetPosition());

	// 当たり判定の作成(球)
	// Playerの座標を取得し、それをセット
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition({2.5f,0,0});
	sphereDatas["main"][0].SetRadius(1.0f);
	//modelObjects["main"].SetPosition(GetPosition());
	modelObjects["main"].SetScale(1);

	sphereFrameHitCheckNum = 1;


	

	hp.SetData(10, GetObjectName(), "HP", 0, 100);
	power.SetData(10, GetObjectName(), "Power", 1, 100);
}

void Player::Update()
{
	Move();


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

	int z = 0;

}

std::shared_ptr<MelLib::GameObject> Player::GetNewPtr()
{
	return std::make_shared<Player>();
}
