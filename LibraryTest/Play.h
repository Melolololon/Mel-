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
	std::unique_ptr<ObjModel> objModel;
	std::unique_ptr<PrimitiveModel> priModel;

	std::unique_ptr<Sprite2D> sprite2D;
	std::unique_ptr<Sprite3D> sprite3D;
	std::unique_ptr<Texture> tex;

	FreamTimer testTime;
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

