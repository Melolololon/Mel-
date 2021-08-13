#include "CollisionTestObject.h"
#include"TextWrite.h"

#include"ModelData.h"

MelLib::PBRMaterial mtl;
bool first = false;

CollisionTestObject::CollisionTestObject(const MelLib::Vector3& pos, const bool inputMove) : inputFlag(inputMove)
{
	position = pos;
	speed = 0.25f;
	model[0] = std::make_unique<MelLib::ModelObject>(MelLib::ModelData::Get("ball"), nullptr);
	model[1] = std::make_unique<MelLib::ModelObject>(MelLib::ModelData::Get("ball"), nullptr);
	

	//マテリアルテスト
	/*if(!first)
	{
		MelLib::DrawData data = MelLib::PipelineState::GetDefaultDrawData(MelLib::PipelineStateType::MODEL);

		first = true;
		mtl.Create(data);
	}

	model[0]->SetMaterial(&mtl,0);*/

	/*collisionFlag.sphere = true;
	sphereData.resize(1);
	sphereData[0].r = 7.0f;*/

	collisionFlag.box = true;
	boxData.resize(1);
	boxData[0].size = 4;
	
	boxCalcResult.resize(1);

	//collisionFlag.capsule = true;
	//capsuleData.resize(1);
	//capsuleData[0].r = 0.5f;
	
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

	//capsuleData[0].angle = 0;
	//capsuleData[0].length = 4.0f;
	//capsuleData[0].position = position;
	//model[0]->SetPosition(position + MelLib::Vector3(0, capsuleData[0].length/2, 0));
	//model[1]->SetPosition(position + MelLib::Vector3(0, -capsuleData[0].length / 2, 0));
	//

	//if(MelLib::Input::KeyState(DIK_SPACE) && inputFlag)
	//{
	//	model[0]->SetPosition(position + MelLib::Vector3(capsuleData[0].length / 2,0, 0));
	//	model[1]->SetPosition(position + MelLib::Vector3( -capsuleData[0].length / 2, 0,0));
	//	capsuleData[0].angle.z = 90;
	//}

	//sphereData[0].position = position;
	boxData[0].position = position;
	
	angle.y += 3.0f;
	/*model[0]->SetAngle(angle);
	model[1]->SetAngle(angle);*/
}

void CollisionTestObject::Draw()
{
	//model[0]->Draw();
	//model[1]->Draw();

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
