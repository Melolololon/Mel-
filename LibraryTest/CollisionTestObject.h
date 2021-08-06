#pragma once
#include"GameObject.h"

#include"ModelData.h"
#include"ModelObject.h"

//当たり判定テスト用オブジェクト
class CollisionTestObject:public melLib::GameObject
{
private:
	std::unique_ptr<melLib::ModelObject> model[2];
	bool hitFlag = false;
	bool inputFlag = false;
public:
	CollisionTestObject(const melLib::Vector3& pos,const bool inputMove);

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
		const melLib::CollisionType3D collisionType,
		const int arrayNum,
		const melLib::CollisionType3D hitObjColType,
		const int hitObjArrayNum
	)override;
};

