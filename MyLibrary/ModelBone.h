#pragma once
#include"GameObject.h"
class ModelBone:public GameObject
{
private:

public:
	ModelBone(const Color& color, const ModelDataAndKey& modelData);
	~ModelBone();

	void Initialize()override;
	void Update()override;
	void Draw()override;

};

