#pragma once
#include<typeinfo>
#include<vector>
#include"Library.h"
#include"Vector.h"
#include"CollisionType.h"

//オブジェクトマネージャー追加時に判定を選ぶようにする?(判定ごとに追加関数と配列作る)

//GameObjectを継承させてPhysicsObject作ると、Hit関数で重さとか受け取れなくなるからまとめた

//velocityとかを勝手に書き換えられるとバグる。
//GameObjectのpositionとかをprivateにして関数でセットするようにする?(velocityは書き換えられないようにする)
//そうすれば、Unityみたいになる
//positionもvelocityもセットできるようにする。(変数用意するのめんどいから)


class GameObject
{
private:

protected:

	//加速度
	Vector3 acceleration = 0;
	//物体が動く力
	Vector3 force = 0;
	//重さ
	float mass = 100.0f;


	Vector3 position = { 0,0,0 };
	Vector3 velocity = { 0,0,0 };
	Vector3 speed = { 0,0,0 };

	//生死フラグ(これがtrueになると、オブジェクトマネージャーから除外される)
	bool eraseManager = false;

	//物理的な挙動を行うための計算を行うかどうか。
	bool calcPhysics = false;

#pragma region 判定データ
	std::vector<SphereData> sphereData;
	std::vector<BoxData> boxData;
	std::vector<LineSegmentData> lineSegmentData;
	std::vector<RayData> layData;
	std::vector<PlaneData>planeData;
	std::vector<BoardData>boardData;
#pragma endregion

	CollisionFlag collisionFlag;

	//ソート用数値。ソート順を自分で調整するための変数
	short sortNumber = 0;
public:
	
	//コンストラクタ
	GameObject();
	//デストラクタ
	virtual ~GameObject();

	////初期化
	//virtual void Initialize();
	//更新
	virtual void Update();
	//描画
	virtual void Draw();

	/// <summary>
	/// 判定処理
	/// </summary>
	/// <param name="object">相手オブジェトのポインタ</param>
	/// <param name="collisionType">自分のどの判定に当たったか</param>
	/// <param name="arrayNum">何個目の判定に当たったか</param>
	virtual void Hit
	(
		const GameObject *const  object,
		const CollisionType collisionType,
		const int arrayNum,
		const CollisionType hitObjColType,
		const int hitObjArrayNum
	);




	//void CalcHitPhysics(GameObject* hitObject,const Vector3& hutPreVelocity,const CollisionType& collisionType);

	virtual const void* GetPtr()const;



	//確実に初期値が決まっている変数(eraseManagerなど)を初期化する変数(初期化忘れ防止用)
	//これで初期化せずにヘッダで初期化する?
	//再追加したときに初期化したいからこのままでいい
	void ObjectInitialize();
	
	//物理に基づいた移動関係の計算を行う関数
	void CalcMovePhysics();

	Vector3 GetPosition()const { return position; }
	Vector3 GetVelocity()const { return velocity; }
	Vector3 GetAcceleration()const { return acceleration; }
	Vector3 GetForce()const { return force; }
	float GetMass()const { return mass; }
	bool GetCalcPhysicsFlag()const { return calcPhysics; }
	short GetSortNumber() const { return sortNumber; }

	//オブジェクトマネージャーから削除するかどうかのフラグを返す
	bool GetEraseManager();

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


	void SetPosition(const Vector3& position) { this->position = position; }
	void SetVelocity(const Vector3& velocity) { this->velocity = velocity; }
};