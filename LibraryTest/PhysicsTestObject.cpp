#include "PhysicsTestObject.h"
#include"ModelObject.h"
#include"ModelData.h"
//‚²”ÑH‚×‚½‚ç‚â‚é‚±‚Æ
//‹… ‚Ìƒ‚ƒfƒ‹—pˆÓ


PhysicsTestObject::PhysicsTestObject(const Vector3& pos, const Vector3& force)
{
	model = std::make_unique<ModelObject>(ModelData::Get("ball"), nullptr);
	model->SetScale(5);

	position = pos;
	velocity = 0;
	calcPhysics = true;

	sphereData.resize(1);
	collisionFlag.sphere = true;

	this->force = force;
	mass = 100.0f;
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
	sphereData[0].r = 5.0f;
}

void PhysicsTestObject::Draw()
{
	model->Draw();
}
