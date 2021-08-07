#include "GameObject.h"

#include"Physics.h"

using namespace MelLib;


#ifdef _DEBUG


//判定確認用モデルのパイプライン
PipelineState GameObject::collisionCheckModelPipelineState;
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

#ifdef _DEBUG

void MelLib::GameObject::CreateCollisionCheckModelPipelineState()
{
	PipelineStateData pData = PipelineState::GetDefaultPipelineData(PipelineStateType::MODEL);
	pData.drawMode = DrawMode::WIREFRAME;

	collisionCheckModelPipelineState.CreatePipeline
	(
		pData,
		{L"LIB","",""},
		{L"LIB","",""},
		{L"LIB","",""},
		{L"LIB","",""},
		{L"LIB","",""},
		PipelineStateType::MODEL,
		nullptr,
		typeid(ModelObject).name(),
		1
	);
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
				modelObjcts[i].SetPipeline(&collisionCheckModelPipelineState);
			}
		}
		else if (dataNum < objNum)
		{
			modelObjcts.resize(objNum);
		}

	};

	//Box
	createOrDeleteModel(boxData.size(), boxModelObject, ShapeType3D::BOX);
	
}

void MelLib::GameObject::SetCollisionCheckModelData()
{
	//Box
	size_t dataNum = boxData.size();
	for(size_t i = 0; i < dataNum;i++)
	{
		boxModelObject[i].SetScale(boxData[i].size);
		boxModelObject[i].SetPosition(boxData[i].position);
	}
}

void MelLib::GameObject::DrawCollisionCheckModel()
{
	for(auto& box : boxModelObject)
	{
		box.Draw();
	}
}

#endif // _DEBUG
