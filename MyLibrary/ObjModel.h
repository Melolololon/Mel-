#pragma once
#include"Model.h"

//PrimitibModelを継承すると、プリミティブモデルを作成する関数に
//このクラスを渡せてしまうため、Modelに変更

class ObjModel:public Model
{
private:
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

	//モデルファイルにあるオブジェクト数
	int loadFileObjectNum;
public:
	ObjModel();
	virtual ~ObjModel();

	/// <summary>
	/// objモデルの頂点を読み込みます。
	/// </summary>
	/// <param name="path">モデルのパス</param>
	/// <param name="loadUV">uvを読み込むか</param>
	/// <param name="loadNormal">normalを読み込むか</param>
	void loadModelVertices
	(
		const std::string& path,
		const bool& loadUV,
		const bool& loadNormal
	);

	void loadModelMaterial
	(
		const int& createNum,
		const size_t& constDataSize = 0
	);
};

