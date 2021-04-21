#pragma once
#include"Model.h"

//PrimitibModelを継承すると、プリミティブモデルを作成する関数に
//このクラスを渡せてしまうため、Modelに変更

class ObjModel:public Model
{
private:


	//頂点
	std::vector<std::vector<Vertex>> vertices;
	//インデックス
	std::vector<std::vector<USHORT>> indices;

	BoneData boneConstData;


	//[.obj内のオブジェクトごと][頂点ごと]
	std::vector<std::vector<int>> objBoneNums;//頂点に割り当てられているボーン番号
	//[モデルごと(keyでアクセス)][ボーンごと]
	 std::vector<DirectX::XMFLOAT3> objBonePositions;//ボーン座標


	//[ボーンごと](オブジェクトごとに変える必要なさそうなので、こうしてる)
	std::vector<ParentBoneData> parentBoneData;//親ボーンと影響度


public:
	ObjModel();
	~ObjModel();
};

