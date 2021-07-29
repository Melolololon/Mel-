#pragma once
#include"Scene.h"
#include"TuringPattern.h"
#include"SpringTestObject.h"
#include<memory>
#include"FrameTimer.h"
class PhysicsTest :public Scene
{
private:
	TuringPattern tPattern;

	Vector3 topPos = Vector3(0, 50, 0);
	Vector3 topRootPos = topPos + Vector3(0, 15, 0);
	static const int SPRING_OBJECT_NUM = 3;
	std::array<std::shared_ptr<SpringTestObject>, SPRING_OBJECT_NUM>springObjects;

	FrameTimer addTimer;


	Vector3 angle = 0;
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
