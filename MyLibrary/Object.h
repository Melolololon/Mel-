#pragma once
#include<typeinfo>
#include<vector>
#include"Library.h"
#include"Vector.h"
#include"CollisionType.h"

//オブジェクトマネージャー追加時に判定を選ぶようにする?(判定ごとに追加関数と配列作る)

class Object
{
public:


protected:
	//座標
	Vector3 position;
	//向き
	Vector3 velocity;
	//移動速度
	Vector3 speed;

	//生死フラグ 
	bool isDead;


#pragma region 判定データ
	std::vector<SphereData> sphereData;
	std::vector<LineSegmentData> lineSegmentData;
	std::vector<RayData> layData;
	std::vector<PlaneData>planeData;
	std::vector<BoardData>boardData;
#pragma endregion

	CollisionFlag collisionFlag;
public:

	//コンストラクタ
	Object();
	//デストラクタ
	virtual ~Object();
	//処理
	virtual void update();
	//描画処理
	virtual void draw();

	/// <summary>
	/// 判定処理
	/// </summary>
	/// <param name="object">相手オブジェトのポインタ</param>
	/// <param name="collisionType">自分のどの判定に当たったか</param>
	/// <param name="arrayNum">何個目の判定に当たったか</param>
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