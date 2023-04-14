#include "TestObject.h"

#include"Random.h"

void TestObject::Move()
{
}

TestObject::TestObject() :GameObject("Test")
{
	// MelLib;;ModelObject�̔z��
	// �l�p�`���Z�b�g
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX),"Test");

	collisionCheckDistance = 1.0f;


}

void TestObject::Update()
{
	Move();


	MelLib::Vector3 ang;
	ang.y = 0;
	SetAngle(ang);
}

void TestObject::Draw()
{
	// ModelObjects�ɒǉ�����Ă���ModelObject�����ׂĕ`��
	AllDraw();
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
