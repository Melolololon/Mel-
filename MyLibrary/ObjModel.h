#pragma once
#include"Model.h"
#include"Texture.h"

//objの頂点構造体や、ボーンセット関数を使いつつ、
//さらに頂点にデータを持たせたい場合は、
//これを継承して、Objの頂点と持たせたいデータを混ぜた自作構造体を作り、
//それを使ってmapする
//そうすれば、データ足しつつ、ボーンセット関数、マップ関数を使うことができる
//値は、Objの頂点と追加データで配列を分けて管理してもらう


class ObjModel:public Model
{
private:

	static PipelineState defaultObjPipeline;

	std::string materialFileName;

	//頂点
	std::vector<std::vector<ObjAnimationVertex>> vertices;
	std::vector<std::vector<DirectX::XMFLOAT3>> smoothNormal;

	//[obj内のオブジェクト分]スムーズシェーディングの計算用データ
	std::vector< std::unordered_map < USHORT, std::vector<USHORT> >>smoothData;

	//ボーンの座標とか
	BoneData boneConstData;
	//[.obj内のオブジェクトごと][頂点ごと]
	std::vector<std::vector<int>> objBoneNums;//頂点に割り当てられているボーン番号
	//[モデルごと(keyでアクセス)][ボーンごと]
	 std::vector<DirectX::XMFLOAT3> objBonePositions;//ボーン座標
	//[ボーンごと](オブジェクトごとに変える必要なさそうなので、こうしてる)
	std::vector<ParentBoneData> parentBoneData;//親ボーンと影響度


	std::vector<std::unique_ptr<Texture>>textures;

	/// <summary>
/// objモデルの頂点を読み込みます。
/// </summary>
/// <param name="path">モデルのパス</param>
/// <param name="loadUV">uvを読み込むか</param>
/// <param name="loadNormal">normalを読み込むか</param>
	void loadModelVertices
	(
		const std::string& path,
		const bool loadUV,
		const bool loadNormal
	);

	void loadModelMaterial
	(
		const std::string& directryPath,
		const int createNum,
		const size_t constDataSize = 0
	);
public:
	ObjModel();
	virtual ~ObjModel();


	/// <summary>
	/// objモデルを読み込みます。
	/// </summary>
	/// <param name="path">パス</param>
	/// <param name="loadUV">UVを読み込むかどうか</param>
	/// <param name="createNum">生成数</param>
	/// <param name="constDataSize">シェーダーに送る利用者が用意したデータのバイト数</param>
	void loadModel
	(
		const std::string& path,
		const bool loadUV,
		const int createNum,
		const size_t constDataSize = 0
	);


	static bool initialize();


	static void mapBoneData();
};

