#include "GameObject.h"



GameObject::GameObject()
{
	
}


GameObject::~GameObject()
{
}

void GameObject::Initialize()
{
}

void GameObject::Update() 
{
}

void GameObject::Draw() 
{

}

void GameObject::Hit
(
	const GameObject* const  object,
	const CollisionType& collisionType,
	const int& arrayNum
)
{
}

bool GameObject::GetEraseManager() { return eraseManager; }
void GameObject::ObjectInitialize() 
{
	eraseManager = false;
}

CollisionFlag GameObject::GetCollisionFlag() 
{
	return collisionFlag;
}


const void* GameObject::GetPtr()const
{
	return this;
}

std::vector<SphereData> GameObject::GetSphereData()
{
	return sphereData;
}

std::vector<BoxData> GameObject::GetBoxData()
{
	return boxData;
}

std::vector<LineSegmentData> GameObject::GetLineSegmentData()
{
	return lineSegmentData;
}

std::vector<PlaneData> GameObject::GetPlaneData()
{
	return planeData;
}

std::vector<BoardData> GameObject::GetBoardData()
{
	return boardData;
}

Vector3& GameObject::GetLineSegmentHitPosition(const int  num)
{
	return lineSegmentData[num].hitPos;
}

Vector3& GameObject::GetBoardHitPosition(const int  num)
{
	return boardData[num].hitPos;
}


//void Object::setLineSegmentDataHitPos(const Vector3& pos, const int& arrayNum)
//{
//	lineSegmentData[arrayNum].hitPos = pos;
//}
//
//void Object::setBoardDataHitPos(const Vector3& pos, const int& arrayNum)
//{
//	boardData[arrayNum].hitPos = pos;
//}
