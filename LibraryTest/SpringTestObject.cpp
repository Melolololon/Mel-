#include "SpringTestObject.h"
#include"Physics.h"
#include"LibMath.h"
#include"PhysicsTestObject.h"
#include"HurikoGame.h"
#include"Random.h"

UINT SpringTestObject::score = 0;

SpringTestObject::SpringTestObject(SpringTestObject* preObject) 
	: preObject(preObject)
{
	model = std::make_unique<ModelObject>(ModelData::Get("ball"), nullptr);
	
	if(preObject)
	{
		position = preObject->GetPosition() + Vector3(0, -15, 0);
	}
	else
	{
		position = Vector3(0, 80, 0);
	}
	
	velocity = 0;

	calcPhysics = true;
	force = 0;
	mass = 1.0f;

	sphereData.resize(1);
	collisionFlag.sphere = true;

	model->SetScale(modelScale);
	sphereData[0].position = position;
	sphereData[0].r = modelScale.x;



	if (!preObject) 
	{
		calcPhysics = false;
		const float SPEED = 1.0f;
		velocity = Vector3::Normalize(Vector3
		(
			Random::GetRandomFloatNumberRangeSelect(-1.0f, 1.0f, 1),
			0,
			Random::GetRandomFloatNumberRangeSelect(-1.0f, 1.0f, 1)
		)) * SPEED;
	}
}

void SpringTestObject::Update()
{
	if (!preObject)
	{
#pragma region ˆÚ“®
		const Vector3 MAX_POS = HurikoGame::GetFieldSize() / 2;
		if (position.x >= MAX_POS.x
			|| position.x <= -MAX_POS.x
			|| position.z >= MAX_POS.z
			|| position.z <= -MAX_POS.z)
		{
			velocity *= -1;
		}
#pragma endregion
		position += velocity;

		model->SetPosition(position);
		sphereData[0].position = position;
	}

	CalcSpring();
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



void SpringTestObject::Hit
(
	const GameObject* const  object,
	const CollisionType collisionType,
	const int arrayNum,
	const CollisionType hitObjColType,
	const int hitObjArrayNum
)
{
	if (typeid(PhysicsTestObject ) == typeid(*object))
	{
		score++;
	}
}

