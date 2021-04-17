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

	void initialize()override;
	void update()override;
	void draw()override;
	void end()override;
	Scene* getNextScene()override;

	static void loadResources();
};

