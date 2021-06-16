#pragma once
#include"ModelData.h"

//ここじゃなくてモデルの管理の方で生成する?

class PrimitiveModelData :public ModelData
{
public:
	enum PrimityveModelType
	{
		PRIMITYVE_TYPE_BOX,
	};

private:
	std::vector<Vertex>vertices;

	//生成関数
	static void CreateBox();
public:

	PrimitiveModelData();
	~PrimitiveModelData();


	static void Initialize();



};

