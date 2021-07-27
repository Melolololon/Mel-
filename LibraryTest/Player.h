#pragma once
#include"GameObject.h"
#include"ModelObject.h"
class Player :public GameObject
{
private:
	Vector3 angle = 0;
public:
	Player();
	void Update()override;
	void Draw()override;


};

