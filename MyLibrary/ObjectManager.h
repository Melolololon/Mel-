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
		//XYZの合計値でソート
		OBJECT_SORT_NONE,
		OBJECT_SORT_XYZ_SUM,
		OBJECT_SORT_X,
		OBJECT_SORT_Y,
		OBJECT_SORT_Z,
		//カメラから近い順
		OBJECT_SORT_NEAR_DISTANCE,
		//カメラから遠い順
		OBJECT_SORT_FAR_DISTANCE,

	};


private:
	ObjectManager();
	~ObjectManager();

	std::vector<Object*>objects;
	//追加されたものを一時的に入れておく配列
	std::vector<Object*>addObjects;

	CollisionFlag checkCollision;

	//追加したフレームごとにソートするか
	ObjectSort addObjectSort;
	bool addObjectSortOrderType;

	//カーソル判定
	bool checkMouseCollision;
	Vector3 cameraPosition;
	std::unique_ptr<MouseCursor>cursor;

	//データ
	Vector3 nearPos;
	Vector3 farPos;
public:
	ObjectManager(const ObjectManager& obj) = delete;
	ObjectManager& operator=(const ObjectManager& obj) = delete;

	static ObjectManager* getInstance();

	/// <summary>
	/// 初期化します
	/// </summary>
	void initialize();

	/// <summary>
	/// 更新処理を行います
	/// </summary>
	void update();

	/// <summary>
	/// 描画処理を行います
	/// </summary>
	void draw();

	/// <summary>
	/// isDeadがtrueかどうかを確認します。trueの場合は、削除されます
	/// </summary>
	void isDeadCheck();

	/// <summary>
	/// 終了処理を行います
	/// </summary>
	void end();

	/// <summary>
	/// どの判定を行うかを設定します
	/// </summary>
	/// <param name="type"></param>
	void setCollisionFlag3D(const CollisionFlag& type);
	
	/// <summary>
	/// マウスカーソルとの判定をとるか設定します
	/// </summary>
	/// <param name="flag"></param>
	void setMouseCollisionFlag(const bool& flag);


#pragma region オブジェクト管理

	/// <summary>
	/// 配列のメモリを確保します。これにより、追加時の処理が速くなりますが、メモリ効率が悪くなります
	/// </summary>
	/// <param name="reserveNum"></param>
	void reserveObjectArray(const int& reserveNum);

	/// <summary>
	/// オブジェクトを追加します。
	/// </summary>
	/// <param name="object"></param>
	void addObject(Object* object);

	/// <summary>
	/// 追加したフレームごとにソートするかを設定します。追加しない場合、orderTypeは無視されます
	/// </summary>
	/// <param name="sort">ソートの仕方</param>
	/// <param name="orderType">true 昇順  false 降順</param>
	void setAddObjectSortState(const ObjectSort& sort,const bool& orderType);

	
/// <summary>
/// オブジェクトの配列をソートします。
/// </summary>
/// <param name="sort">ソートの仕方</param>
/// <param name="orderType">true 昇順  false 降順</param>
	void objectSort(const ObjectSort& sort,const bool& orderType);
#pragma endregion

	/// <summary>
	/// 全てのオブジェクトを解放します
	/// </summary>
	void allDeleteObject();
};

