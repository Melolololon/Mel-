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
	Vector3 position = { 0,0,0 };
	Vector3 velocity = { 0,0,0 };
	Vector3 speed = { 0,0,0 };

	//生死フラグ(これがtrueになると、オブジェクトマネージャーから除外される)
	bool eraseManager = false;


#pragma region 判定データ
	std::vector<SphereData> sphereData;
	std::vector<BoxData> boxData;
	std::vector<LineSegmentData> lineSegmentData;
	std::vector<RayData> layData;
	std::vector<PlaneData>planeData;
	std::vector<BoardData>boardData;
#pragma endregion

	CollisionFlag collisionFlag = {false,false,false,false,false,false};
public:

	//コンストラクタ
	Object();
	//デストラクタ
	virtual ~Object();

	//初期化
	virtual void initialize();
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
	virtual void hit
	(
		const Object *const  object,
		const CollisionType& collisionType,
		const int& arrayNum
	);

	virtual void* getPtr();

	//オブジェクトマネージャーから削除するかどうかのフラグを返す
	bool getEraseManager();


	//確実に初期値が決まっている変数(eraseManagerなど)を初期化する変数(初期化忘れ防止用)
	void objectInitialize();
	
	Vector3 getPosition() { return position; };

	//判定用関数
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