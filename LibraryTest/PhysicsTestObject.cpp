#include "PhysicsTestObject.h"
#include"ModelObject.h"
#include"ModelData.h"
//‚²”ÑH‚×‚½‚ç‚â‚é‚±‚Æ
//‹… ‚Ìƒ‚ƒfƒ‹—pˆÓ

int PhysicsTestObject::CREATE_COUNT = 0;

PhysicsTestObject::PhysicsTestObject(const Vector3& pos, const Vector3& vel)
{
	ModelObject::Create(ModelData::Get("ball"), nullptr, "ball" + std::to_string(CREATE_COUNT));
	position = pos;
	velocity = vel;
	modelNum = CREATE_COUNT;
	CREATE_COUNT++;
	calcPhysics = true;

	sphereData.resize(1);
	collisionFlag.sphere = true;

	mass = 100.0f;
}

void PhysicsTestObject::Update()
{
	position += velocity;
	ModelObject::Get("ball" + std::to_string(modelNum))->SetPosition(position);
	sphereData[0].position = position;
	sphereData[0].r = 2.0f;
}

void PhysicsTestObject::Draw()
{
	ModelObject::Get("ball" + std::to_string(modelNum))->Draw();
}
