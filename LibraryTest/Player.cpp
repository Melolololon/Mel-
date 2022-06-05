#include "Player.h"
#include"TestObject.h"
#include<Input.h>
#include<LibMath.h>


void Player::Move()
{
	// �ړ��x�N�g��
	MelLib::Vector3 moveVector;
	// �ړ����x
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

	// ���Z
	// AddPosition�ASetPosition�͓����蔻����ꏏ�ɓ���
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
	// MelLib;;ModelObject�̔z��
	// �l�p�`���Z�b�g
	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX));

	// �����ʒu��0,0,5��
	SetPosition(MelLib::Vector3(0, 0, 0));

	// �����蔻��̍쐬(��)
	// Player�̍��W���擾���A������Z�b�g
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(GetPosition());
	sphereDatas["main"][0].SetRadius(0.5f);
	modelObjects["main"].SetScale(1);

	sphereFrameHitCheckNum = 1;


	// �`��ݒ�
	MelLib::DrawOption data;
	// ���f���̏����ݒ���Z�b�g
	data.SetModelDefData();

	// (�ݒ�\����,�e�N�X�`����(�f�B�X�N���v�^�q�[�v�̃T�C�Y))
	testMtl.Create(data, 2);

	// (�e�N�X�`��,���O(��X�ύX�����肷��Ƃ��ɂ��̖��O�ŌĂяo��))
	// �Z�b�g�������Ƀ��W�X�^�[0����g����
	testMtl.SetTexture(MelLib::Texture::Get("test"),"main");
	testMtl.SetTexture(MelLib::Texture::Get("test2"),"NormalMap");

	modelObjects["main"].SetMaterial(&testMtl);
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



	collisionCheckDistance = 40.0f;
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
