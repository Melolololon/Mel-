#include "SpringTestObject.h"
#include"Physics.h"
#include"LibMath.h"
#include"PhysicsTestObject.h"

SpringTestObject::SpringTestObject(const Vector3& pos,const Vector3& rootPos)
{
	model = std::make_unique<ModelObject>(ModelData::Get("ball"), nullptr);
	rootModel = std::make_unique<ModelObject>(ModelData::Get("ball"), nullptr);
	position = pos;
	this->rootPos = rootPos;
	velocity = 0;
	distance = LibMath::CalcDistance3D(pos, rootPos);

	calcPhysics = true;
	force = 0;
	mass = 1.0f;

	sphereData.resize(1);
	collisionFlag.sphere = true;


	rootModel->SetPosition(rootPos);
}

void SpringTestObject::Update()
{
	sphereData[0].position = position;
	sphereData[0].r = 1.0f;
}

void SpringTestObject::Draw()
{
	model->Draw();
	rootModel->Draw();
}

void SpringTestObject::CalcSpring()
{
	velocity = Physics::CalcSpringVelocity
	(
		position,
		velocity,
		rootPos,
		distance,
		mass,
		0.2,//d—Í‰Á‘¬“x
		0.01,//‚Î‚Ë’è”
		0.1//”S«’ïRŒW”
	);

	position += velocity;
	model->SetPosition(position); 
	sphereData[0].position = position;
}

void SpringTestObject::SetRootPosition(const Vector3& pos)
{
	rootPos = pos;
	rootModel->SetPosition(pos);
}




