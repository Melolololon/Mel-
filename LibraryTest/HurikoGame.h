#pragma once
#include"Scene.h"
#include"TuringPattern.h"
#include"SpringTestObject.h"
#include<memory>
#include"FrameTimer.h"
class HurikoGame :public Scene
{
private:
	TuringPattern tPattern;

	Vector3 topPos = Vector3(0, 50, 0);
	Vector3 topRootPos = topPos + Vector3(0, 10, 0);
	Vector3 movePos = 0;
	static const int SPRING_OBJECT_NUM = 6;
	std::array<std::shared_ptr<SpringTestObject>, SPRING_OBJECT_NUM>springObjects;

	FrameTimer addTimer;


	Vector3 angle = 0;

	std::unique_ptr<ModelObject>topModel;

public:
	HurikoGame();
	~HurikoGame();

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Finitialize()override;
	Scene* GetNextScene()override;

	static void LoadResources();
};

