#pragma once
#include"ModelData.h"
class ObjModelData :public ModelData
{
private:
	std::vector<std::vector<ObjAnimationVertex>>vertices;
	//スムーズシェーディング用法線
	std::vector<std::vector<DirectX::XMFLOAT3>> smoothNormal;


	//[obj内のオブジェクト分]スムーズシェーディングの計算用データ
	std::vector< std::unordered_map < USHORT, std::vector<USHORT> >>smoothData;

	//[.obj内のオブジェクトごと][頂点ごと]
	std::vector<std::vector<int>> objBoneNums;//頂点に割り当てられているボーン番号
	//[ボーンごと]
	std::vector<Vector3> objBonePositions;//ボーン座標

public:

	bool Load(const std::string& path);

	static bool Initialize();


#pragma region セット

#pragma endregion


#pragma region ゲット
	const std::vector<Vector3>&  GetBonePositions() const { return objBonePositions; }


#pragma endregion

};

