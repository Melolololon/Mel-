#include "SpringTestObject.h"
#include"Physics.h"
SpringTestObject::SpringTestObject(const Vector3& pos)
{
	model = std::make_unique<ModelObject>(ModelData::Get("ball"), nullptr);
	rootModel = std::make_unique<ModelObject>(ModelData::Get("ball"), nullptr);
	position = pos;
	rootPos = position + Vector3(0, 30, 0);
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
	if(Input::KeyState(DIK_W))
	{
		rootPos.y += 0.2f;
	}
	
	if(Input::KeyState(DIK_S))
	{
		rootPos.y -= 0.2f;
	}
	
	if(Input::KeyState(DIK_A))
	{
		rootPos.x -= 0.2f;
	}
	
	if(Input::KeyState(DIK_D))
	{
		rootPos.x += 0.2f;
	}
	rootModel->SetPosition(rootPos);

	velocity = Physics::CalcSpringVelocity
	(
		position,
		velocity,
		rootPos,
		30.0f,
		1.0f,
		0.2,//�d�͉����x
		0.01,//�΂˒萔
		0.1//�S����R�W��
	);

	position += velocity;
	model->SetPosition(position);
}

void SpringTestObject::Draw()
{
	model->Draw();
	rootModel->Draw();
}
