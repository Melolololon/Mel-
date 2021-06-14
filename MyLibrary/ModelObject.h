#pragma once
#include"Model.h"

class ModelObject
{
private:
	//モデル
	Model* model;

	//定数バッファ
	std::vector<std::vector<ComPtr<ID3D12Resource>>> constBuffer;//メイン(基本的な情報)
	std::vector<std::vector<ComPtr<ID3D12Resource>>> materialConstBuffer;//マテリアル
	std::vector<std::vector<ComPtr<ID3D12Resource>>> userConstBuffer;//ユーザー
	std::vector<std::vector<ComPtr<ID3D12Resource>>> modelConstBuffer;//モデル特有

	//定数にセットする座標などの値
	//[モデルごと][モデル内のオブジェクト数]
	std::vector <std::vector<ModelConstData>>modelConstDatas;
	//[モデル内のオブジェクトごと]
	std::vector<Material> materials;

public:

};

