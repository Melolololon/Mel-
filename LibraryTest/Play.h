#pragma once
#include"Scene.h"
#include"Library.h"
#include"LibMath.h"
#include"Vector.h"
#include"FrameTimer.h"
#include"Curve.h"
#include"MouseCursor.h"
#include"ModelObject.h"
class Play:public Scene
{
private:
	FrameTimer ti;
	std::unique_ptr<ModelObject>pbrModel;

	Vector3 modelAngle = 0;
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

