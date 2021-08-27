#include "CollisionTestObject.h"
#include"TextWrite.h"

#include"ModelData.h"
#include"Texture.h"
#include"Input.h"
#include"LibMath.h"
MelLib::ADSAMaterial mtl;
bool first = false;

CollisionTestObject::CollisionTestObject(const MelLib::Vector3& pos, const bool inputMove) : inputFlag(inputMove)
{

	position = pos;
	speed = 0.25f;
	model[0] = std::make_unique<MelLib::ModelObject>(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), nullptr);
	model[1] = std::make_unique<MelLib::ModelObject>(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), nullptr);
	model[0]->SetScale(9);
	model[1]->SetScale(9);
	
	//マテリアルテスト
	if(!first)
	{
		MelLib::DrawData data = MelLib::PipelineState::GetDefaultDrawData(MelLib::PipelineStateType::MODEL);
		data.drawMode = MelLib::DrawMode::SOLID;
		first = true;
		mtl.Create(data);
		mtl.SetColor(MelLib::Color(255, 0, 0, 255));

		mtl.SetTexture(MelLib::Texture::Get("t"));
	}

	model[0]->SetMaterial(&mtl,0);

	/*collisionFlag.sphere = true;
	sphereData.resize(1);
	sphereData[0].r = 7.0f;*/

	/*collisionFlag.box = true;
	boxData.resize(1);
	boxData[0].size = 4;*/
	
	//boxCalcResult.resize(1);

	collisionFlag.capsule = true;
	capsuleData.resize(1);
	/*capsuleData[0].r = 3.0f;
	capsuleData[0].segmentData.angle = 0;*/
	capsuleData[0].SetRadius(3.0f);

	
	/*collisionFlag.lineSegment = true;
	lineSegmentData.resize(1);
	lineSegmentCalcResult.resize(1);*/



	model[0]->MeshCat();
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

	capsuleData[0].GetRefSegment3DData().SetRotatePoint(1.0f);

	if (inputFlag)
	{
		capsuleData[0].GetRefSegment3DData().
			SetPosition(MelLib::Value2<MelLib::Vector3>
				(position + MelLib::Vector3(0, 7, 0), MelLib::Vector3(0, -7, 0)));
	}
	else
	{
		capsuleData[0].GetRefSegment3DData().
			SetPosition(MelLib::Value2<MelLib::Vector3>
				(position + MelLib::Vector3(0, 7, 0), position + MelLib::Vector3(0, -7, 0)));
	}
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
	//boxData[0].position = position;
	
	if(MelLib::Input::KeyState(DIK_LEFT))angle.z += 3.0f;
	if(MelLib::Input::KeyState(DIK_RIGHT))angle.z -= 3.0f;
	if(MelLib::Input::KeyState(DIK_UP))angle.x += 3.0f;
	if(MelLib::Input::KeyState(DIK_DOWN))angle.x -= 3.0f;
	/*MelLib::Camera::Get()->SetAngle(angle);
	MelLib::Camera::Get()->SetCameraToTargetDistance(20.0f);
	MelLib::Camera::Get()->SetRotatePoint(MelLib::Camera::RotatePoint::ROTATE_POINT_TARGET_POSITION);
	MelLib::Camera::Get()->SetRotateCriteriaPosition(0);*/


	capsuleData[0].GetRefSegment3DData().SetAngle(angle);

	//model[0]->SetAngle(angle);
	//model[1]->SetAngle(angle);*/

	model[0]->SetScale(MelLib::Vector3(9, 9, 9));
	if(MelLib::Input::KeyState(DIK_F))
	{

		model[0]->SetScale(MelLib::Vector3(9, 9, 0));
	}
	if (MelLib::Input::KeyState(DIK_G))
	{

		model[0]->SetScale(MelLib::Vector3(9, 9, 0.0001));
	}
}

void CollisionTestObject::Draw()
{
	model[0]->Draw();
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
