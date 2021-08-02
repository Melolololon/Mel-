#include "CollisionTestObject.h"
#include"TextWrite.h"

#include"ModelData.h"

CollisionTestObject::CollisionTestObject(const Vector3& pos, const bool inputMove) : inputMove(inputMove)
{
	position = pos;
	speed = 0.25f;
	model = std::make_unique<ModelObject>(ModelData::Get("ball"), nullptr);

	/*collisionFlag.sphere = true;
	sphereData.resize(1);
	sphereData[0].r = 1.0f;*/

	collisionFlag.box = true;
	boxData.resize(1);
	boxData[0].size = 1;
	
	boxCalcResult.resize(1);
	
}

void CollisionTestObject::Update()
{
	hitFlag = false;

	velocity = 0;

	if (inputMove) 
	{
		if (Input::KeyState(DIK_W))velocity.y = 1.0f;
		if (Input::KeyState(DIK_S))velocity.y = -1.0f;
		if (Input::KeyState(DIK_D))velocity.x = 1.0f;
		if (Input::KeyState(DIK_A))velocity.x = -1.0f;
		position += velocity * speed;
	}
	model->SetPosition(position);
	
	//sphereData[0].position = position;
	boxData[0].position = position;
	
	
}

void CollisionTestObject::Draw()
{
	model->Draw();

	TextWrite::Draw(0, Color(255, 255, 255, 255), std::to_wstring(hitFlag), "test");
}

void CollisionTestObject::Hit(const GameObject* const object, const CollisionType collisionType, const int arrayNum, const CollisionType hitObjColType, const int hitObjArrayNum)
{
	hitFlag = true;
}
