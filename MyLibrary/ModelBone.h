#pragma once
#include"Object.h"
class ModelBone:public Object
{
private:

public:
	ModelBone(const Color& color, const ModelData& modelData);
	~ModelBone();

	void Initialize()override;
	void Update()override;
	void Draw()override;

};

