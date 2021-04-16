#include "TestObject.h"

ModelData m;

TestObject::TestObject()
{
	m.key = "k";
	Library::create3DBox({ 1,1,1 }, m);
	Library::createHeapData2({ 244,244,244,255 }, 1, m);
	Library::setPosition({ 5,3,0 }, m, 0);
}
TestObject::~TestObject()
{
}

void TestObject::initialize()
{
}
void TestObject::update()
{
}
void TestObject::draw()
{
	Library::drawGraphic(m, 0);
}
