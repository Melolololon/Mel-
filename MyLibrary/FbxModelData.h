#pragma once
#include"ModelData.h"

class FbxModelData:public ModelData
{
private:
	std::vector<std::vector<FbxVertex>>vertices;
	//スムーズシェーディング用法線
	std::vector<std::vector<DirectX::XMFLOAT3>> smoothNormal;


	//[obj内のオブジェクト分]スムーズシェーディングの計算用データ
	std::vector< std::unordered_map < USHORT, std::vector<USHORT> >>smoothData;



public:

	bool Load(const std::string& path);

	static void Initialize();
};

