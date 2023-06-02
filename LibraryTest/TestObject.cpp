#include "TestObject.h"

#include"Random.h"
#include<TextWrite.h>
#include<Input.h>

void TestObject::Move()
{
}

TestObject::TestObject() :GameObject("Test")
{
	// MelLib;;ModelObjectの配列
	// 四角形をセット
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX),"Test");

	collisionCheckDistance = 1.0f;


	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(0);
	sphereDatas["main"][0].SetRadius(1.0f);
}

void TestObject::Update()
{
	Move();

	if (MelLib::Input::KeyTrigger(DIK_Z))SetAngle(MelLib::Vector3(3, 0, 0));
}

void TestObject::Draw()
{
	// ModelObjectsに追加されているModelObjectをすべて描画
	AllDraw();

	MelLib::TextWrite::Draw(0, MelLib::Color(255, 255, 255, 255), std::to_wstring(modelObjects["main"].GetAngle().x), "Arial");
	MelLib::TextWrite::Draw(MelLib::Vector2(0,80), MelLib::Color(255, 255, 255, 255), std::to_wstring(GetAngle().x), "Arial");
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
