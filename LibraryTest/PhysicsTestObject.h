#pragma once
#include"GameObject.h"
class PhysicsTestObject :public GameObject
{
	static int CREATE_COUNT;
	int modelNum = 0;
public:
	PhysicsTestObject(const Vector3& pos ,const Vector3& vel);
	void Update()override;
	void Draw()override;
};

