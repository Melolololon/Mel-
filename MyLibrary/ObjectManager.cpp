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
	cursor = std::unique_ptr<MouseCursor>(new MouseCursor());

	addObjectSort = OBJECT_SORT_NONE;
	addObjectSortOrderType = false;
}

void ObjectManager::update() 
{
#pragma region update
	//�J�[�\���A�b�v�f�[�g
	cursor.get()->update();
	
	//�g��for���́A�r���ŃT�C�Y�ς���ƃ_��
	//�ϐ��p�ӂ���size����邩�A�ꎞ�I�ɕʂ̔z��ɓ���āAupdate�I����ɒǉ�
	std::vector<Object*>o = objects;
	for (auto& obj : o) 
	{
		obj->update();
	}

	nearPos = cursor.get()->getNearPos();
	farPos = cursor.get()->getFarPos();
#pragma endregion

#pragma region collision

	CollisionFlag f1;
	CollisionFlag f2;
	int collisionCount[2] = { 0,0 };

	std::vector<SphereData>sphere1;
	std::vector<SphereData>sphere2;
	
	std::vector<BoardData>boardData1;

	std::vector<LineSegmentData>segmentData1;

	//if�����Ɋ֐��Ă΂��ɂ��炩���ߎ擾�����ق�������
#pragma region ���Ƌ�

	if (checkCollision.sphere) 
	{
		for (auto& o1 : o)
		{
			for (auto& o2 : o)
			{
				f1 = o1->getCollisionFlag();
				f2 = o2->getCollisionFlag();

				//�����Ɣ�r�A�܂��͂ǂ��炩������m�F���Ȃ��Ă����ꍇ�A����
				if (o1 == o2 || !f1.sphere || !f2.sphere)continue;

				sphere1 = o1->getSphereData();
				sphere2 = o2->getSphereData();

				for (auto& c1 : sphere1)
				{
					for (auto& c2 : sphere2)
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

#pragma region �����Ɣ�
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
				for (auto& c1 : segmentData1)
				{
					for (auto& c2 : boardData1)
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

							c1.hitPos = hitPos;
							c2.hitPos = hitPos;

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

#pragma region �}�E�X�J�[�\���Ɣ|��
	if (checkMouseCollision && checkCollision.board)
	{
		for (auto& o1 : o)
		{
			if(o1->getCollisionFlag().board)
			{
				boardData1 = o1->getBoardData();
				for (auto& c1 : boardData1)
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
						c1.hitPos = hitPos;
					
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
	int size = objects.size();
	for (int i = 0; i < size; i++)
	{
		if (objects[i]->getIsDead())
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

#pragma region �I�u�W�F�N�g�֐�
void ObjectManager::reserveObjectArray(const int& reserveNum)
{
	objects.reserve(reserveNum);
}

void ObjectManager::addObject(Object* object)
{
	if (object)
	{
		object->initializeObject();
		objects.push_back(object);
	}

	if (addObjectSort != OBJECT_SORT_NONE)
		setObjectSort(addObjectSort, addObjectSortOrderType);
}

void ObjectManager::setAddObjectSortState(const ObjectManager::ObjectSort& sort, const bool& orderType)
{
	addObjectSort = sort;
	addObjectSortOrderType = orderType;
}

void ObjectManager::setObjectSort(const ObjectManager::ObjectSort& sort, const bool& orderType)
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
