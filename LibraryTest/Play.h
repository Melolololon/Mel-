#pragma once
#include"Scene.h"
#include"Sprite2D.h"
class Play:public melLib::Scene
{
private:
	std::unique_ptr<melLib::Sprite2D>sprite2DTest;
	melLib::Vector2 scale = 1;

public:
	Play();
	~Play();

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Finitialize()override;
	Scene* GetNextScene()override;

	static void LoadResources();
};

