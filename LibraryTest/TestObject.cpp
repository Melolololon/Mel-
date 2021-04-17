#include "TestObject.h"


TestObject::TestObject()
{
	m.modelData.key = "k";
	Library::create3DBox({ 1,1,1 }, m.modelData);
	Library::createHeapData2({ 244,244,244,255 }, 1, m.modelData);
	Library::setPosition({ 5,3,0 }, m.modelData, 0);
}
TestObject::~TestObject()
{
	int z = 0;
}

void TestObject::initialize()
{
}
void TestObject::update()
{
}
void TestObject::draw()
{
	Library::drawGraphic(m.modelData, 0);
}
