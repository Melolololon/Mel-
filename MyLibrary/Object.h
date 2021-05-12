#pragma once
#include<typeinfo>
#include<vector>
#include"Library.h"
#include"Vector.h"
#include"CollisionType.h"

//�I�u�W�F�N�g�}�l�[�W���[�ǉ����ɔ����I�Ԃ悤�ɂ���?(���育�Ƃɒǉ��֐��Ɣz����)

class Object
{
public:


protected:
	Vector3 position = { 0,0,0 };
	Vector3 velocity = { 0,0,0 };
	Vector3 speed = { 0,0,0 };

	//�����t���O(���ꂪtrue�ɂȂ�ƁA�I�u�W�F�N�g�}�l�[�W���[���珜�O�����)
	bool eraseManager = false;


#pragma region ����f�[�^
	std::vector<SphereData> sphereData;
	std::vector<BoxData> boxData;
	std::vector<LineSegmentData> lineSegmentData;
	std::vector<RayData> layData;
	std::vector<PlaneData>planeData;
	std::vector<BoardData>boardData;
#pragma endregion

	CollisionFlag collisionFlag;
public:

	//�R���X�g���N�^
	Object();
	//�f�X�g���N�^
	virtual ~Object();

	//������
	virtual void Initialize();
	//����
	virtual void Update();
	//�`�揈��
	virtual void Draw();

	/// <summary>
	/// ���菈��
	/// </summary>
	/// <param name="object">����I�u�W�F�g�̃|�C���^</param>
	/// <param name="collisionType">�����̂ǂ̔���ɓ���������</param>
	/// <param name="arrayNum">���ڂ̔���ɓ���������</param>
	virtual void Hit
	(
		const Object *const  object,
		const CollisionType& collisionType,
		const int& arrayNum
	);

	virtual const void* GetPtr()const;

	//�I�u�W�F�N�g�}�l�[�W���[����폜���邩�ǂ����̃t���O��Ԃ�
	bool GetEraseManager();


	//�m���ɏ����l�����܂��Ă���ϐ�(eraseManager�Ȃ�)������������ϐ�(�������Y��h�~�p)
	//����ŏ����������Ƀw�b�_�ŏ���������?
	//�Ēǉ������Ƃ��ɏ��������������炱�̂܂܂ł���
	void ObjectInitialize();
	
	Vector3 GetPosition()const { return position; };

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


	
};