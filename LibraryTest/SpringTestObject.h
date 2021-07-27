#pragma once
#include <GameObject.h>
#include"ModelObject.h"

//�����Ȃ��悤
class SpringTestObject :
    public GameObject
{
private:
	std::unique_ptr<ModelObject>model;
	Vector3 rootPos;
	float distance = 0.0f;

	Vector3 modelScale = 5;
public:
	SpringTestObject(const Vector3& pos, const Vector3& rootPos);
	void Update()override;
	void Draw()override;
	void CalcSpring();

	void SetRootPosition(const Vector3& pos);

	
};

