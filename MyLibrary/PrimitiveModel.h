#pragma once
#include"Model.h"
class PrimitiveModel :public Model
{
protected:
	
	//頂点
	std::vector<std::vector<Vertex>> vertices;
	//インデックス
	std::vector<std::vector<USHORT>> indices;

	


public:
	PrimitiveModel();
	virtual ~PrimitiveModel();
};

