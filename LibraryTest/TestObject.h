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

	//初期化
	 void initialize()override;
	//処理
	 void update()override;
	//描画処理
	 void draw()override;

	/// <summary>
	/// 判定処理
	/// </summary>
	/// <param name="object">相手オブジェトのポインタ</param>
	/// <param name="collisionType">自分のどの判定に当たったか</param>
	/// <param name="arrayNum">何個目の判定に当たったか</param>
	/* void hit
	(
		const Object const*  object,
		const CollisionType& collisionType,
		const int& arrayNum
	)override;

	 void* getPtr()override;*/
};

