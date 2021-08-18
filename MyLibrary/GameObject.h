#pragma once
#include<typeinfo>
#include<vector>
#include"Library.h"
#include"Vector.h"
#include"CollisionType.h"

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

		//カプセル引き伸ばすと形崩れるから、球と円柱組み合わせて表示したほうがいいかも
		//判定のデータの変数は今のまま(角度消すと円柱を組み合わせるのが大変になる)
		std::array<std::vector<ModelObject>, 3>capsuleModelObjects;
#endif // _DEBUG

	protected:

		Vector3 position = 0;
		Vector3 velocity = 0;
		Vector3 speed = 0;



#pragma region 物理関係

		//物理的な挙動を行うための計算を行うかどうか。
		bool calcPhysics = false;

		//加速度
		Vector3 acceleration = 0;
		//物体が動く力
		Vector3 force = 0;
		//重さ
		float mass = 100.0f;
#pragma endregion

#pragma region 判定データ
		CollisionDetectionFlag collisionFlag;

		std::vector<SphereData> sphereData;
		std::vector<SphereCalcResult> sphereCalcResult;

		std::vector<BoxData> boxData;
		std::vector<BoxCalcResult> boxCalcResult;


		std::vector<Segment3DData> lineSegmentData;
		std::vector<Segment3DCalcResult> lineSegmentCalcResult;

		std::vector<RayData> layData;
		std::vector<PlaneData>planeData;

		std::vector<BoardData>boardData;
		std::vector<BoardCalcResult>boardCalcResult;

		std::vector<CapsuleData>capsuleData;


#pragma endregion


		//ソート用数値。ソート順を自分で調整するための変数
		short sortNumber = 0;

		//生死フラグ(これがtrueになると、オブジェクトマネージャーから除外される)
		bool eraseManager = false;
		

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




		//void CalcHitPhysics(GameObject* hitObject,const Vector3& hutPreVelocity,const CollisionType& collisionType);

		//virtual const void* GetPtr()const;



		//確実に初期値が決まっている変数(eraseManagerなど)を初期化する変数(初期化忘れ防止用)
		//これで初期化せずにヘッダで初期化する?
		//再追加したときに初期化したいからこのままでいい
		void ObjectInitialize();

		//物理に基づいた移動関係の計算を行う関数
		void CalcMovePhysics();

#pragma region セット


		void SetPosition(const Vector3& position) { this->position = position; }
		void SetVelocity(const Vector3& velocity) { this->velocity = velocity; }
#pragma endregion

#pragma region ゲット


		Vector3 GetPosition()const { return position; }
		Vector3 GetVelocity()const { return velocity; }
		Vector3 GetAcceleration()const { return acceleration; }
		Vector3 GetForce()const { return force; }
		float GetMass()const { return mass; }
		bool GetCalcPhysicsFlag()const { return calcPhysics; }
		short GetSortNumber() const { return sortNumber; }

		//オブジェクトマネージャーから削除するかどうかのフラグを返す
		bool GetEraseManager() { return eraseManager; }
#pragma endregion


#pragma region 判定用関数


		//判定用関数
		CollisionDetectionFlag GetCollisionFlag() { return collisionFlag; }
		std::vector<SphereData> GetSphereData() { return sphereData; }
		std::vector<BoxData> GetBoxData() { return boxData; }
		std::vector<Segment3DData> GetSegmentData() { return lineSegmentData; }
		std::vector<PlaneData> GetPlaneData() { return planeData; }
		std::vector<BoardData> GetBoardData() { return boardData; }
		std::vector<CapsuleData>GetCapsuleData() { return capsuleData; }

		//ここ参照取得じゃなくてSetにする?
		std::vector<SphereCalcResult>& GetSphereCalcResult() { return sphereCalcResult; }
		std::vector<BoxCalcResult>& GetBoxCalcResult() { return boxCalcResult; }
		std::vector<Segment3DCalcResult>& GetSegmentCalcResult() { return lineSegmentCalcResult; }
		std::vector<BoardCalcResult>& GetBoardCalcResult() { return boardCalcResult; }

		//Vector3& GetLineSegmentHitPosition(const int num);
		//Vector3& GetBoardHitPosition(const int num);
		//BoxHitDirection& GetSphereBoxHitDistance(const int num) { return sphereData[num].boxHitDistance; }
		//BoxHitDirection& GetBoxBoxHitDistance(const int num) { return boxData[num].boxHitDistance; }

		/// <summary>
		/// 判定計算用のデータと計算結果格納関数が
		/// </summary>
		void CheckCalcResultSize();

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