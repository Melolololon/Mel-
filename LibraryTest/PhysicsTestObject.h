#pragma once
#include"GameObject.h"
#include"ModelObject.h"
#include"Sprite3D.h"
#include"FrameTimer.h"
class PhysicsTestObject :public GameObject
{
	std::unique_ptr<ModelObject>model;

	FrameTimer deadTimer;

public:
	PhysicsTestObject(const Vector3& pos ,const Vector3& vel);
	void Update()override;
	void Draw()override;
	void Hit
	(
		const GameObject* const  object,
		const CollisionType collisionType,
		const int arrayNum,
		const CollisionType hitObjColType,
		const int hitObjArrayNum
	)override;


};

