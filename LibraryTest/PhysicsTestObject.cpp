#include "PhysicsTestObject.h"
#include"ModelObject.h"
#include"ModelData.h"
#include"SpringTestObject.h"

PhysicsTestObject::PhysicsTestObject(const Vector3& pos, const Vector3& vel)
{
	model = std::make_unique<ModelObject>(ModelData::Get("ball"), nullptr);
	model->SetScale(3);

	position = pos;
	velocity = vel;
	calcPhysics = true;

	sphereData.resize(1);
	collisionFlag.sphere = true;

	force = 0.007f * velocity;
	mass = 1.0f;

	deadTimer.SetStopFlag(false);
}

void PhysicsTestObject::Update()
{
	//if(force.x > 0)
	//{
	//	force -= Vector3(0.001f, 0.001f, 0);
	//}
	//else
	//{
	//	force = 0;
	//}
	
	//position += velocity;
	model->SetPosition(position);
	sphereData[0].position = position;
	sphereData[0].r = scale.x;

	if(deadTimer.GetNowTime() >= 60 * 5)
	{
		eraseManager = true;
	}
}

void PhysicsTestObject::Draw()
{
	model->Draw();
}

void PhysicsTestObject::Hit
(
	const GameObject* const  object,
	const CollisionType collisionType,
	const int arrayNum,
	const CollisionType hitObjColType,
	const int hitObjArrayNum
)
{
  	if(typeid(SpringTestObject) == typeid(*object))
	{
		eraseManager = true;
	}
}
