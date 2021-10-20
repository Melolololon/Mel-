#pragma once
#include<GameObject.h>

class MeshCatObject :public MelLib::GameObject
{
private:
	static const MelLib::PlaneData* pPlaneData;

	bool firstFream = true;
	bool isCat = false;
public:
	static void SetPlaneData(const MelLib::PlaneData* p) { pPlaneData = p; }

	MeshCatObject(MelLib::Vector3 pos, MelLib::ModelData* pModel = nullptr);
	~MeshCatObject(){}

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
		const MelLib::ShapeType3D& collisionType,
		const int arrayNum,
		const MelLib::ShapeType3D& hitObjColType,
		const int hitObjArrayNum
	)override;
};

