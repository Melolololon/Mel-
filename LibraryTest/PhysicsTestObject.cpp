#include "PhysicsTestObject.h"
#include"ModelObject.h"
#include"ModelData.h"
//‚²”ÑH‚×‚½‚ç‚â‚é‚±‚Æ
//‹… ‚Ìƒ‚ƒfƒ‹—pˆÓ


PhysicsTestObject::PhysicsTestObject(const Vector3& pos, const Vector3& vel)
{
	model = std::make_unique<ModelObject>(ModelData::Get("ball"), nullptr);
	model->SetScale(5);

	position = pos;
	velocity = vel;
	calcPhysics = true;

	sphereData.resize(1);
	collisionFlag.sphere = true;

	force = 0.02f * velocity;
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
	sphereData[0].r = 5.0f;

	if(deadTimer.GetNowTime() >= 60 * 5)
	{
		eraseManager = true;
	}
}

void PhysicsTestObject::Draw()
{
	model->Draw();
}
