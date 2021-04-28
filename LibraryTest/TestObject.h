#pragma once
#include"Object.h"
class TestObject :public Object
{

	ModelData m;
public:
	TestObject();
	~TestObject();

	void Initialize()override;
	void Update()override;
	void Draw()override;
};

