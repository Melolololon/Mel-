#pragma once
#include<typeinfo>
#include<vector>
#include"Library.h"
#include"Vector.h"
#include"CollisionType.h"
#include"Physics.h"
#ifdef _DEBUG
#include"PipelineState.h"
#include"ModelObject.h"
#include"Material.h"
#endif // _DEBUG


//オブジェクトマネージャー追加時に判定を選ぶようにする?(判定ごとに追加関数と配列作る)

//GameObjectを継承させてPhysicsObject作ると、Hit関数で重さとか受け取れなくなるからまとめた

//velocityとかを勝手に書き換えられるとバグる。
//GameObjectのpositionとかをprivateにして関数でセットするようにする?(velocityは書き換えられないようにする)
//そうすれば、Unityみたいになる
//positionもvelocityもセットできるようにする。(変数用意するのめんどいから)

//SetPositionを仮想関数にして、モデルに座標セットする処理とか書けるようにする
namespace MelLib 
{
	class GameObject
	{
	private:


#ifdef _DEBUG
		//判定確認用モデルのパイプライン
		static PipelineState collisionCheckModelPipelineState;

		//判定確認用モデルのマテリアル
		static ADSAMaterial material;

		//判定確認用モデル

		std::vector<ModelObject>sphereModelObjects;
		std::vector<ModelObject>boxModelObjects;
		std::vector<ModelObject>boardModelObjects;
		std::array<std::vector<ModelObject>, 2>segmentModelObjects;

		//カプセル引き伸ばすと形崩れるから、球と円柱組み合わせて表示したほうがいいかも
		//判定のデータの変数は今のまま(角度消すと円柱を組み合わせるのが大変になる)
		std::array<std::vector<ModelObject>, 2>capsuleModelObjects;


#endif // _DEBUG




#pragma region 物理関係

		//速度
		Vector3 velocity = 0;
		//重力加速度
		static float gravutationalAcc;
		//加速度
		Vector3 acceleration = 0;
#pragma endregion



	protected:
		Vector3 position = 0;

		//物体が動く力
		Vector3 force = 0;
		//重さ(kg)
		float mass = 1.0f;


#pragma region 判定データ
		CollisionDetectionFlag collisionFlag;

		std::vector<SphereData> sphereData;
		std::vector<BoxData> boxData;
		std::vector<Segment3DData> segment3DData;
		std::vector<RayData> layData;
		std::vector<PlaneData>planeData;
		std::vector<BoardData>boardData;
		std::vector<CapsuleData>capsuleData;


#pragma endregion


		//ソート用数値。ソート順を自分で調整するための変数
		short sortNumber = 0;

		//生死フラグ(これがtrueになると、オブジェクトマネージャーから除外される)
		bool eraseManager = false;
		

	private:
	
	public:

		//コンストラクタ
		GameObject();
		//デストラクタ
		virtual ~GameObject();

		//更新
		virtual void Update();
		//描画
		virtual void Draw();

		/// <summary>
		/// 当たった時の処理
		/// </summary>
		/// <param name="object">相手オブジェトのポインタ</param>
		/// <param name="collisionType">自分のどの判定に当たったか</param>
		/// <param name="arrayNum">自分の何個目の判定に当たったか</param>
		/// <param name="hitObjColType">相手のどの判定に当たったか</param>
		/// <param name="hitObjArrayNum">相手の何個目の判定に当たったか</param>
		virtual void Hit
		(
			const GameObject* const  object,
			const ShapeType3D collisionType,
			const int arrayNum,
			const ShapeType3D hitObjColType,
			const int hitObjArrayNum
		);

		/// <summary>
	/// 現在設定されている値で物理演算を行います。
	/// </summary>
		void CalcMovePhysics();


		//void CalcHitPhysics(GameObject* hitObject,const Vector3& hutPreVelocity,const CollisionType& collisionType);

		//virtual const void* GetPtr()const;



		//確実に初期値が決まっている変数(eraseManagerなど)を初期化する変数(初期化忘れ防止用)
		//これで初期化せずにヘッダで初期化する?
		//再追加したときに初期化したいからこのままでいい
		void ObjectInitialize();

#pragma region 加算
		virtual void AddPosition(const Vector3& vec) { position += vec; };
#pragma endregion


#pragma region セット


		virtual void SetPosition(const Vector3& position) { this->position = position; }
		//void SetVelocity(const Vector3& velocity) { this->velocity = velocity; }

		//void SetAcceleration(const Vector3& acc) { acceleration = acc; }
		void SetForce(const Vector3& force) { this->force = force; }
		void SetMass(const float mass) { this->mass = mass; }

		static void SetGravutationalAcceleration(const float acc) { gravutationalAcc = acc; };
#pragma endregion

#pragma region ゲット


		Vector3 GetPosition()const { return position; }
		Vector3 GetVelocity()const { return velocity; }
		Vector3 GetAcceleration()const { return acceleration; }
		Vector3 GetForce()const { return force; }
		float GetMass()const { return mass; }
		short GetSortNumber() const { return sortNumber; }

		static float GetGravutationalAcceleration() { return gravutationalAcc; };

		//オブジェクトマネージャーから削除するかどうかのフラグを返す
		bool GetEraseManager()const { return eraseManager; }


#pragma endregion


#pragma region 判定用関数


		//判定用関数
		CollisionDetectionFlag GetCollisionFlag() const { return collisionFlag; }
		std::vector<SphereData> GetSphereData() const { return sphereData; }
		std::vector<BoxData> GetBoxData() const { return boxData; }
		std::vector<Segment3DData> GetSegmentData() const { return segment3DData; }
		std::vector<PlaneData> GetPlaneData() const { return planeData; }
		std::vector<BoardData> GetBoardData()const { return boardData; }
		std::vector<CapsuleData>GetCapsuleData() const { return capsuleData; }

		//ここ参照取得じゃなくてSetにする?
		//そもそも持たせない。Hit関数で渡す
		void SetSphereCalcResult(const SphereCalcResult& result, const UINT index) { sphereData[index].SetCalcResult(result); }
		void SetBoxCalcResult(const BoxCalcResult& result, const UINT index) { boxData[index].SetCalcResult(result); }
		void SetSegmentCalcResult(const Segment3DCalcResult& result, const UINT index) { segment3DData[index].SetCalcResult(result); }
		void SetBoardCalcResult(const BoardCalcResult& result, const UINT index) { boardData[index].SetCalcResult(result); }
		void SetCapsuleCalcResult(const Segment3DCalcResult& result, const UINT index)
		{ capsuleData[index].GetRefSegment3DData().SetCalcResult(result); }

		//Vector3& GetLineSegmentHitPosition(const int num);
		//Vector3& GetBoardHitPosition(const int num);
		//BoxHitDirection& GetSphereBoxHitDistance(const int num) { return sphereData[num].boxHitDistance; }
		//BoxHitDirection& GetBoxBoxHitDistance(const int num) { return boxData[num].boxHitDistance; }



#ifdef _DEBUG
		static void CreateCollisionCheckModelPipelineState();

		//衝突確認用モデル生成、削除
		void CreateCollisionCheckModel();
		//衝突確認用モデルのデータセット
		void SetCollisionCheckModelData();
		//衝突確認用モデルの描画
		void DrawCollisionCheckModel();
#endif // _DEBUG

#pragma endregion


	};
}