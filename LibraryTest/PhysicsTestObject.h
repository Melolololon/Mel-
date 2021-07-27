#pragma once
#include"GameObject.h"
#include"ModelObject.h"
class PhysicsTestObject :public GameObject
{
	std::unique_ptr<ModelObject>model;

public:
	PhysicsTestObject(const Vector3& pos ,const Vector3& vel);
	void Update()override;
	void Draw()override;
};

