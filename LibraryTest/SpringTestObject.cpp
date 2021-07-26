#include "SpringTestObject.h"
#include"Physics.h"
SpringTestObject::SpringTestObject(const Vector3& pos,const Vector3& rootPos)
{
	model = std::make_unique<ModelObject>(ModelData::Get("ball"), nullptr);
	rootModel = std::make_unique<ModelObject>(ModelData::Get("ball"), nullptr);
	position = pos;
	this->rootPos = rootPos;
	velocity = 0;
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
		30.0f,
		1.0f,
		0.2,//重力加速度
		0.01,//ばね定数
		0.1//粘性抵抗係数
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
