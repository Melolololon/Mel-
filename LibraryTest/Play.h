#pragma once
#include"Scene.h"
#include"Sprite2D.h"
class Play:public MelLib::Scene
{
private:
	std::unique_ptr<MelLib::Sprite2D>sprite2DTest;
	MelLib::Vector2 scale = 1;

public:
	Play();
	~Play();

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Finalize()override;
	Scene* GetNextScene()override;

	static void LoadResources();
};

