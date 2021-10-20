#include "MeshCatObject.h"

#include"Input.h"
#include"GameObjectManager.h"

const MelLib::PlaneData* MeshCatObject::pPlaneData;

MeshCatObject::MeshCatObject(MelLib::Vector3 pos, MelLib::ModelData* pModel)
{
	//modelObjects.emplace("main", MelLib::ModelObject());
	modelObjects["main"];
	if(pModel)
	{
		modelObjects["main"].Create(pModel, nullptr);
	}
	else
	{
		modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), nullptr);
		modelObjects["main"].SetScale(2);
	}

	modelObjects["main"].SetPosition(pos);
}

void MeshCatObject::Update()
{
	MelLib::ModelData* pFront = nullptr;
	MelLib::ModelData* pBack = nullptr;

	if (MelLib::Input::KeyTrigger(DIK_SPACE) && !firstFream)
	{
		bool res = modelObjects["main"].MeshCat(*pPlaneData, pFront, pBack, false);
		if (res)
		{
			MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<MeshCatObject>(pPlaneData->GetNormal() * 0.2f, pFront));
			MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<MeshCatObject>(pPlaneData->GetNormal() * -0.2f, pBack));
			//eraseManager = true;
			isCat = true;
		}
	}

	firstFream = false;
	int z = 0;
}

void MeshCatObject::Draw()
{
	if(!isCat)modelObjects["main"].Draw();
}

void MeshCatObject::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
{
}
