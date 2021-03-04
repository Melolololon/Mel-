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

	void initialize()override;
	void update()override;
	void draw()override;
	void end()override;
	std::string getNextScene()override;

};

