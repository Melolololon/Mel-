#pragma once
#include <GameObject.h>
#include"ModelObject.h"
class SpringTestObject :
    public GameObject
{
private:
	std::unique_ptr<ModelObject>model;
	std::unique_ptr<ModelObject>rootModel;
	Vector3 rootPos;

public:
	SpringTestObject(const Vector3& pos);
	void Update()override;
	void Draw()override;
};

