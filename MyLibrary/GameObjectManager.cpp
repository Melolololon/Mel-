#include "GameObjectManager.h"
#include"CollisionType.h"
#include"LibMath.h"
#include"Collision.h"
#include"Physics.h"
#include"Values.h"


using namespace MelLib;

GameObjectManager::GameObjectManager()
{
}


GameObjectManager::~GameObjectManager()
{
}

GameObjectManager* GameObjectManager::GetInstance()
{
	static GameObjectManager inst;
	return &inst;
}

void GameObjectManager::Initialize() 
{
	

	cursor = std::make_unique<MouseCursor>();
	//cursor->Initialize();

	addObjectSort = OBJECT_SORT_NONE;
	addObjectSortOrderType = false;

	addObjects.reserve(100);
}

void GameObjectManager::Update() 
{
#pragma region オブジェクトのUpdate
	//カーソルアップデート
	if (cursor) 
	{
		cursor->Update();
		nearPos = cursor->GetNearPos();
		farPos = cursor->GetFarPos();
	}
	
	for (auto& obj : objects) 
	{
		obj->Update();
		
	}
	

	if(addObjects.size() != 0)
	{
		for (auto& a : addObjects) 
		{

			a.get()->Update();
			objects.push_back(a);
		}
		
		if (addObjectSort != OBJECT_SORT_NONE)
			ObjectSort(addObjectSort, addObjectSortOrderType);

		addObjects.clear();
	}

#pragma endregion

#pragma region 新判定処理
	

	size_t objectSize = objects.size();

	std::vector<CollisionDetectionFlag>collisionFlags(objectSize);
	for (int i = 0; i < objectSize; i++)
	{
		collisionFlags[i] = objects[i]->GetCollisionFlag();


#ifdef _DEBUG
		objects[i]->CreateCollisionCheckModel();
		objects[i]->SetCollisionCheckModelData();
#endif // _DEBUG
	}



	for (int objI = 0; objI < objectSize; objI++)
	{
		GameObject* obj1 = objects[objI].get();
		for (int objJ = 0; objJ < objectSize; objJ++)
		{
			GameObject* obj2 = objects[objJ].get();
			
			////自分と比較、比較済の組み合わせはcontinue
			if (objI >= objJ)continue;

#pragma region Sphere & Sphere


			if (collisionFlags[objI].sphere
				&& collisionFlags[objJ].sphere) 
			{

				std::vector<SphereData>sphereData1 = obj1->GetSphereData();
				size_t sphereData1Size = sphereData1.size();
				std::vector<SphereData>sphereData2 = obj2->GetSphereData();
				size_t sphereData2Size = sphereData2.size();


				for (int colI = 0; colI < sphereData1Size; colI++)
				{
					for (int colJ = 0; colJ < sphereData2Size; colJ++)
					{
						if (Collision::SphereAndSphere(sphereData1[colI], sphereData2[colJ]))
						{
							//hitを呼び出す
							obj1->Hit
							(
								obj2,
								ShapeType3D::SPHERE,
								colI,
								ShapeType3D::SPHERE,
								colJ
							);
							obj2->Hit
							(
								obj1,
								ShapeType3D::SPHERE,
								colJ,
								ShapeType3D::SPHERE,
								colI
							);
						}
					}
				}

			}
#pragma endregion

#pragma region Box & Box
			if (collisionFlags[objI].box
				&& collisionFlags[objJ].box)
			{

				std::vector<BoxData>boxData1 = obj1->GetBoxData();
				size_t boxData1Size = boxData1.size();
				std::vector<BoxData>boxData2 = obj2->GetBoxData();
				size_t boxData2Size = boxData2.size();


				for (int colI = 0; colI < boxData1Size; colI++)
				{
					for (int colJ = 0; colJ < boxData2Size; colJ++)
					{
						BoxCalcResult result1;
						BoxCalcResult result2;

						if (Collision::BoxAndBox(boxData1[colI], &result1, boxData2[colJ], &result2))
						{
							obj1->SetBoxCalcResult(result1, colI);
							obj2->SetBoxCalcResult(result2, colJ);

							//hitを呼び出す
							obj1->Hit
							(
								obj2,
								ShapeType3D::BOX,
								colI,
								ShapeType3D::BOX,
								colJ
							);
							obj2->Hit
							(
								obj1,
								ShapeType3D::BOX,
								colJ,
								ShapeType3D::BOX,
								colI
							);
						}
					}
				}


			}
#pragma endregion

#pragma region Segment & Segment
			if (collisionFlags[objI].segment
				&& collisionFlags[objJ].segment)
			{

				std::vector<Segment3DData>segmentData1 = obj1->GetSegmentData();
				size_t segmentData1Size = segmentData1.size();
				std::vector<Segment3DData>segmentData2 = obj2->GetSegmentData();
				size_t segmentData2Size = segmentData2.size();



				for (int colI = 0; colI < segmentData1Size; colI++)
				{
					for (int colJ = 0; colJ < segmentData2Size; colJ++)
					{
						Segment3DCalcResult result1;
						Segment3DCalcResult result2;

						if (Collision::Segment3DAndSegment3D
						(
							segmentData1[colI],
							&result1,
							segmentData2[colJ],
							&result2
						))
						{
							obj1->SetSegmentCalcResult(result1,colI);
							obj2->SetSegmentCalcResult(result2,colJ);

							//hitを呼び出す
							obj1->Hit
							(
								obj2,
								ShapeType3D::SEGMENT,
								colI,
								ShapeType3D::SEGMENT,
								colJ
							);
							obj2->Hit
							(
								obj1,
								ShapeType3D::SEGMENT,
								colJ,
								ShapeType3D::SEGMENT,
								colI
							);
						}
					}
				}

			}
#pragma endregion

#pragma region Capsule & Capsule
			if (collisionFlags[objI].capsule
				&& collisionFlags[objJ].capsule)
			{

				std::vector<CapsuleData>capsuleData1 = obj1->GetCapsuleData();
				size_t capsuleData1Size = capsuleData1.size();
				std::vector<CapsuleData>capsuleData2 = obj2->GetCapsuleData();
				size_t capsuleData2Size = capsuleData2.size();


				for (int colI = 0; colI < capsuleData1Size; colI++)
				{
					for (int colJ = 0; colJ < capsuleData2Size; colJ++)
					{
						if (Collision::CapsuleAndCapsule(capsuleData1[colI], capsuleData2[colJ]))
						{
							//hitを呼び出す
							obj1->Hit
							(
								obj2,
								ShapeType3D::CAPSULE,
								colI,
								ShapeType3D::CAPSULE,
								colJ
							);
							obj2->Hit
							(
								obj1,
								ShapeType3D::CAPSULE,
								colJ,
								ShapeType3D::CAPSULE,
								colI
							);
						}
					}
				}

			}
#pragma endregion

#pragma region Sphere & Box
			if (collisionFlags[objI].sphere
				&& collisionFlags[objJ].box)
			{

				std::vector<SphereData>sphereData = obj1->GetSphereData();
				size_t sphereDataSize = sphereData.size();
				std::vector<BoxData>boxData = obj2->GetBoxData();
				size_t boxDataSize = boxData.size();

				for (int colI = 0; colI < sphereDataSize; colI++)
				{
					for (int colJ = 0; colJ < boxDataSize; colJ++)
					{
						SphereCalcResult result1;
						BoxCalcResult result2;

						if (Collision::SphereAndBox
						(
							sphereData[colI],
							&result1,
							boxData[colJ],
							&result2
						))
						{
							obj1->SetSphereCalcResult(result1, colI);
							obj2->SetBoxCalcResult(result2, colJ);
							//hitを呼び出す
							obj1->Hit
							(
								obj2,
								ShapeType3D::SPHERE,
								colI,
								ShapeType3D::BOX,
								colJ
							);
							obj2->Hit
							(
								obj1,
								ShapeType3D::BOX,
								colJ,
								ShapeType3D::SPHERE,
								colI
							);
						}
					}
				}




			}

			if(collisionFlags[objJ].sphere
				&& collisionFlags[objI].box)
			{

				std::vector<SphereData>sphereData = obj2->GetSphereData();
				size_t sphereDataSize = sphereData.size();
				std::vector<BoxData>boxData = obj1->GetBoxData();
				size_t boxDataSize = boxData.size();

				for (int colI = 0; colI < sphereDataSize; colI++)
				{
					for (int colJ = 0; colJ < boxDataSize; colJ++)
					{
						SphereCalcResult result1;
						BoxCalcResult result2;

						if (Collision::SphereAndBox
						(
							sphereData[colI],
							&result1,
							boxData[colJ],
							&result2
						))
						{
							obj2->SetSphereCalcResult(result1, colI);
							obj1->SetBoxCalcResult(result2, colJ);
							//hitを呼び出す
							obj2->Hit
							(
								obj1,
								ShapeType3D::SPHERE,
								colI,
								ShapeType3D::BOX,
								colJ
							);
							obj1->Hit
							(
								obj2,
								ShapeType3D::BOX,
								colJ,
								ShapeType3D::SPHERE,
								colI
							);
						}
					}
				}

			}

#pragma endregion

#pragma region Sphere & Capsule
			if (collisionFlags[objI].sphere
				&& collisionFlags[objJ].capsule)
			{
				std::vector<SphereData>sphereData = obj1->GetSphereData();
				size_t sphereDataSize = sphereData.size();
				std::vector<CapsuleData>capsuleData = obj2->GetCapsuleData();
				size_t capsuleDataSize = capsuleData.size();
			
				for (int colI = 0; colI < sphereDataSize; colI++)
				{
					for (int colJ = 0; colJ < capsuleDataSize; colJ++)
					{
						if (Collision::SphereAndCapsule(sphereData[colI], capsuleData[colJ]))
						{
							//hitを呼び出す
							obj1->Hit
							(
								obj2,
								ShapeType3D::SPHERE,
								colI,
								ShapeType3D::CAPSULE,
								colJ
							);
							obj2->Hit
							(
								obj1,
								ShapeType3D::CAPSULE,
								colJ,
								ShapeType3D::SPHERE,
								colI
							);

						}
					}
				}

			}

			if (collisionFlags[objJ].sphere
				&& collisionFlags[objI].capsule)
			{

				std::vector<SphereData>sphereData = obj2->GetSphereData();
				size_t sphereDataSize = sphereData.size();
				std::vector<CapsuleData>capsuleData = obj1->GetCapsuleData();
				size_t capsuleDataSize = capsuleData.size();

				for (int colI = 0; colI < sphereDataSize; colI++)
				{
					for (int colJ = 0; colJ < capsuleDataSize; colJ++)
					{
						if (Collision::SphereAndCapsule(sphereData[colI], capsuleData[colJ]))
						{
							//hitを呼び出す
							obj2->Hit
							(
								obj1,
								ShapeType3D::SPHERE,
								colI,
								ShapeType3D::CAPSULE,
								colJ
							);
							obj1->Hit
							(
								obj2,
								ShapeType3D::CAPSULE,
								colJ,
								ShapeType3D::SPHERE,
								colI
							);

						}
					}
				}
			}

#pragma endregion

#pragma region Board & Segent
			if (collisionFlags[objI].board
				&& collisionFlags[objJ].segment)
			{
				std::vector<BoardData>boardData = obj1->GetBoardData();
				size_t boardDataSize = boardData.size();
				std::vector<Segment3DData>segmentData = obj2->GetSegmentData();
				size_t segmentDataSize = segmentData.size();

				for (int colI = 0; colI < boardDataSize; colI++)
				{
					for (int colJ = 0; colJ < segmentDataSize; colJ++)
					{
						BoardCalcResult result1;
						Segment3DCalcResult result2;

						if (Collision::BoardAndSegment3D(boardData[colI], &result1, segmentData[colJ], &result2))
						{
							obj1->SetBoardCalcResult(result1, colI);
							obj2->SetSegmentCalcResult(result2, colJ);

							//hitを呼び出す
							obj1->Hit
							(
								obj2,
								ShapeType3D::BOARD,
								colI,
								ShapeType3D::SEGMENT,
								colJ
							);
							obj2->Hit
							(
								obj1,
								ShapeType3D::SEGMENT,
								colJ,
								ShapeType3D::BOARD,
								colI
							);

						}
					}
				}

			}

			if (collisionFlags[objJ].board
				&& collisionFlags[objI].segment)
			{
				std::vector<BoardData>boardData = obj2->GetBoardData();
				size_t boardDataSize = boardData.size();
				std::vector<Segment3DData>segmentData = obj1->GetSegmentData();
				size_t segmentDataSize = segmentData.size();

				for (int colI = 0; colI < boardDataSize; colI++)
				{
					for (int colJ = 0; colJ < segmentDataSize; colJ++)
					{
						BoardCalcResult result1;
						Segment3DCalcResult result2;

						if (Collision::BoardAndSegment3D(boardData[colI], &result1, segmentData[colJ], &result2))
						{
							obj2->SetBoardCalcResult(result1, colI);
							obj1->SetSegmentCalcResult(result2, colJ);

							//hitを呼び出す
							obj2->Hit
							(
								obj1,
								ShapeType3D::BOARD,
								colI,
								ShapeType3D::SEGMENT,
								colJ
							);
							obj1->Hit
							(
								obj2,
								ShapeType3D::SEGMENT,
								colJ,
								ShapeType3D::BOARD,
								colI
							);

						}
					}
				}

			}

#pragma endregion

#pragma region Board & Capsule
			if (collisionFlags[objI].board
				&& collisionFlags[objJ].capsule)
			{
				std::vector<BoardData>boardData = obj1->GetBoardData();
				size_t boardDataSize = boardData.size();
				std::vector<CapsuleData>capsuleData = obj2->GetCapsuleData();
				size_t capsuleDataSize = capsuleData.size();

				for (int colI = 0; colI < boardDataSize; colI++)
				{
					for (int colJ = 0; colJ < capsuleDataSize; colJ++)
					{
						BoardCalcResult result1;
						Segment3DCalcResult result2;

						if (Collision::BoardAndSegment3D(boardData[colI], &result1, capsuleData[colJ].GetSegment3DData(), &result2))
						{
							obj1->SetBoardCalcResult(result1, colI);
							obj2->SetCapsuleCalcResult(result2, colJ);

							//hitを呼び出す
							obj1->Hit
							(
								obj2,
								ShapeType3D::BOARD,
								colI,
								ShapeType3D::CAPSULE,
								colJ
							);
							obj2->Hit
							(
								obj1,
								ShapeType3D::CAPSULE,
								colJ,
								ShapeType3D::BOARD,
								colI
							);

						}
					}
				}

			}

			if (collisionFlags[objJ].board
				&& collisionFlags[objI].capsule)
			{
				std::vector<BoardData>boardData = obj2->GetBoardData();
				size_t boardDataSize = boardData.size();
				std::vector<CapsuleData>capsuleData = obj1->GetCapsuleData();
				size_t capsuleDataSize = capsuleData.size();

				for (int colI = 0; colI < boardDataSize; colI++)
				{
					for (int colJ = 0; colJ < capsuleDataSize; colJ++)
					{
						BoardCalcResult result1;
						Segment3DCalcResult result2;

						if (Collision::BoardAndSegment3D(boardData[colI], &result1, capsuleData[colJ].GetSegment3DData(), &result2))
						{
							obj2->SetBoardCalcResult(result1, colI);
							obj1->SetCapsuleCalcResult(result2, colJ);

							//hitを呼び出す
							obj2->Hit
							(
								obj1,
								ShapeType3D::BOARD,
								colI,
								ShapeType3D::CAPSULE,
								colJ
							);
							obj1->Hit
							(
								obj2,
								ShapeType3D::CAPSULE,
								colJ,
								ShapeType3D::BOARD,
								colI
							);

						}
					}
				}

			}
#pragma endregion


		}
	}





#pragma endregion

	EraseObjectCheck();

}

void GameObjectManager::Draw() 
{
	for (auto& o : objects)
	{
		o->Draw();
		
#ifdef _DEBUG

		o->DrawCollisionCheckModel();
#endif // _DEBUG
	}
}

void GameObjectManager::EraseObjectCheck()
{

	size_t size = objects.size();
	for (size_t i = 0; i < size; i++)
	{
		if (objects[i]->GetEraseManager())
		{
			objects.erase(objects.begin() + i);
			i--;
			size--;
		}
	}


	objects.shrink_to_fit();

}

void GameObjectManager::Finalize() 
{
	AllEraseObject();
}

#pragma region オブジェクト関数
void GameObjectManager::ReserveObjectArray(const int& reserveNum)
{
	objects.reserve(reserveNum);
}

void GameObjectManager::AddObject(const std::shared_ptr<GameObject>& object)
{

	if (object)
	{
		object.get()->ObjectInitialize();
		addObjects.push_back(object);
	}

	
}

void GameObjectManager::SetAddObjectSortState(const ObjectSortType& sort, const bool& orderType)
{
	addObjectSort = sort;
	addObjectSortOrderType = orderType;
}

void GameObjectManager::ObjectSort(const ObjectSortType& sort, const bool& orderType)
{
	switch (sort)
	{
	case OBJECT_SORT_XYZ_SUM:
		std::sort(objects.begin(), objects.end(), [&orderType](const std::shared_ptr<GameObject>& obj1, const std::shared_ptr<GameObject>& obj2)
		{
			Vector3 pos1 = obj1->GetPosition();
			Vector3 pos2 = obj2->GetPosition();
			float posSum1 = pos1.x + pos1.y + pos1.z;
			float posSum2 = pos2.x + pos2.y + pos2.z;
			
			if (orderType)return posSum1 < posSum2;
			return posSum1 > posSum2;
		});
		break;

	case OBJECT_SORT_X:
		std::sort
		(
			objects.begin(),
			objects.end(), 
			[&orderType]
		(
			const std::shared_ptr<GameObject>& obj1, 
			const std::shared_ptr<GameObject>& obj2
		)
		{
			Vector3 pos1 = obj1->GetPosition();
			Vector3 pos2 = obj2->GetPosition();

			if (orderType)return pos1.x < pos2.x;
			return pos1.x > pos2.x;
		});
		break;

	case OBJECT_SORT_Y:
		std::sort
		(
			objects.begin(),
			objects.end(),
			[&orderType]
		(
			const std::shared_ptr<GameObject>& obj1,
			const std::shared_ptr<GameObject>& obj2
		)
		{
			Vector3 pos1 = obj1->GetPosition();
			Vector3 pos2 = obj2->GetPosition();

			if (orderType)return pos1.y < pos2.y;
			return pos1.y > pos2.y;
		});
		break;

	case OBJECT_SORT_Z:
		std::sort
		(
			objects.begin(),
			objects.end(),
			[&orderType]
		(
			const std::shared_ptr<GameObject>& obj1,
			const std::shared_ptr<GameObject>& obj2
		)
		{
			Vector3 pos1 = obj1->GetPosition();
			Vector3 pos2 = obj2->GetPosition();

			if (orderType)return pos1.z < pos2.z;
			return pos1.z > pos2.z;
		});
		break;

	case OBJECT_SORT_NEAR_DISTANCE:
		
		std::sort
		(
			objects.begin(),
			objects.end(),
			[&]
		(
			const std::shared_ptr<GameObject>& obj1,
			const std::shared_ptr<GameObject>& obj2
		)
		{
			Vector3 pos1 = obj1->GetPosition();
			Vector3 pos2 = obj2->GetPosition();

			float dis1 = LibMath::CalcDistance3D(pos1, nearPos);
			float dis2 = LibMath::CalcDistance3D(pos2, nearPos);

			if (orderType)return dis1 < dis2;
			return dis1 > dis2;
		});
		break;

	case OBJECT_SORT_FAR_DISTANCE:

		std::sort
		(
			objects.begin(),
			objects.end(),
			[&]
		(
			const std::shared_ptr<GameObject>& obj1,
			const std::shared_ptr<GameObject>& obj2
		)
		{
			Vector3 pos1 = obj1->GetPosition();
			Vector3 pos2 = obj2->GetPosition();

			float dis1 = LibMath::CalcDistance3D(pos1, farPos);
			float dis2 = LibMath::CalcDistance3D(pos2, farPos);

			if (orderType)return dis1 < dis2;
			return dis1 > dis2;
		});
		break;

	case OBJECT_SORT_SORT_NUMBER:
		std::sort
		(
			objects.begin(),
			objects.end(),
			[&]
		(
			const std::shared_ptr<GameObject>& obj1,
			const std::shared_ptr<GameObject>& obj2
			)
		{
			short obj1Num = obj1->GetSortNumber();
			short obj2Num = obj2->GetSortNumber();
			if (orderType)return obj1Num < obj2Num;
			return obj1Num > obj2Num;
		});
		break;
	}
}

#pragma endregion

//void GameObjectManager::SetCollisionFlag3D(const CollisionFlag& type)
//{
//	checkCollision = type;
//}

void GameObjectManager::SetMouseCollisionFlag(const bool& flag)
{
	checkMouseCollision = flag;
}



void GameObjectManager::AllEraseObject()
{
	objects.clear();
}
