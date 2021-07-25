#pragma once
#include"Scene.h"
#include"TuringPattern.h"
class PhysicsTest :public Scene
{
private:
	TuringPattern tPattern;
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

