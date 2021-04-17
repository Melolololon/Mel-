#pragma once
#include"Object.h"
class TestObject :public Object
{

	SmartModelData m;
public:
	TestObject();
	~TestObject();

	void initialize()override;
	void update()override;
	void draw()override;
};

