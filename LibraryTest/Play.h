#pragma once
#include"Scene.h"
#include"Library.h"
#include"LibMath.h"
#include"Vector.h"
#include"FreamTimer.h"
#include"Curve.h"
#include"PipelineState.h"

class Play:public Scene
{
private:
	static const int CREATE_NUM = 1;
	std::unique_ptr<FbxModel> fbxModel;
	std::unique_ptr<Sprite2D> spr;
	std::unique_ptr<Texture> tex;
	std::unique_ptr<Curve> curve;

	PipelineState fbxPipeline;

	//FreamTimer ti;
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

