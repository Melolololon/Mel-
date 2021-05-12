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

	CollisionFlag collisionFlag;
public:

	//コンストラクタ
	Object();
	//デストラクタ
	virtual ~Object();

	//初期化
	virtual void Initialize();
	//処理
	virtual void Update();
	//描画処理
	virtual void Draw();

	/// <summary>
	/// 判定処理
	/// </summary>
	/// <param name="object">相手オブジェトのポインタ</param>
	/// <param name="collisionType">自分のどの判定に当たったか</param>
	/// <param name="arrayNum">何個目の判定に当たったか</param>
	virtual void Hit
	(
		const Object *const  object,
		const CollisionType& collisionType,
		const int& arrayNum
	);

	virtual const void* GetPtr()const;

	//オブジェクトマネージャーから削除するかどうかのフラグを返す
	bool GetEraseManager();


	//確実に初期値が決まっている変数(eraseManagerなど)を初期化する変数(初期化忘れ防止用)
	//これで初期化せずにヘッダで初期化する?
	//再追加したときに初期化したいからこのままでいい
	void ObjectInitialize();
	
	Vector3 GetPosition()const { return position; };

	//判定用関数
	CollisionFlag GetCollisionFlag();
	std::vector<SphereData> GetSphereData();
	std::vector<BoxData> GetBoxData();
	std::vector<LineSegmentData> GetLineSegmentData();
	std::vector<PlaneData> GetPlaneData();
	std::vector<BoardData> GetBoardData();
	Vector3& GetLineSegmentHitPosition(const int num);
	Vector3& GetBoardHitPosition(const int num);
	BoxHitDirection& GetSphereBoxHitDistance(const int num) { return sphereData[num].boxHitDistance; }
	BoxHitDirection& GetBoxBoxHitDistance(const int num) { return boxData[num].boxHitDistance; }


	
};