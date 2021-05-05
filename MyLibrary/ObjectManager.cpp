#include "ObjectManager.h"
#include"CollisionType.h"
#include"LibMath.h"


ObjectManager::ObjectManager()
{
}


ObjectManager::~ObjectManager()
{
}

ObjectManager* ObjectManager::GetInstance()
{
	static ObjectManager inst;
	return &inst;
}

void ObjectManager::Initialize() 
{
	checkCollision.board = false;
	checkCollision.box = false;
	checkCollision.lineSegment = false;
	checkCollision.plane = false;
	checkCollision.ray = false;
	checkCollision.sphere = false;

	checkMouseCollision = false;
	cameraPosition = 0.0f;
	cursor = std::make_unique<MouseCursor>();
	cursor->Initialize();

	addObjectSort = OBJECT_SORT_NONE;
	addObjectSortOrderType = false;

	addObjects.reserve(100);
}

void ObjectManager::Update() 
{
#pragma region Update
	//カーソルアップデート
	if (cursor) 
	{
		cursor->Update();
		nearPos = cursor->GetNearPos();
		farPos = cursor->GetFarPos();
	}
	
	for (auto& obj : objects)
		obj->Update();
	

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

#pragma region collision

	CollisionFlag f1;
	CollisionFlag f2;
	int collisionCount[2] = { 0,0 };

	std::vector<SphereData>sphere1;
	std::vector<SphereData>sphere2;

	std::vector<BoxData>box1;
	std::vector<BoxData>box2;
	
	std::vector<BoardData>boardData1;

	std::vector<LineSegmentData>segmentData1;

	//if文毎に関数呼ばずにあらかじめ取得したほうがいい
#pragma region 球と球

	if (checkCollision.sphere) 
	{
		for (auto& o1 : objects)
		{
			for (auto& o2 : objects)
			{
				f1 = o1->GetCollisionFlag();
				f2 = o2->GetCollisionFlag();

				//自分と比較、またはどちらかが判定確認しなくていい場合、無視
				if (o1 == o2 || !f1.sphere || !f2.sphere)continue;

				sphere1 = o1->GetSphereData();
				sphere2 = o2->GetSphereData();

				for (const auto& c1 : sphere1)
				{
					for (const auto& c2 : sphere2)
					{
						if (LibMath::SphereAndSphereCollision
						(
							c1.position,
							c1.r,
							c2.position,
							c2.r
						))
						{
							o1->Hit(o2.get(), CollisionType::COLLISION_SPHERE, collisionCount[0]);
						}
						collisionCount[1]++;
					}
					collisionCount[0]++;
					collisionCount[1] = 0;
				}
				collisionCount[0] = 0;

			}
		}
	}
#pragma endregion

#pragma region 球とAABB
	if (checkCollision.sphere && checkCollision.box)
	{
		for (auto& o1 : objects)
		{
			for (auto& o2 : objects)
			{
				f1 = o1->GetCollisionFlag();
				f2 = o2->GetCollisionFlag();

				//自分と比較、またはどちらかが判定確認しなくていい場合、無視
				if (o1 == o2 || !f1.sphere || !f2.box)continue;

				sphere1 = o1->GetSphereData();
				box1 = o2->GetBoxData();

				for (const auto& c1 : sphere1)
				{
					for (const auto& c2 : box1)
					{
						BoxHitDirection dis;
						if (LibMath::SphereAndBoxCollision
						(
							c1.position,
							c1.r,
							c2.position,
							c2.size,
							&dis
						))
						{
							o1->GetSphereBoxHitDistance(collisionCount[0]) = dis;
							o2->GetBoxBoxHitDistance(collisionCount[1]) = dis;

							o1->Hit(o2.get(), CollisionType::COLLISION_SPHERE, collisionCount[0]);
							o2->Hit(o1.get(), CollisionType::COLLISION_BOX, collisionCount[1]);
						}
						

						collisionCount[1]++;
					}
					collisionCount[0]++;
					collisionCount[1] = 0;
				}
				collisionCount[0] = 0;

			}
		}
	}
#pragma endregion

#pragma region AABBとAABB
	if(checkCollision.box)
	{
		for (auto& o1 : objects)
		{
			for (auto& o2 : objects)
			{
				f1 = o1->GetCollisionFlag();
				f2 = o2->GetCollisionFlag();

				//自分と比較、またはどちらかが判定確認しなくていい場合、無視
				if (o1 == o2 || !f1.box || !f2.box)continue;

				box1 = o1->GetBoxData();
				box2 = o2->GetBoxData();

				for (const auto& c1 : box1)
				{
					for (const auto& c2 : box2)
					{
						BoxHitDirection dis;
						if(LibMath::BoxAndBoxCollision
						(
							c1.position,
							c1.size,
							c2.position,
							c2.size,
							&dis,
							nullptr
						))
						{
							o1->GetBoxBoxHitDistance(collisionCount[0]) = dis;
							o1->Hit(o2.get(), CollisionType::COLLISION_BOX, collisionCount[0]);
						}


						collisionCount[1]++;
					}
					collisionCount[0]++;
					collisionCount[1] = 0;
				}
				collisionCount[0] = 0;

			}
		}
	}

#pragma endregion


#pragma region 線分と板
	if (checkCollision.lineSegment && checkCollision.board)
	{
		for (auto& o1 : objects)
		{
			for (auto& o2 : objects)
			{
				f1 = o1->GetCollisionFlag();
				f2 = o2->GetCollisionFlag();

				if (o1 == o2 || !f1.lineSegment || !f2.board)continue;

				segmentData1 = o1->GetLineSegmentData();
				boardData1 = o2->GetBoardData();
				for (const auto& c1 : segmentData1)
				{
					for (const auto& c2 : boardData1)
					{
						std::vector<Vector3>p(4);
						p[0] = c2.leftDownPos;
						p[1] = c2.leftUpPos;
						p[3] = c2.rightUpPos;
						p[2] = c2.rightDownPos;

						Vector3 hitPos = { 0,0,0 };
						if (LibMath::LineSegmentAndBoardCollision
						(
							c1.position[0],
							c1.position[1],
							c2.normal,
							c2.position,
							p,
							&hitPos

						))
						{

							o1->GetLineSegmentHitPosition(collisionCount[0]) = hitPos;
							o2->GetBoardHitPosition(collisionCount[1]) = hitPos;

							o1->Hit(o2.get(), CollisionType::COLLISION_LINESEGMENT, collisionCount[0]);
							o2->Hit(o1.get(), CollisionType::COLLISION_BOARD, collisionCount[1]);
						}

						collisionCount[1]++;
					}

					collisionCount[0]++;
					collisionCount[1] = 0;
				}

				collisionCount[0] = 0;
			}
		}
	}
#pragma endregion

#pragma region マウスカーソルと板ポリ
	if (checkMouseCollision && checkCollision.board)
	{
		for (auto& o1 : objects)
		{
			if(o1->GetCollisionFlag().board)
			{
				boardData1 = o1->GetBoardData();
				for (const auto& c1 : boardData1)
				{
					std::vector<Vector3>p(4);
					p[0] = c1.leftDownPos;
					p[1] = c1.leftUpPos;
					p[3] = c1.rightUpPos;
					p[2] = c1.rightDownPos;
					Vector3 hitPos = { 0,0,0 };
					if (LibMath::LineSegmentAndBoardCollision
					(
						nearPos,
						farPos,
						c1.normal,
						c1.position,
						p,
						&hitPos
						))
					{
						o1->GetBoardHitPosition(collisionCount[0]) = hitPos;
					
						o1->Hit(cursor.get(), CollisionType::COLLISION_BOARD, collisionCount[0]);
					}
					collisionCount[0]++;
				}
			}
		}
	}
#pragma endregion


#pragma endregion


	EraseObjectCheck();
}

void ObjectManager::Draw() 
{
	for (auto& o : objects)
	{
		o.get()->Draw();
	}
}

void ObjectManager::EraseObjectCheck()
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

void ObjectManager::Finitialize() 
{
	AllEraseObject();
}

#pragma region オブジェクト関数
void ObjectManager::ReserveObjectArray(const int& reserveNum)
{
	objects.reserve(reserveNum);
}

void ObjectManager::AddObject(const std::shared_ptr<Object>& object)
{

	if (object)
	{
		object.get()->ObjectInitialize();
		addObjects.push_back(object);
	}

	
}

void ObjectManager::SetAddObjectSortState(const ObjectSortType& sort, const bool& orderType)
{
	addObjectSort = sort;
	addObjectSortOrderType = orderType;
}

void ObjectManager::ObjectSort(const ObjectSortType& sort, const bool& orderType)
{
	switch (sort)
	{
	case OBJECT_SORT_XYZ_SUM:
		std::sort(objects.begin(), objects.end(), [&orderType](const std::shared_ptr<Object>& obj1, const std::shared_ptr<Object>& obj2)
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
			const std::shared_ptr<Object>& obj1, 
			const std::shared_ptr<Object>& obj2
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
			const std::shared_ptr<Object>& obj1,
			const std::shared_ptr<Object>& obj2
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
			const std::shared_ptr<Object>& obj1,
			const std::shared_ptr<Object>& obj2
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
			const std::shared_ptr<Object>& obj1,
			const std::shared_ptr<Object>& obj2
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
			const std::shared_ptr<Object>& obj1,
			const std::shared_ptr<Object>& obj2
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
	}
}

#pragma endregion

void ObjectManager::SetCollisionFlag3D(const CollisionFlag& type)
{
	checkCollision = type;
}

void ObjectManager::SetMouseCollisionFlag(const bool& flag)
{
	checkMouseCollision = flag;
}



void ObjectManager::AllEraseObject()
{
	objects.clear();
}
