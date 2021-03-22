#include "ObjectManager.h"
#include"CollisionType.h"
#include"LibMath.h"


ObjectManager::ObjectManager()
{
}


ObjectManager::~ObjectManager()
{
}

ObjectManager* ObjectManager::getInstance()
{
	static ObjectManager inst;
	return &inst;
}

void ObjectManager::initialize() 
{
	checkCollision.board = false;
	checkCollision.lineSegment = false;
	checkCollision.plane = false;
	checkCollision.ray = false;
	checkCollision.sphere = false;

	checkMouseCollision = false;
	cameraPosition = 0.0f;
	cursor = std::make_unique<MouseCursor>();
	cursor.get()->initialize();

	addObjectSort = OBJECT_SORT_NONE;
	addObjectSortOrderType = false;
}

void ObjectManager::update() 
{
#pragma region update
	//カーソルアップデート
	if (cursor) 
	{
		cursor.get()->update();
		nearPos = cursor.get()->getNearPos();
		farPos = cursor.get()->getFarPos();
	}
	//拡張for文は、途中でサイズ変えるとダメ
	//変数用意してsize入れるか、一時的に別の配列に入れて、update終了後に追加
	std::vector<Object*>o = objects;
	for (auto& obj : o)
	{
		obj->update();
	}

	
#pragma endregion

#pragma region collision

	CollisionFlag f1;
	CollisionFlag f2;
	int collisionCount[2] = { 0,0 };

	std::vector<SphereData>sphere1;
	std::vector<SphereData>sphere2;
	
	std::vector<BoardData>boardData1;

	std::vector<LineSegmentData>segmentData1;

	//if文毎に関数呼ばずにあらかじめ取得したほうがいい
#pragma region 球と球

	if (checkCollision.sphere) 
	{
		for (auto& o1 : o)
		{
			for (auto& o2 : o)
			{
				f1 = o1->getCollisionFlag();
				f2 = o2->getCollisionFlag();

				//自分と比較、またはどちらかが判定確認しなくていい場合、無視
				if (o1 == o2 || !f1.sphere || !f2.sphere)continue;

				sphere1 = o1->getSphereData();
				sphere2 = o2->getSphereData();

				for (const auto& c1 : sphere1)
				{
					for (const auto& c2 : sphere2)
					{
						if (LibMath::sphereCollision
						(
							c1.position,
							c1.r,
							c2.position,
							c2.r
						))
						{
							o1->hit(o2, CollisionType::COLLISION_SPHERE, collisionCount[0]);
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
		for (auto& o1 : o)
		{
			for (auto& o2 : o)
			{
				f1 = o1->getCollisionFlag();
				f2 = o2->getCollisionFlag();

				if (o1 == o2 || !f1.lineSegment || !f2.board)continue;

				segmentData1 = o1->getLineSegmentData();
				boardData1 = o2->getBoardData();
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
						if (LibMath::lineSegmentAndBoardCollision
						(
							c1.position[0],
							c1.position[1],
							c2.normal,
							c2.position,
							p,
							&hitPos

						))
						{

							o1->getLineSegmentHitPosition(collisionCount[0]) = hitPos;
							o2->getBoardHitPosition(collisionCount[1]) = hitPos;

							o1->hit(o2, CollisionType::COLLISION_LINESEGMENT, collisionCount[0]);
							o2->hit(o1, CollisionType::COLLISION_BOARD, collisionCount[1]);
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
		for (auto& o1 : o)
		{
			if(o1->getCollisionFlag().board)
			{
				boardData1 = o1->getBoardData();
				for (const auto& c1 : boardData1)
				{
					std::vector<Vector3>p(4);
					p[0] = c1.leftDownPos;
					p[1] = c1.leftUpPos;
					p[3] = c1.rightUpPos;
					p[2] = c1.rightDownPos;
					Vector3 hitPos = { 0,0,0 };
					if (LibMath::lineSegmentAndBoardCollision
					(
						nearPos,
						farPos,
						c1.normal,
						c1.position,
						p,
						&hitPos
						))
					{
						o1->getBoardHitPosition(collisionCount[0]) = hitPos;
					
						o1->hit(cursor.get(), CollisionType::COLLISION_BOARD, collisionCount[0]);
					}
					collisionCount[0]++;
				}
			}
		}
	}
#pragma endregion


#pragma endregion

}

void ObjectManager::draw() 
{
	for (auto& o : objects)
	{
		o->draw();
	}
}

void ObjectManager::isDeadCheck()
{
	size_t size = objects.size();
	for (size_t i = 0; i < size; i++)
	{
		if (objects[i]->getEraseManager())
		{
			delete objects[i];
			objects.erase(objects.begin() + i);
			i--;
			size--;
		}
	}
	objects.shrink_to_fit();

}

void ObjectManager::end() 
{
	allDeleteObject();
}

#pragma region オブジェクト関数
void ObjectManager::reserveObjectArray(const int& reserveNum)
{
	objects.reserve(reserveNum);
}

void ObjectManager::addObject(Object* object)
{

	if (object)
	{
		object->objectInitialize();
		object->initialize();
		objects.push_back(object);
	}

	if (addObjectSort != OBJECT_SORT_NONE)
		objectSort(addObjectSort, addObjectSortOrderType);
}

void ObjectManager::setAddObjectSortState(const ObjectManager::ObjectSort& sort, const bool& orderType)
{
	addObjectSort = sort;
	addObjectSortOrderType = orderType;
}

void ObjectManager::objectSort(const ObjectManager::ObjectSort& sort, const bool& orderType)
{
	switch (sort)
	{
	case OBJECT_SORT_XYZ_SUM:
		std::sort(objects.begin(), objects.end(), [&orderType]( Object* obj1,  Object* obj2)
		{
			Vector3 pos1 = obj1->getPosition();
			Vector3 pos2 = obj2->getPosition();
			float posSum1 = pos1.x + pos1.y + pos1.z;
			float posSum2 = pos2.x + pos2.y + pos2.z;
			
			if (orderType)return posSum1 < posSum2;
			return posSum1 > posSum2;
		});
		break;

	case OBJECT_SORT_X:
		std::sort(objects.begin(), objects.end(), [&orderType](Object* obj1, Object* obj2)
		{
			Vector3 pos1 = obj1->getPosition();
			Vector3 pos2 = obj2->getPosition();

			if (orderType)return pos1.x < pos2.x;
			return pos1.x > pos2.x;
		});
		break;

	case OBJECT_SORT_Y:
		std::sort(objects.begin(), objects.end(), [&orderType](Object* obj1, Object* obj2)
		{
			Vector3 pos1 = obj1->getPosition();
			Vector3 pos2 = obj2->getPosition();

			if (orderType)return pos1.y < pos2.y;
			return pos1.y > pos2.y;
		});
		break;

	case OBJECT_SORT_Z:
		std::sort(objects.begin(), objects.end(), [&orderType](Object* obj1, Object* obj2)
		{
			Vector3 pos1 = obj1->getPosition();
			Vector3 pos2 = obj2->getPosition();

			if (orderType)return pos1.z < pos2.z;
			return pos1.z > pos2.z;
		});
		break;

	case OBJECT_SORT_NEAR_DISTANCE:
		
		std::sort(objects.begin(), objects.end(), [&](Object* obj1, Object* obj2)
		{
			Vector3 pos1 = obj1->getPosition();
			Vector3 pos2 = obj2->getPosition();

			float dis1 = LibMath::calcDistance3D(pos1, nearPos);
			float dis2 = LibMath::calcDistance3D(pos2, nearPos);

			if (orderType)return dis1 < dis2;
			return dis1 > dis2;
		});
		break;

	case OBJECT_SORT_FAR_DISTANCE:

		std::sort(objects.begin(), objects.end(), [&](Object* obj1, Object* obj2)
		{
			Vector3 pos1 = obj1->getPosition();
			Vector3 pos2 = obj2->getPosition();

			float dis1 = LibMath::calcDistance3D(pos1, farPos);
			float dis2 = LibMath::calcDistance3D(pos2, farPos);

			if (orderType)return dis1 < dis2;
			return dis1 > dis2;
		});
		break;
	}
}

#pragma endregion

void ObjectManager::setCollisionFlag3D(const CollisionFlag& type)
{
	checkCollision = type;
}

void ObjectManager::setMouseCollisionFlag(const bool& flag)
{
	checkMouseCollision = flag;
}



void ObjectManager::allDeleteObject()
{
	for (auto& o : objects) 
	{
		delete o;
	}
	objects.clear();
	objects.shrink_to_fit();

	
}
