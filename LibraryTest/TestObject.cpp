#include "TestObject.h"

int TestObject::createCount;

TestObject::TestObject(const Vector3& pos)
{
	position = pos;

	m.key = "test" + createCount;
	Library::createBoard({ 4,4 }, m);
	Library::createHeapData2({ 255,255,0,150 }, 1, m);

	heapNum = createCount;
	createCount++;
	//createCount = createCount >= 2 ? 0 : createCount;
	Library::setPosition(position, m, 0);
}

TestObject::~TestObject()
{
}

void TestObject::initialize()
{
	velocity = 0;

	
}

void TestObject::update()
{

}

void TestObject::draw()
{
	Library::drawGraphic(m, 0);
}

