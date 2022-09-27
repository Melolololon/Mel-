#include "TestObject.h"

#include"Random.h"

void TestObject::Move()
{
}

TestObject::TestObject() :GameObject("Test")
{
	// MelLib;;ModelObjectの配列
	// 四角形をセット
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));
	modelObjects["main2"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));
	modelObjects["main3"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	// 初期位置を0,0,5に
	//SetPosition(MelLib::Vector3(MelLib::Random::GetRandomFloatNumberRangeSelect(-10.0f, 10.0f, 2),0, MelLib::Random::GetRandomFloatNumberRangeSelect(-10.0f, 10.0f, 2)));
	SetPosition(MelLib::Vector3(0, 0, 0));

	// 当たり判定の作成(球)
	// Playerの座標を取得し、それをセット
	/*sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(GetPosition());
	sphereDatas["main"][0].SetRadius(1.0f);*/

	/*obbDatas["main"].resize(1);
	obbDatas["main"][0].SetPosition(GetPosition());
	obbDatas["main"][0].SetSize(0.05f);
	modelObjects["main"].SetScale(0.05f);*/

	/*boxDatas["main"].resize(1);
	boxDatas["main"][0].SetPosition(GetPosition());
	boxDatas["main"][0].SetSize(0.05f);
	modelObjects["main"].SetScale(0.05f);*/
	triangleDatas["main"].resize(1);
	triangleDatas["main"][0].SetPosition(MelLib::Value3<MelLib::Vector3>
		(
			MelLib::Vector3(-3,0,-3),
			MelLib::Vector3(0, 0, 3),
			MelLib::Vector3(3,0,-3)
			));

	//SetAllCollisionFlag(false);
	//collisionFlag.box = true;
	collisionCheckDistance = 1.0f;


	modelObjects["main"].SetPosition(triangleDatas["main"][0].GetPosition().v1);
	modelObjects["main2"].SetPosition(triangleDatas["main"][0].GetPosition().v2);
	modelObjects["main3"].SetPosition(triangleDatas["main"][0].GetPosition().v3);
}

void TestObject::Update()
{
	Move();

	modelObjects["main"].SetMulColor(MelLib::Color(255, 255, 255, 255));
	modelObjects["main2"].SetMulColor(MelLib::Color(255, 255, 255, 255));
	modelObjects["main3"].SetMulColor(MelLib::Color(255, 255, 255, 255));

	MelLib::Vector3 ang;
	ang.y = 0;
	SetAngle(ang);
}

void TestObject::Draw()
{
	// ModelObjectsに追加されているModelObjectをすべて描画
	//AllDraw();
}

void TestObject::Hit
(
	const GameObject& object,
	const MelLib::ShapeType3D shapeType,
	const std::string& shapeName,
	const MelLib::ShapeType3D hitObjShapeType,
	const std::string& hitShapeName
)
{

}

std::shared_ptr<MelLib::GameObject> TestObject::GetNewPtr()
{
	return std::make_shared<TestObject>();
}
