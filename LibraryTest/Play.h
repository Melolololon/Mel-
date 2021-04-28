#pragma once
#include"Scene.h"
#include"Library.h"
#include"LibMath.h"
#include"Vector.h"
class Play:public Scene
{
private:

public:
	Play();
	~Play();

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void end()override;
	Scene* GetNextScene()override;

	static void LoadResources();
};

