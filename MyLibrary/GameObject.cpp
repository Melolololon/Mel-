#include "GameObject.h"

#include"Physics.h"

using namespace MelLib;


#ifdef _DEBUG


//判定確認用モデルのパイプライン
PipelineState GameObject::collisionCheckModelPipelineState;

//マテリアル
ADSAMaterial GameObject::material;
#endif // _DEBUG

GameObject::GameObject()
{
}


GameObject::~GameObject()
{
}

//void GameObject::Initialize()
//{
//}

void GameObject::Update() 
{
}

void GameObject::Draw() 
{

}

void GameObject::Hit
(
	const GameObject* const  object,
	const ShapeType3D collisionType,
	const int arrayNum,
	const ShapeType3D hitObjColType,
	const int hitObjArrayNum
)
{
}
//
//const void* GameObject::GetPtr() const
//{
//	return nullptr;
//}

//void GameObject::CalcHitPhysics(GameObject* hitObject, const Vector3& hutPreVelocity, const CollisionType& collisionType)
//{
//
//	//switch (collisionType)
//	//{
//	//case CollisionType::COLLISION_SPHERE:
//	//	
//	//	velocity = Physics::CalcRepulsionVelocity
//	//	(
//	//	);
//	//	break;
//	//}
//}


void GameObject::ObjectInitialize() 
{
	eraseManager = false;


}

void GameObject::CalcMovePhysics()
{
	if (mass <= 0.0f)return;
	acceleration += force / mass;
	velocity += acceleration;
	position += velocity;
}

void MelLib::GameObject::ResizeCalcResult()
{
	if (sphereData.size() != sphereCalcResult.size()) sphereCalcResult.resize(sphereData.size());
	if (boxData.size() != boxCalcResult.size()) boxCalcResult.resize(boxData.size());
	if (lineSegmentData.size() != lineSegmentCalcResult.size()) lineSegmentCalcResult.resize(lineSegmentData.size());
	if (boardData.size() != boardCalcResult.size()) boardCalcResult.resize(boardData.size());
}
#ifdef _DEBUG



void MelLib::GameObject::CreateCollisionCheckModelPipelineState()
{
	DrawData pData = PipelineState::GetDefaultDrawData(PipelineStateType::MODEL);
	pData.drawMode = DrawMode::WIREFRAME;
	pData.cullMode = CullMode::NONE;

	ShaderDataSet set =
	{
		{L"LIB","",""},
		{L"LIB","",""},
		{L"LIB","",""},
		{L"LIB","",""},
		{L"LIB","",""}
	};

	collisionCheckModelPipelineState.CreatePipeline
	(
		pData,
		set,
		PipelineStateType::MODEL,
		nullptr,
		1
	);

	DrawData data = PipelineState::GetDefaultDrawData(PipelineStateType::MODEL);
	data.cullMode = CullMode::NONE;
	data.drawMode = DrawMode::WIREFRAME;
	
	material.Create(data);

}

void MelLib::GameObject::CreateCollisionCheckModel()
{
	//判定数に応じてモデルを生成したり削除したりします
	auto createOrDeleteModel = [](const size_t& dataNum,std::vector<ModelObject>& modelObjcts,const ShapeType3D type)
	{
		size_t objNum = modelObjcts.size();
		if(dataNum > objNum)
		{
			size_t addSize = dataNum - objNum;
			modelObjcts.resize(dataNum);

			//不足分生成
			for(int i = objNum; i < dataNum;i++)
			{
				modelObjcts[i].Create(ModelData::Get(type), nullptr);

				modelObjcts[i].SetMaterial(&material, 0);
			}
		}
		else if (dataNum < objNum)
		{
			modelObjcts.resize(objNum);
		}

	};

	//Box
	createOrDeleteModel(boxData.size(), boxModelObjects, ShapeType3D::BOX);
	
	//Sphere
	createOrDeleteModel(sphereData.size(), sphereModelObjects, ShapeType3D::BOX);

	//Capsule
	//球を作成
	createOrDeleteModel(capsuleData.size(), capsuleModelObjects[0], ShapeType3D::BOX);
	createOrDeleteModel(capsuleData.size(), capsuleModelObjects[1], ShapeType3D::BOX);
	//円注を作成
	createOrDeleteModel(capsuleData.size(), capsuleModelObjects[2], ShapeType3D::BOX);
}

void MelLib::GameObject::SetCollisionCheckModelData()
{
	//Box
	size_t dataNum = boxData.size();
	for(size_t i = 0; i < dataNum;i++)
	{
		boxModelObjects[i].SetScale(boxData[i].size);
		boxModelObjects[i].SetPosition(boxData[i].position);
	}

	//Sphere
	dataNum = sphereData.size();
	for (size_t i = 0; i < dataNum; i++)
	{
		sphereModelObjects[i].SetScale(sphereData[i].r);
		sphereModelObjects[i].SetPosition(sphereData[i].position);
	}

	//Capsule
	dataNum = capsuleData.size();
	for (size_t i = 0; i < dataNum; i++)
	{
		Value2<Vector3>lineSegmentPos = capsuleData[i].segmentData.GetRotatePosition();
		
		//四角形はスケール1だと1辺が1なので、半径1のときは1辺を2にしないといけないため、2倍
		capsuleModelObjects[0][i].SetScale(capsuleData[i].r * 2);
		capsuleModelObjects[0][i].SetPosition(lineSegmentPos.v1);
		capsuleModelObjects[0][i].SetAngle(capsuleData[i].segmentData.angle);

		capsuleModelObjects[1][i].SetScale(capsuleData[i].r * 2);
		capsuleModelObjects[1][i].SetPosition(lineSegmentPos.v2);
		capsuleModelObjects[1][i].SetAngle(capsuleData[i].segmentData.angle);

		//円柱
		capsuleModelObjects[2][i].SetScale
		(Vector3(capsuleData[i].r * 2, capsuleData[i].segmentData.GetPositionDistance(), capsuleData[i].r * 2));
		capsuleModelObjects[2][i].SetPosition(capsuleData[i].segmentData.GetCenterPosition());
		capsuleModelObjects[2][i].SetAngle(capsuleData[i].segmentData.angle);
	}

}

void MelLib::GameObject::DrawCollisionCheckModel()
{
	for(auto& box : boxModelObjects)
	{
		box.Draw();
	}

	for (auto& sphere : sphereModelObjects)
	{
		sphere.Draw();
	}

	for (int i = 0, size = capsuleModelObjects.size(); i < size; i++)
	{
		for (auto& capsule : capsuleModelObjects[i])
		{
			capsule.Draw();
		}
	}
}

#endif // _DEBUG
