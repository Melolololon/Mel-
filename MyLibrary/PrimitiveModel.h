#pragma once
#include"Model.h"
class PrimitiveModel :public Model
{
protected:
	
	//���_
	std::vector<std::vector<Vertex>> vertices;
	//�C���f�b�N�X
	std::vector<std::vector<USHORT>> indices;

	


public:
	PrimitiveModel();
	virtual ~PrimitiveModel();
};

