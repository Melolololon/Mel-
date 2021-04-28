#include "Object.h"



Object::Object()
{
	
}


Object::~Object()
{
}

void Object::Initialize()
{
}

void Object::Update() 
{
}

void Object::Draw() 
{

}

void Object::Hit
(
	const Object* const  object,
	const CollisionType& collisionType,
	const int& arrayNum
)
{
}

bool Object::GetEraseManager() { return eraseManager; }
void Object::ObjectInitialize() 
{
	eraseManager = false;
}

CollisionFlag Object::GetCollisionFlag() 
{
	return collisionFlag;
}


void* Object::GetPtr()
{
	return this;
}

std::vector<SphereData> Object::GetSphereData()
{
	return sphereData;
}

std::vector<BoxData> Object::GetBoxData()
{
	return boxData;
}

std::vector<LineSegmentData> Object::GetLineSegmentData()
{
	return lineSegmentData;
}

std::vector<PlaneData> Object::GetPlaneData()
{
	return planeData;
}

std::vector<BoardData> Object::GetBoardData()
{
	return boardData;
}

Vector3& Object::GetLineSegmentHitPosition(const int  num)
{
	return lineSegmentData[num].hitPos;
}

Vector3& Object::GetBoardHitPosition(const int  num)
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
