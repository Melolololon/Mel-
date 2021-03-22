#pragma once
#include<Object.h>
class TestObject:public Object
{
private:
	ModelData m;
	int heapNum;
	static int createCount;
	Vector3 angle;
public:
	TestObject(const Vector3& pos);
	~TestObject();

	//������
	 void initialize()override;
	//����
	 void update()override;
	//�`�揈��
	 void draw()override;

	/// <summary>
	/// ���菈��
	/// </summary>
	/// <param name="object">����I�u�W�F�g�̃|�C���^</param>
	/// <param name="collisionType">�����̂ǂ̔���ɓ���������</param>
	/// <param name="arrayNum">���ڂ̔���ɓ���������</param>
	/* void hit
	(
		const Object const*  object,
		const CollisionType& collisionType,
		const int& arrayNum
	)override;

	 void* getPtr()override;*/
};

