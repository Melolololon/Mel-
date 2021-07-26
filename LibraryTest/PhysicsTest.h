#pragma once
#include"Scene.h"
#include"TuringPattern.h"
#include"SpringTestObject.h"
#include<memory>
class PhysicsTest :public Scene
{
private:
	TuringPattern tPattern;

	Vector3 topRootPos = Vector3(0, 0, 30);
	static const int SPRING_OBJECT_NUM = 4;
	std::array<std::shared_ptr<SpringTestObject>, SPRING_OBJECT_NUM>springObjects;
public:
	PhysicsTest();
	~PhysicsTest();

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Finitialize()override;
	Scene* GetNextScene()override;

	static void LoadResources();
};

