#include "Object.h"



Object::Object()
{
	
}


Object::~Object()
{
}

void Object::update() 
{
}

void Object::draw() 
{

}

void Object::hit(Object* object, const CollisionType& collisionType, const int& arrayNum)
{
}

bool Object::getIsDead() { return isDead; }
void Object::initializeObject() 
{
	isDead = false;
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
	return this;
}

std::vector<LineSegmentData>& Object::getLineSegmentData()
{
	return lineSegmentData;
}

std::vector<PlaneData> Object::getPlaneData()
{
	return planeData;
}

std::vector<BoardData>& Object::getBoardData()
{
	return boardData;
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
