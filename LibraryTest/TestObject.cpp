#include "TestObject.h"



void TestObject::Move()
{
}

TestObject::TestObject()
{
	// MelLib;;ModelObject�̔z��
	// �l�p�`���Z�b�g
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	// �����ʒu��0,0,5��
	SetPosition(MelLib::Vector3(0, 0, 0));

	// �����蔻��̍쐬(��)
	// Player�̍��W���擾���A������Z�b�g
	obbDatas["main"].resize(1);
	obbDatas["main"][0].GetRefBoxData().SetPosition(GetPosition());
	obbDatas["main"][0].GetRefBoxData().SetSize(MelLib::Vector3(5,1,1));
	modelObjects["main"].SetScale(MelLib::Vector3(5, 1, 1));
}

void TestObject::Update()
{
	Move();

	modelObjects["main"].SetMulColor(MelLib::Color(255, 255, 255, 255));

	MelLib::Vector3 ang;
	ang.y = GetAngle().y + 3;
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
