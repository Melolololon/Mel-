#pragma once
#include<vector>
#include<DirectXMath.h>
#include"Vector.h"
#include"DirectXStruct.h"
//ポリゴンの頂点情報や、インデックスを取得するためのクラス
class CreatePolygon
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT2 = DirectX::XMFLOAT2;


private:
	int windowWidth;
	int windowHeight;

public:
	CreatePolygon(int window_width, int window_height);
	~CreatePolygon();

#pragma region 式を使った多角形作成
	/// <summary>
	/// 式を使って頂点を設定します(多角形作るやつ)
	/// </summary>
	/// <param name="r"></param>
	/// <param name="vertexNum">中心を除いた頂点数</param>
	/// <param name="tyusinX">中心座標X</param>
	/// <param name="tyusinY">中心座標Y</param>
	/// <returns></returns>
	std::vector<Vertex> setPolygonVertex( float r,  int vertexNum, XMFLOAT2 tyusin);
	
	/// /// <summary>
	/// インデックスを設定します
	/// </summary>
	/// <param name="vertexNum">中心を除いた頂点数</param>
	/// <returns></returns>
	std::vector<unsigned short> setPolygonIndex(int vertexNum);

#pragma region 三角錐

	/// <summary>
	/// 式を使って頂点を設定します
	/// </summary>
	/// <param name="r"></param>
	/// <param name="vertexNum">中心を除いた頂点数</param>
	/// <param name="tyusinX">中心座標X</param>
	/// <param name="tyusinY">中心座標Y</param>*	/// <returns></returns>
	std::vector<Vertex> getTriangularPyramidVertex(float r, int vertexNum, XMFLOAT3 tyusin,float up);

	/// /// <summary>
	/// インデックスを設定します
	/// </summary>
	/// <param name="vertexNum">中心を除いた頂点数</param>
	/// <returns></returns>
	std::vector<unsigned short> setTriangularPyramidIndex(int vertexNum);
#pragma endregion

#pragma region 正多角柱
	std::vector<Vertex> setPolygonalPrismVertex(float r, int vertexNum, XMFLOAT3 tyusin, float up);

	std::vector<unsigned short> setPolygonalPrismIndex(int vertexNum);


#pragma endregion

#pragma region 球

#pragma endregion


#pragma endregion


#pragma region スラッシュポリゴン

	std::vector<Vertex> setSlushPolygonVertex(XMFLOAT2 tyusin, XMFLOAT2 pos,int type);
	std::vector<unsigned short> setSlushPolygonIndex(int type);

#pragma endregion

#pragma region 四角形
	//2D3DともにBoardのほうを使ってます

#pragma region 四角形2Dポリゴン
	/// <summary>
	/// 四角形の頂点を設定します
	/// </summary>
	/// <param name="startPosX">左上座標X</param>
	/// <param name="startPosY">左上座標Y</param>
	/// <param name="endPosX">右下座標X</param>
	/// <param name="endPosY">右下座標Y</param>
	/// <returns></returns>
	std::vector<Vertex> setBoxPolygonVertex(XMFLOAT2 startPos, XMFLOAT2 endPos);
	
	/// <summary>
	/// インデックスを設定します
	/// </summary>
	/// <returns></returns>
	std::vector<unsigned short> setBoxPolygonIndex();



#pragma endregion

#pragma region 4頂点板ポリゴン
	std::vector<Vertex> setBoardPolygonVertex(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 pos3, XMFLOAT3 pos4);
	std::vector<unsigned short> setBoardPolygonIndex();



#pragma endregion

#pragma region 箱
	std::vector<Vertex> get3DBox(DirectX::XMFLOAT3 size);
	std::vector<unsigned short>  get3DBoxIndex();

	//頂点数が多いが、uvをちゃんと、自由に設定できる
	std::vector<Vertex> getVertexMany3DBox(DirectX::XMFLOAT3 size);
	std::vector<unsigned short>  getVertexMany3DBoxIndex();
#pragma endregion


#pragma endregion



};

