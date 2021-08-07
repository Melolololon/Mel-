#include "CollisionTestObject.h"
#include"TextWrite.h"

#include"ModelData.h"

CollisionTestObject::CollisionTestObject(const MelLib::Vector3& pos, const bool inputMove) : inputFlag(inputMove)
{
	position = pos;
	speed = 0.25f;
	model[0] = std::make_unique<MelLib::ModelObject>(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), nullptr);
	

	/*collisionFlag.sphere = true;
	sphereData.resize(1);
	sphereData[0].r = 1.0f;*/

	collisionFlag.box = true;
	boxData.resize(1);
	boxData[0].size = 4;
	
	boxCalcResult.resize(1);

	/*collisionFlag.capsule = true;
	capsuleData.resize(1);
	capsuleData[0].r = 1.0f;*/
	
	/*collisionFlag.lineSegment = true;
	lineSegmentData.resize(1);
	lineSegmentCalcResult.resize(1);*/
}

void CollisionTestObject::Update()
{
	
	hitFlag = false;

	velocity = 0;

	if (inputFlag) 
	{
		if (MelLib::Input::KeyState(DIK_W))velocity.y = 1.0f;
		if (MelLib::Input::KeyState(DIK_S))velocity.y = -1.0f;
		if (MelLib::Input::KeyState(DIK_D))velocity.x = 1.0f;
		if (MelLib::Input::KeyState(DIK_A))velocity.x = -1.0f;
		position += velocity * speed;
	}
	model[0]->SetPosition(position);
	
	//sphereData[0].position = position;
	boxData[0].position = position;
	
	angle.y += 3.0f;
	/*model[0]->SetAngle(angle);
	model[1]->SetAngle(angle);*/
}

void CollisionTestObject::Draw()
{
	//model[0]->Draw();

	MelLib::TextWrite::Draw(0, MelLib::Color(255, 255, 255, 255), std::to_wstring(hitFlag), "test");
}

void CollisionTestObject::Hit
(
	const GameObject* const object, 
	const MelLib::ShapeType3D collisionType, 
	const int arrayNum, 
	const MelLib::ShapeType3D hitObjColType, 
	const int hitObjArrayNum
)
{
	hitFlag = true;
}
