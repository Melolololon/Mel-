#pragma once
#include"Object.h"
#include"CollisionType.h"
#include<vector>
#include"MouseCursor.h"
class ObjectManager
{
public:
	enum ObjectSort
	{
		//XYZ�̍��v�l�Ń\�[�g
		OBJECT_SORT_NONE,
		OBJECT_SORT_XYZ_SUM,
		OBJECT_SORT_X,
		OBJECT_SORT_Y,
		OBJECT_SORT_Z,
		OBJECT_SORT_NEAR_DISTANCE,
		OBJECT_SORT_FAR_DISTANCE,

	};


private:
	ObjectManager();
	~ObjectManager();

	std::vector<Object*>objects;

	CollisionFlag checkCollision;

	//�ǉ����邲�ƂɃ\�[�g���邩
	ObjectSort addObjectSort;
	bool addObjectSortOrderType;

	//�J�[�\������
	bool checkMouseCollision;
	Vector3 cameraPosition;
	std::unique_ptr<MouseCursor>cursor;

	//�f�[�^
	Vector3 nearPos;
	Vector3 farPos;
public:
	ObjectManager(const ObjectManager& obj) = delete;
	ObjectManager& operator=(const ObjectManager& obj) = delete;

	static ObjectManager* getInstance();

	/// <summary>
	/// ���������܂�
	/// </summary>
	void initialize();

	/// <summary>
	/// �X�V�������s���܂�
	/// </summary>
	void update();

	/// <summary>
	/// �`�揈�����s���܂�
	/// </summary>
	void draw();

	/// <summary>
	/// isDead��true���ǂ������m�F���܂��Btrue�̏ꍇ�́A�폜����܂�
	/// </summary>
	void isDeadCheck();

	/// <summary>
	/// �I���������s���܂�
	/// </summary>
	void end();

	/// <summary>
	/// �ǂ̔�����s������ݒ肵�܂�
	/// </summary>
	/// <param name="type"></param>
	void setCollisionFlag3D(const CollisionFlag& type);
	
	/// <summary>
	/// �}�E�X�J�[�\���Ƃ̔�����Ƃ邩�ݒ肵�܂�
	/// </summary>
	/// <param name="flag"></param>
	void setMouseCollisionFlag(const bool& flag);


#pragma region �I�u�W�F�N�g�Ǘ�

	/// <summary>
	/// �z��̃��������m�ۂ��܂��B����ɂ��A�ǉ����̏����������Ȃ�܂����A�����������������Ȃ�܂�
	/// </summary>
	/// <param name="reserveNum"></param>
	void reserveObjectArray(const int& reserveNum);

	/// <summary>
	/// �I�u�W�F�N�g��ǉ����܂��B
	/// </summary>
	/// <param name="object"></param>
	void addObject(Object* object);

	/// <summary>
	/// �ǉ����邲�ƂɃ\�[�g���邩��ݒ肵�܂��B�ǉ����Ȃ��ꍇ�AorderType�͖�������܂�
	/// </summary>
	/// <param name="sort">�\�[�g�̎d��</param>
	/// <param name="orderType">true ����  false �~��</param>
	void setAddObjectSortState(const ObjectSort& sort,const bool& orderType);

	
/// <summary>
/// �I�u�W�F�N�g�̔z����\�[�g���܂��B
/// </summary>
/// <param name="sort">�\�[�g�̎d��</param>
/// <param name="orderType">true ����  false �~��</param>
	void setObjectSort(const ObjectSort& sort,const bool& orderType);
#pragma endregion

	/// <summary>
	/// �S�ẴI�u�W�F�N�g��������܂�
	/// </summary>
	void allDeleteObject();
};

