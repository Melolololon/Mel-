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

	//�����t���O 
	bool isDead;


#pragma region ����f�[�^
	std::vector<SphereData> sphereData;
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
	virtual void hit(Object* object,const CollisionType& collisionType,const int& arrayNum);

	bool getIsDead();
	void initializeObject();
	
	Vector3 getPosition() { return position; };

	CollisionFlag getCollisionFlag();
	std::vector<SphereData> getSphereData();
	std::vector<LineSegmentData>& getLineSegmentData();
	std::vector<PlaneData> getPlaneData();
	std::vector<BoardData>& getBoardData();

	/*void setLineSegmentDataHitPos(const Vector3& pos, const int& arrayNum);
	void setBoardDataHitPos(const Vector3& pos, const int& arrayNum);*/

	virtual void* getPtr();

	
};