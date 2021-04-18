#pragma once
#include"Object.h"
class TestObject :public Object
{

	ModelData m;
public:
	TestObject();
	~TestObject();

	void initialize()override;
	void update()override;
	void draw()override;
};

