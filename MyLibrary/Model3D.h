#pragma once
#include"Library.h"
#include"Vector.h"
#include"Color.h"
#include<DirectXMath.h>
#include<vector>
#include"DirectXStruct.h"

//とりあえずこれのポインタをキーにする

//これにバッファ持たせると、終了時に確実にバッファ解放できない。
//プログラム終了時に

//これをObjModelなどに継承する
//これを引数にしても、OBJのボーンとか引き出せない。
//セットとかの関数をここに移植するしかない
class Model3D
{
private:
	//[objにあるモデルごと][頂点ごと]
	std::vector<std::vector<Vertex>> vertices;
	std::vector<std::vector<USHORT>> indices;


	std::vector < ModelConstData>modelConstData;
	MaterialConstBuffData material;

	std::vector <Model3D* > parentModel;
	std::vector <int> parentModelNumbers;
public:
	Model3D();
	~Model3D();
};

