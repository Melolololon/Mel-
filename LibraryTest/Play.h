#pragma once
#include"Scene.h"
#include"Library.h"
#include"LibMath.h"
#include"Vector.h"
#include"FreamTimer.h"
class Play:public Scene
{
private:
	std::unique_ptr<FbxModel> fbxModel;
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

