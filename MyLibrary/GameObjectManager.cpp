#include "GameObjectManager.h"
#include"CollisionType.h"
#include"LibMath.h"
#include"Collision.h"
#include"Physics.h"
#include"Values.h"

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
#pragma region �I�u�W�F�N�g��Update
	//�J�[�\���A�b�v�f�[�g
	if (cursor) 
	{
		cursor->Update();
		nearPos = cursor->GetNearPos();
		farPos = cursor->GetFarPos();
	}
	
	for (auto& obj : objects) 
	{
		if (obj->GetCalcPhysicsFlag())obj->CalcMovePhysics();
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

	EraseObjectCheck();


#pragma region �V���菈��

	size_t objectSize = objects.size();

	std::vector<CollisionFlag>collisionFlags(objectSize);
	for (int i = 0; i < objectSize; i++)
	{
		collisionFlags[i] = objects[i]->GetCollisionFlag();
	}



	for (int objI = 0; objI < objectSize; objI++)
	{
		GameObject* obj1 = objects[objI].get();
		for (int objJ = 0; objJ < objectSize; objJ++)
		{
			GameObject* obj2 = objects[objJ].get();

#pragma region ��&��

			if (checkCollision.sphere)
			{
				//�����Ɣ�r�A��r�ς̑g�ݍ��킹�A�܂��͂ǂ��炩������m�F���Ȃ��Ă����ꍇ�Acontinue
				if (!collisionFlags[objI].sphere
					|| !collisionFlags[objJ].sphere
					|| objI >= objJ)continue;

				std::vector<SphereData>sphereData1 = obj1->GetSphereData();
				size_t sphereData1Size = sphereData1.size();
				std::vector<SphereData>sphereData2 = obj2->GetSphereData();
				size_t sphereData2Size = sphereData2.size();


				for (int colI = 0; colI < sphereData1Size; colI++)
				{
					for (int colJ = 0; colJ < sphereData2Size; colJ++)
					{
						if (Collision::SphereAndSphere(sphereData1[colI], sphereData2[objJ]))
						{
							//hit���Ăяo��
							obj1->Hit
							(
								obj2,
								CollisionType::COLLISION_SPHERE,
								objI,
								CollisionType::COLLISION_SPHERE,
								objJ
							);
							obj2->Hit
							(
								obj1,
								CollisionType::COLLISION_SPHERE,
								objJ,
								CollisionType::COLLISION_SPHERE,
								objI
							);
						}
					}
				}
			}

#pragma endregion

		}
	}





#pragma endregion


#pragma region �����菈��

//	CollisionFlag f1;
//	CollisionFlag f2;
//
//	//�I�u�W�F�N�g�̃��[�v��(��,�O)
//	int objectCount[2] = { 0,0 };
//	//�˓@�̃��[�v��
//	int collisionCount[2] = { 0,0 };
//	int skipCount = 0;
//
//	std::vector<SphereData>sphere1;
//	std::vector<SphereData>sphere2;
//
//	std::vector<BoxData>box1;
//	std::vector<BoxData>box2;
//	
//	std::vector<BoardData>boardData1;
//
//	std::vector<LineSegment3DData>segmentData1;
//
//	//if�����Ɋ֐��Ă΂��ɂ��炩���ߎ擾�����ق�������(����t���O�̂���?)
//#pragma region ���Ƌ�
//	if (checkCollision.sphere) 
//	{
//		for (auto& o1 : objects)
//		{
//			skipCount = 0;
//			for (auto& o2 : objects)
//			{
//				f1 = o1->GetCollisionFlag();
//				f2 = o2->GetCollisionFlag();
//
//				
//				//������r�̓J�E���g���Ȃ�
//				skipCount++;
//				if (objectCount[0] > skipCount - 1)continue;
//
//				//�����Ɣ�r�A�܂��͂ǂ��炩������m�F���Ȃ��Ă����ꍇ�A����
//				if (o1 == o2 || !f1.sphere || !f2.sphere)continue;
//
//				sphere1 = o1->GetSphereData();
// 				sphere2 = o2->GetSphereData();
//
//				for (const auto& c1 : sphere1)
//				{
//					for (const auto& c2 : sphere2)
//					{
//
//						if (Collision::SphereAndSphere(c1, c2))
//						{
//							//����
//  							if (o1->GetCalcPhysicsFlag() && o2->GetCalcPhysicsFlag()) 
//							{
//								Value2<Vector3> velocity = Physics::CalcRepulsionVelocity
//								(
//									Value2<Vector3>(o1->GetPosition(), o2->GetPosition()),
//									Value2<Vector3>(o1->GetVelocity(), o2->GetVelocity()),
//									Value2<float>(o1->GetMass(), o2->GetMass()),
//									Value2<Vector3>(1.0f, 1.0f)
//								);
//
//								if (o1->GetCalcPhysicsFlag()) 
//								{
//									o1->SetPosition(o1->GetPosition() + velocity.v1);
//									o1->SetVelocity(velocity.v1);
//								}
//								if (o2->GetCalcPhysicsFlag())
//								{
//									o2->SetPosition(o2->GetPosition() + velocity.v2);
//									o2->SetVelocity(velocity.v2);
//								}
//							}
//
//							//hit���Ăяo��
//							o1->Hit
//							(
//								o2.get(),
//								CollisionType::COLLISION_SPHERE, 
//								collisionCount[0],
//								CollisionType::COLLISION_SPHERE,
//								collisionCount[1]
//							);	
//							o2->Hit
//							(
//								o1.get(),
//								CollisionType::COLLISION_SPHERE,
//								collisionCount[1],
//								CollisionType::COLLISION_SPHERE, 
//								collisionCount[0]
//							);
//
//							//�����2��A�������Z��1��Ȃ̂ŁA�񐔂̃Y���ɂ��A�o�O��B
//							//1��ɓ��ꂷ��B
//							//�������Z2��ɂ���ƁA�ՓˑO�x�N�g���擾�ł��Ȃ��Ė���
//							//��x�m�F(o1�Ɋ��铖�Ă��Ċm�F)������A�����ΏۊO�ɂ���?A*��close�݂�����
//
//						}
//						collisionCount[1]++;
//					}
//					collisionCount[0]++;
//					collisionCount[1] = 0;
//				}
//				collisionCount[0] = 0;
//
//				objectCount[1]++;
//			}
//			objectCount[0]++;
//			objectCount[1] = 0;
//		}
//		objectCount[0] = 0;
//	}
//#pragma endregion
//
//#pragma region ����AABB
//	if (checkCollision.sphere && checkCollision.box)
//	{
//		for (auto& o1 : objects)
//		{
//			for (auto& o2 : objects)
//			{
//				f1 = o1->GetCollisionFlag();
//				f2 = o2->GetCollisionFlag();
//
//				//������r�̓J�E���g���Ȃ�
//				//��������̎�ނ���Ȃ���΂��ꂢ��Ȃ�
//				/*skipCount++;
//				if (objectCount[0] > skipCount - 1)continue;*/
//
//				//�����Ɣ�r�A�܂��͂ǂ��炩������m�F���Ȃ��Ă����ꍇ�A����
//				if (o1 == o2 || !f1.sphere || !f2.box)continue;
//
//				sphere1 = o1->GetSphereData();
//				box1 = o2->GetBoxData();
//
//				for (const auto& c1 : sphere1)
//				{
//					for (const auto& c2 : box1)
//					{
//						BoxHitDirection dis;
//						if (LibMath::SphereAndBoxCollision
//						(
//							c1.position,
//							c1.r,
//							c2.position,
//							c2.size,
//							&dis
//						))
//						{
//							o1->GetSphereBoxHitDistance(collisionCount[0]) = dis;
//							o2->GetBoxBoxHitDistance(collisionCount[1]) = dis;
//
//							o1->Hit(o2.get(), CollisionType::COLLISION_SPHERE, collisionCount[0], CollisionType::COLLISION_BOX, collisionCount[1]);
//							o2->Hit(o1.get(), CollisionType::COLLISION_BOX, collisionCount[1], CollisionType::COLLISION_SPHERE, collisionCount[0]);
//						}
//						
//
//						collisionCount[1]++;
//					}
//					collisionCount[0]++;
//					collisionCount[1] = 0;
//				}
//				collisionCount[0] = 0;
//				objectCount[1]++;
//			}
//			objectCount[0]++;
//			objectCount[1] = 0;
//		}
//		objectCount[0] = 0;
//	}
//#pragma endregion
//
//#pragma region AABB��AABB
//	if(checkCollision.box)
//	{
//		for (auto& o1 : objects)
//		{
//			for (auto& o2 : objects)
//			{
//				f1 = o1->GetCollisionFlag();
//				f2 = o2->GetCollisionFlag();
//
//				//�����Ɣ�r�A�܂��͂ǂ��炩������m�F���Ȃ��Ă����ꍇ�A����
//				if (o1 == o2 || !f1.box || !f2.box)continue;
//
//				box1 = o1->GetBoxData();
//				box2 = o2->GetBoxData();
//
//				for (const auto& c1 : box1)
//				{
//					for (const auto& c2 : box2)
//					{
//						BoxHitDirection dis;
//						if(LibMath::BoxAndBoxCollision
//						(
//							c1.position,
//							c1.size,
//							c2.position,
//							c2.size,
//							&dis,
//							nullptr
//						))
//						{
//							o1->GetBoxBoxHitDistance(collisionCount[0]) = dis;
//							o1->Hit(o2.get(), CollisionType::COLLISION_BOX, collisionCount[0], CollisionType::COLLISION_BOX, collisionCount[1]);
//						}
//
//
//						collisionCount[1]++;
//					}
//					collisionCount[0]++;
//					collisionCount[1] = 0;
//				}
//				collisionCount[0] = 0;
//
//			}
//		}
//	}
//
//#pragma endregion
//
//
//#pragma region �����Ɣ�
//	if (checkCollision.lineSegment && checkCollision.board)
//	{
//		for (auto& o1 : objects)
//		{
//			for (auto& o2 : objects)
//			{
//				f1 = o1->GetCollisionFlag();
//				f2 = o2->GetCollisionFlag();
//
//				if (o1 == o2 || !f1.lineSegment || !f2.board)continue;
//
//				segmentData1 = o1->GetLineSegmentData();
//				boardData1 = o2->GetBoardData();
//				for (const auto& c1 : segmentData1)
//				{
//					for (const auto& c2 : boardData1)
//					{
//						std::vector<Vector3>p(4);
//						p[0] = c2.leftDownPos;
//						p[1] = c2.leftUpPos;
//						p[3] = c2.rightUpPos;
//						p[2] = c2.rightDownPos;
//
//						Vector3 hitPos = { 0,0,0 };
//						if (LibMath::LineSegmentAndBoardCollision
//						(
//							c1.position[0],
//							c1.position[1],
//							c2.normal,
//							c2.position,
//							p,
//							&hitPos
//
//						))
//						{
//
//							o1->GetLineSegmentHitPosition(collisionCount[0]) = hitPos;
//							o2->GetBoardHitPosition(collisionCount[1]) = hitPos;
//
//							o1->Hit(o2.get(), CollisionType::COLLISION_LINESEGMENT, collisionCount[0], CollisionType::COLLISION_BOARD, collisionCount[1]);
//							o2->Hit(o1.get(), CollisionType::COLLISION_BOARD, collisionCount[1], CollisionType::COLLISION_LINESEGMENT, collisionCount[0]);
//						}
//
//						collisionCount[1]++;
//					}
//
//					collisionCount[0]++;
//					collisionCount[1] = 0;
//				}
//
//				collisionCount[0] = 0;
//			}
//		}
//	}
//#pragma endregion
//
//#pragma region �}�E�X�J�[�\���Ɣ|��
//	/*if (checkMouseCollision && checkCollision.board)
//	{
//		for (auto& o1 : objects)
//		{
//			if(o1->GetCollisionFlag().board)
//			{
//				boardData1 = o1->GetBoardData();
//				for (const auto& c1 : boardData1)
//				{
//					std::vector<Vector3>p(4);
//					p[0] = c1.leftDownPos;
//					p[1] = c1.leftUpPos;
//					p[3] = c1.rightUpPos;
//					p[2] = c1.rightDownPos;
//					Vector3 hitPos = { 0,0,0 };
//					if (LibMath::LineSegmentAndBoardCollision
//					(
//						nearPos,
//						farPos,
//						c1.normal,
//						c1.position,
//						p,
//						&hitPos
//						))
//					{
//						o1->GetBoardHitPosition(collisionCount[0]) = hitPos;
//					
//						o1->Hit(cursor.get(), CollisionType::COLLISION_BOARD, collisionCount[0]);
//					}
//					collisionCount[0]++;
//				}
//			}
//		}
//	}*/
//#pragma endregion


#pragma endregion

}

void GameObjectManager::Draw() 
{
	for (auto& o : objects)
	{
		o.get()->Draw();
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

void GameObjectManager::Finitialize() 
{
	AllEraseObject();
}

#pragma region �I�u�W�F�N�g�֐�
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

void GameObjectManager::SetCollisionFlag3D(const CollisionFlag& type)
{
	checkCollision = type;
}

void GameObjectManager::SetMouseCollisionFlag(const bool& flag)
{
	checkMouseCollision = flag;
}



void GameObjectManager::AllEraseObject()
{
	objects.clear();
}
