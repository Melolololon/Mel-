#include "PhysicsTestObject.h"
#include"ModelObject.h"
#include"ModelData.h"
//‚²”ÑH‚×‚½‚ç‚â‚é‚±‚Æ
//‹… ‚Ìƒ‚ƒfƒ‹—pˆÓ


PhysicsTestObject::PhysicsTestObject(const Vector3& pos, const Vector3& vel)
{
	model = std::make_unique<ModelObject>(ModelData::Get("ball"), nullptr);
	position = pos;
	velocity = vel;
	calcPhysics = true;

	sphereData.resize(1);
	collisionFlag.sphere = true;

	mass = 100.0f;
}

void PhysicsTestObject::Update()
{
	position += velocity;
	model->SetPosition(position);
	sphereData[0].position = position;
	sphereData[0].r = 2.0f;
}

void PhysicsTestObject::Draw()
{
	model->Draw();
}
