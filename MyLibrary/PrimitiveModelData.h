#pragma once
#include"ModelData.h"

//��������Ȃ��ă��f���̊Ǘ��̕��Ő�������?

class PrimitiveModelData :public ModelData
{
public:
	enum PrimityveModelType
	{
		PRIMITYVE_TYPE_BOX,
	};

private:
	std::vector<Vertex>vertices;

	//�����֐�
	static void CreateBox();
public:

	PrimitiveModelData();
	~PrimitiveModelData();


	static void Initialize();



};

