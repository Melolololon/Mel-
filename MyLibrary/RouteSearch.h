#pragma once
#include"Vector.h"
#include"ModelObject.h"

namespace MelLib 
{
	//サイズ持たせるようにする?
	//コストも進行不可能オブジェクトとの判定時に計算するようにする

	//Aスターのノード
	struct AStarNode
	{

		using UINT = unsigned int;

		
		//座標
		Vector2 position = 0;

		//コスト
		UINT cost = 1;

		//計算結果(ステップ + 距離 + コスト)
		UINT calcNum = UINT_MAX;
		

		//以下探索用変数

		//配列のインデックス
		int indexX = INT_MAX;
		int indexY = INT_MAX;

		//close配列のインデックス
		int closeIndex = INT_MAX;

		//スタートからの距離
		int stepNum = 0;

		//前のノード
		AStarNode* previousNode = nullptr;

		bool openFlag = false;
		bool closeFlag = false;

		//進行不能オブジェクトと重なっているノード
		bool hitObjectNode = false;

		
#ifdef _DEBUG

		/*AStarNode();
		~AStarNode();*/

		//static std::vector<AStarNode*>pNodes;

		////マス目
		//ModelObject nodeModel;

		//void Draw();
		//static void AllDraw();

#endif // _DEBUG


	};

	//経路探索クラス
	class RouteSearch
	{
		using UINT = unsigned int;
		//今度でいいから、プレイヤーや敵の大きさを考慮して経路を計算できるようにする


#pragma region 最短経路

		/// <summary>
		/// 引数を基準にノードの座標をセットします。
		/// </summary>
		/// <param name="leftUpPos">左上座標</param>
		/// <param name="rightDownPos">右下座標</param>
		/// <param name="nodeNumX">横分割数</param>
		/// <param name="nodeNumY">縦分割数</param>
		/// <param name="nodes">ノードのvector(sizeは0でよい)</param>
		/// <param name="upPlus">上方向がプラスかどうか</param>
		static void SetAStarNodePosition
		(
			const Vector2& leftUpPos,
			const Vector2& rightDownPos,
			const UINT& nodeNumX,
			const UINT& nodeNumY,
			std::vector< std::vector<AStarNode>>& nodes,
			const bool upPlus
		);

		//この関数でコストが1じゃないオブジェクトとそのコストを渡すようにする?
		/// <summary>
		/// ノードが進行不能オブジェクトにヒットしてるかを確認します。
		/// </summary>
		/// <param name="hitObjectsPos">障害物の座標のvector</param>
		/// <param name="hitObjectsSize">障害物のサイズのvector</param>
		/// <param name="nodes">ノードのvector(SetAStarNodePositionに渡した後の配列)</param>
		static void SetAStarNodeHitObjectNodeFlag
		(
			const std::vector<Vector2>& hitObjectsPos,
			const std::vector<Vector2>& hitObjectsSize,
			std::vector< std::vector<AStarNode>>& nodes
		);

		/// <summary>
		/// 渡されたデータをもとに最短経路を求めます。
		/// </summary>
		/// <param name="startPos">スタート地点の座標</param>
		/// <param name="endPos">ゴール地点の座標</param>
		/// <param name="nodes">ノードのvector(SetAStarNodeHitObjectNodeFlagに渡した後の配列)</param>
		/// <param name="routeVector">ゴールまでのルートを格納するvector(sizeは0でよい)</param>
		/// <returns>探索が成功したかどうか</returns>
		static bool GetAStarCalcResult
		(
			const Vector2& startPos,
			const Vector2& endPos,
			std::vector< std::vector<AStarNode>>& nodes,
			std::vector<Vector2>& routeVector
		);

#pragma endregion

	};
}

