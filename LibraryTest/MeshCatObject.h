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
	/// 更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw()override;

	/// <summary>
	/// 当たった時の処理
	/// </summary>
	/// <param name="object">相手オブジェトのポインタ</param>
	/// <param name="collisionType">自分のどの判定に当たったか</param>
	/// <param name="arrayNum">自分の何個目の判定に当たったか</param>
	/// <param name="hitObjColType">相手のどの判定に当たったか</param>
	/// <param name="hitObjArrayNum">相手の何個目の判定に当たったか</param>
	void Hit
	(
		const GameObject* const  object,
		const MelLib::ShapeType3D& collisionType,
		const int arrayNum,
		const MelLib::ShapeType3D& hitObjColType,
		const int hitObjArrayNum
	)override;
};

