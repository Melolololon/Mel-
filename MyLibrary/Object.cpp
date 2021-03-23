#include "Object.h"



Object::Object()
{
	
}


Object::~Object()
{
}

void Object::initialize()
{
}

void Object::update() 
{
}

void Object::draw() 
{

}

void Object::hit
(
	const Object* const  object,
	const CollisionType& collisionType,
	const int& arrayNum
)
{
}

bool Object::getEraseManager() { return eraseManager; }
void Object::objectInitialize() 
{
	eraseManager = false;
}

CollisionFlag Object::getCollisionFlag() 
{
	return collisionFlag;
}

std::vector<SphereData> Object::getSphereData()
{
	return sphereData;
}

void* Object::getPtr() 
{
	return nullptr;
}

std::vector<LineSegmentData> Object::getLineSegmentData()
{
	return lineSegmentData;
}

std::vector<PlaneData> Object::getPlaneData()
{
	return planeData;
}

std::vector<BoardData> Object::getBoardData()
{
	return boardData;
}

Vector3& Object::getLineSegmentHitPosition(const int & num)
{
	return lineSegmentData[num].hitPos;
}

Vector3& Object::getBoardHitPosition(const int & num)
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
