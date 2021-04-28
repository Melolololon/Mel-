#include "TestObject.h"


TestObject::TestObject()
{
	m.key = "k";
	Library::Create3DBox({ 1,1,1 }, m);
	Library::CreateHeapData2({ 244,244,244,255 }, 1, m);
	Library::SetPosition({ 5,3,0 }, m, 0);
}
TestObject::~TestObject()
{
}

void TestObject::Initialize()
{
}
void TestObject::Update()
{
}
void TestObject::Draw()
{
	Library::DrawGraphic(m, 0);
}
