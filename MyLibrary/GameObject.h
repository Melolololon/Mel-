#pragma once
#include<typeinfo>
#include<vector>
#include"Library.h"
#include"Vector.h"
#include"CollisionType.h"

//�I�u�W�F�N�g�}�l�[�W���[�ǉ����ɔ����I�Ԃ悤�ɂ���?(���育�Ƃɒǉ��֐��Ɣz����)

//GameObject���p��������PhysicsObject���ƁAHit�֐��ŏd���Ƃ��󂯎��Ȃ��Ȃ邩��܂Ƃ߂�

//velocity�Ƃ�������ɏ�����������ƃo�O��B
//GameObject��position�Ƃ���private�ɂ��Ċ֐��ŃZ�b�g����悤�ɂ���?(velocity�͏����������Ȃ��悤�ɂ���)
//��������΁AUnity�݂����ɂȂ�
//position��velocity���Z�b�g�ł���悤�ɂ���B(�ϐ��p�ӂ���̂߂�ǂ�����)


class GameObject
{
private:

protected:

	//�����x
	Vector3 acceleration = 0;
	//���̂�������
	Vector3 force = 0;
	//�d��
	float mass = 100.0f;


	Vector3 position = { 0,0,0 };
	Vector3 velocity = { 0,0,0 };
	Vector3 speed = { 0,0,0 };

	//�����t���O(���ꂪtrue�ɂȂ�ƁA�I�u�W�F�N�g�}�l�[�W���[���珜�O�����)
	bool eraseManager = false;

	//�����I�ȋ������s�����߂̌v�Z���s�����ǂ����B
	bool calcPhysics = false;

#pragma region ����f�[�^
	std::vector<SphereData> sphereData;
	std::vector<BoxData> boxData;
	std::vector<LineSegmentData> lineSegmentData;
	std::vector<RayData> layData;
	std::vector<PlaneData>planeData;
	std::vector<BoardData>boardData;
#pragma endregion

	CollisionFlag collisionFlag;

	//�\�[�g�p���l�B�\�[�g���������Œ������邽�߂̕ϐ�
	short sortNumber = 0;


public:
	
	//�R���X�g���N�^
	GameObject();
	//�f�X�g���N�^
	virtual ~GameObject();

	////������
	//virtual void Initialize();
	//�X�V
	virtual void Update();
	//�`��
	virtual void Draw();

	/// <summary>
	/// ���菈��
	/// </summary>
	/// <param name="object">����I�u�W�F�g�̃|�C���^</param>
	/// <param name="collisionType">�����̂ǂ̔���ɓ���������</param>
	/// <param name="arrayNum">���ڂ̔���ɓ���������</param>
	virtual void Hit
	(
		const GameObject *const  object,
		const CollisionType collisionType,
		const int arrayNum,
		const CollisionType hitObjColType,
		const int hitObjArrayNum
	);




	//void CalcHitPhysics(GameObject* hitObject,const Vector3& hutPreVelocity,const CollisionType& collisionType);

	virtual const void* GetPtr()const;



	//�m���ɏ����l�����܂��Ă���ϐ�(eraseManager�Ȃ�)������������ϐ�(�������Y��h�~�p)
	//����ŏ����������Ƀw�b�_�ŏ���������?
	//�Ēǉ������Ƃ��ɏ��������������炱�̂܂܂ł���
	void ObjectInitialize();
	
	//�����Ɋ�Â����ړ��֌W�̌v�Z���s���֐�
	void CalcMovePhysics();

	Vector3 GetPosition()const { return position; }
	Vector3 GetVelocity()const { return velocity; }
	Vector3 GetAcceleration()const { return acceleration; }
	Vector3 GetForce()const { return force; }
	float GetMass()const { return mass; }
	bool GetCalcPhysicsFlag()const { return calcPhysics; }
	short GetSortNumber() const { return sortNumber; }

	//�I�u�W�F�N�g�}�l�[�W���[����폜���邩�ǂ����̃t���O��Ԃ�
	bool GetEraseManager();

	//����p�֐�
	CollisionFlag GetCollisionFlag();
	std::vector<SphereData> GetSphereData();
	std::vector<BoxData> GetBoxData();
	std::vector<LineSegmentData> GetLineSegmentData();
	std::vector<PlaneData> GetPlaneData();
	std::vector<BoardData> GetBoardData();
	Vector3& GetLineSegmentHitPosition(const int num);
	Vector3& GetBoardHitPosition(const int num);
	BoxHitDirection& GetSphereBoxHitDistance(const int num) { return sphereData[num].boxHitDistance; }
	BoxHitDirection& GetBoxBoxHitDistance(const int num) { return boxData[num].boxHitDistance; }


	void SetPosition(const Vector3& position) { this->position = position; }
	void SetVelocity(const Vector3& velocity) { this->velocity = velocity; }
};