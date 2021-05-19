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
	std::unique_ptr<Sprite3D> spr;
	std::unique_ptr<Texture> tex;
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

