#pragma once
#include"Object.h"
class TestObject :public Object
{
public:
	TestObject();
	~TestObject();

	void initialize()override;
	void update()override;
	void draw()override;
};

