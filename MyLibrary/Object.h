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
	//���W
	Vector3 position;
	//����
	Vector3 velocity;
	//�ړ����x
	Vector3 speed;

	//�����t���O(���ꂪtrue�ɂȂ�ƁA�I�u�W�F�N�g�}�l�[�W���[���珜�O�����)
	bool eraseManager;


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
	virtual void initialize();
	//����
	virtual void update();
	//�`�揈��
	virtual void draw();

	/// <summary>
	/// ���菈��
	/// </summary>
	/// <param name="object">����I�u�W�F�g�̃|�C���^</param>
	/// <param name="collisionType">�����̂ǂ̔���ɓ���������</param>
	/// <param name="arrayNum">���ڂ̔���ɓ���������</param>
	virtual void hit
	(
		const Object *const  object,
		const CollisionType& collisionType,
		const int& arrayNum
	);

	virtual void* getPtr();

	//�I�u�W�F�N�g�}�l�[�W���[����폜���邩���ۂ̃t���O��Ԃ�
	bool getEraseManager();

	//�m���ɏ����l�����܂��Ă���ϐ�(eraseManager�Ȃ�)������������ϐ�(�������Y��h�~�p)
	void objectInitialize();
	
	Vector3 getPosition() { return position; };

	//����p�֐�
	CollisionFlag getCollisionFlag();
	std::vector<SphereData> getSphereData();
	std::vector<BoxData> getBoxData();
	std::vector<LineSegmentData> getLineSegmentData();
	std::vector<PlaneData> getPlaneData();
	std::vector<BoardData> getBoardData();
	Vector3& getLineSegmentHitPosition(const int & num);
	Vector3& getBoardHitPosition(const int & num);
	BoxHitDirection& getSphereBoxHitDistance(const int & num) { return sphereData[num].boxHitDistance; }
	BoxHitDirection& getBoxBoxHitDistance(const int & num) { return boxData[num].boxHitDistance; }


	
};