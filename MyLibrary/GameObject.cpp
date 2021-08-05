#include "GameObject.h"

#include"Physics.h"


using namespace melLib;

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
	const CollisionType collisionType,
	const int arrayNum,
	const CollisionType hitObjColType,
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

bool GameObject::GetEraseManager() { return eraseManager; }
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
//
//CollisionFlag GameObject::GetCollisionFlag() 
//{
//	return collisionFlag;
//}
//
//
//const void* GameObject::GetPtr()const
//{
//	return this;
//}
//
//std::vector<SphereData> GameObject::GetSphereData()
//{
//	return sphereData;
//}
//
//std::vector<BoxData> GameObject::GetBoxData()
//{
//	return boxData;
//}
//
//std::vector<LineSegment3DData> GameObject::GetLineSegmentData()
//{
//	return lineSegmentData;
//}
//
//std::vector<PlaneData> GameObject::GetPlaneData()
//{
//	return planeData;
//}
//
//std::vector<BoardData> GameObject::GetBoardData()
//{
//	return boardData;
//}
//
//Vector3& GameObject::GetLineSegmentHitPosition(const int  num)
//{
//	return lineSegmentData[num].hitPos;
//}
//
//Vector3& GameObject::GetBoardHitPosition(const int  num)
//{
//	return boardData[num].hitPos;
//}


//void Object::setLineSegmentDataHitPos(const Vector3& pos, const int& arrayNum)
//{
//	lineSegmentData[arrayNum].hitPos = pos;
//}
//
//void Object::setBoardDataHitPos(const Vector3& pos, const int& arrayNum)
//{
//	boardData[arrayNum].hitPos = pos;
//}
