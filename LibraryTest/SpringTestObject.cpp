#include "SpringTestObject.h"
#include"Physics.h"
#include"LibMath.h"
#include"PhysicsTestObject.h"

SpringTestObject::SpringTestObject(SpringTestObject* preObject) 
	: preObject(preObject)
{
	model = std::make_unique<ModelObject>(ModelData::Get("ball"), nullptr);
	
	if(preObject)
	{
		position = preObject->GetPosition() + Vector3(0, -DISTANCE, 0);
	}
	else
	{
		position = Vector3(0, 50, 0);
	}
	
	velocity = 0;

	calcPhysics = false;
	force = 0;
	mass = 1.0f;

	sphereData.resize(1);
	collisionFlag.sphere = true;

	model->SetScale(modelScale);
}

void SpringTestObject::Update()
{
	if (!preObject)
	{
		float speed = 1.2f;
		if (Input::KeyState(DIK_W))position.y += speed;
		if (Input::KeyState(DIK_S))position.y -= speed;
		if (Input::KeyState(DIK_A))position.x -= speed;
		if (Input::KeyState(DIK_D))position.x += speed;
		model->SetPosition(position);
		sphereData[0].position = position;
	}

	sphereData[0].position = position;
	sphereData[0].r = modelScale.x;
}

void SpringTestObject::Draw()
{
	model->Draw();
}

void SpringTestObject::CalcSpring()
{
	if (!preObject)return;

	std::unique_ptr<Vector3> nextPos;
	if (nextObject)
	{
		nextPos = std::make_unique<Vector3>();
		*nextPos = nextObject->GetPosition();
	}
	std::unique_ptr<Vector3> prePos;
	if (preObject)
	{
		prePos = std::make_unique<Vector3>();
		*prePos = preObject->GetPosition();
	}

	velocity = Physics::CalcSpringVelocity
	(
		position,
		velocity,
		nextPos.get(),
		prePos.get(),
		DISTANCE,
		mass,
		0.2,//d—Í‰Á‘¬“x
		0.01,//‚Î‚Ë’è”
		0.1//”S«’ïRŒW”
	);

	position += velocity;
	model->SetPosition(position); 
	sphereData[0].position = position;
}





