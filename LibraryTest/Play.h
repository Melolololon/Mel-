#pragma once
#include"Scene.h"
#include"Library.h"
#include"LibMath.h"
#include"Vector.h"
#include"TestObject.h"
class Play:public Scene
{
private:

	std::shared_ptr<TestObject>t;
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

