#pragma once
#include"GameObject.h"

#include"ModelData.h"
#include"ModelObject.h"

//�����蔻��e�X�g�p�I�u�W�F�N�g
class CollisionTestObject:public GameObject
{
private:
	std::unique_ptr<ModelObject> model;
	bool hitFlag = false;
	bool inputMove = false;
public:
	CollisionTestObject(const Vector3& pos,const bool inputMove);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update()override;
	
	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw()override;

	/// <summary>
	/// �����������̏���
	/// </summary>
	/// <param name="object">����I�u�W�F�g�̃|�C���^</param>
	/// <param name="collisionType">�����̂ǂ̔���ɓ���������</param>
	/// <param name="arrayNum">�����̉��ڂ̔���ɓ���������</param>
	/// <param name="hitObjColType">����̂ǂ̔���ɓ���������</param>
	/// <param name="hitObjArrayNum">����̉��ڂ̔���ɓ���������</param>
	void Hit
	(
		const GameObject* const  object,
		const CollisionType collisionType,
		const int arrayNum,
		const CollisionType hitObjColType,
		const int hitObjArrayNum
	)override;
};
