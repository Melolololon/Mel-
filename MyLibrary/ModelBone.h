#pragma once
#include"Object.h"
class ModelBone:public Object
{
private:

public:
	ModelBone(const Color& color, const ModelData& modelData);
	~ModelBone();

	void initialize()override;
	void update()override;
	void draw()override;

};

