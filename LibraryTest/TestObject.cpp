#include "TestObject.h"

int TestObject::createCount;

TestObject::TestObject(const Vector3& pos)
{
	position = pos;

	m.key = "test" + createCount;
	Library::createBoard({ 4,4 }, m);
	//Library::create3DBox({ 8,8,8 }, m);
	Library::createHeapData2({ 255,255,255,255 }, 1, m);

	heapNum = createCount;
	createCount++;
	//createCount = createCount >= 2 ? 0 : createCount;
	Library::setPosition(position, m, 0);

	angle = 0;
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
	if (DirectInput::keyState(DIK_A))
		angle.y += 3;
	if (DirectInput::keyState(DIK_D))
		angle.y -= 3;
	Library::setAngle(angle, m, 0);
}

void TestObject::draw()
{
	Library::drawGraphic(m, 0);
}

