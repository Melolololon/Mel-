#include "SpringTestObject.h"
#include"Physics.h"
#include"LibMath.h"

SpringTestObject::SpringTestObject(const Vector3& pos,const Vector3& rootPos)
{
	model = std::make_unique<ModelObject>(ModelData::Get("ball"), nullptr);
	rootModel = std::make_unique<ModelObject>(ModelData::Get("ball"), nullptr);
	position = pos;
	this->rootPos = rootPos;
	velocity = 0;
	distance = LibMath::CalcDistance3D(pos, rootPos);

	/*calcPhysics = true;

	sphereData.resize(1);
	collisionFlag.sphere = true;

	force = Vector3(0.02f, 0.02f, 0);
	mass = 100.0f;*/

	rootModel->SetPosition(rootPos);
}

void SpringTestObject::Update()
{
	velocity = Physics::CalcSpringVelocity
	(
		position,
		velocity,
		rootPos,
		distance,
		1.0f,
		0.2,//d—Í‰Á‘¬“x
		0.01,//‚Î‚Ë’è”
		0.1//”S«’ïRŒW”
	);

	position += velocity;
	model->SetPosition(position);
}

void SpringTestObject::Draw()
{
	model->Draw();
	rootModel->Draw();
}

void SpringTestObject::SetRootPosition(const Vector3& pos)
{
	rootPos = pos;
	rootModel->SetPosition(pos);
}
