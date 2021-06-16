#pragma once
#include"ModelData.h"

//‚±‚±‚¶‚á‚È‚­‚Äƒ‚ƒfƒ‹‚ÌŠÇ—‚Ì•û‚Å¶¬‚·‚é?

class PrimitiveModelData :public ModelData
{
public:
	enum PrimityveModelType
	{
		PRIMITYVE_TYPE_BOX,
	};

private:
	std::vector<Vertex>vertices;

	//¶¬ŠÖ”
	static void CreateBox();
public:

	PrimitiveModelData();
	~PrimitiveModelData();


	static void Initialize();



};

