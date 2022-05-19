#include "Player.h"
#include"TestObject.h"
#include<Input.h>
#include<LibMath.h>


void Player::Move()
{
	// �ړ��x�N�g��
	MelLib::Vector3 moveVector;
	// �ړ����x
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

	// ���Z
	// AddPosition�ASetPosition�͓����蔻����ꏏ�ɓ���
	AddPosition(moveVector);
}

Player::Player()
	:hp(10, "Player", "HP", 0, 100)
	,power(10, "Player", "Power", 1, 100)
{
	// MelLib;;ModelObject�̔z��
	// �l�p�`���Z�b�g
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	// �����ʒu��0,0,5��
	SetPosition(MelLib::Vector3(-5, 0, 0));

	// �����蔻��̍쐬(��)
	// Player�̍��W���擾���A������Z�b�g
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(GetPosition());
	sphereDatas["main"][0].SetRadius(0.5f);

	sphereFrameHitCheckNum = 100;
}

void Player::Update()
{
	Move();

	// SPACE�������烂�f���Ǘ��N���X����폜(���������)
	if(MelLib::Input::KeyTrigger(DIK_SPACE))
	{
		//eraseManager = true;
	}

	modelObjects["main"].SetMulColor(MelLib::Color(255, 255, 255, 255));
}

void Player::Draw()
{
	// ModelObjects�ɒǉ�����Ă���ModelObject�����ׂĕ`��
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
