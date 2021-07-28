#pragma once
#include"GameObject.h"
#include"ModelObject.h"
#include"Sprite2D.h"
#include"Texture.h"
class Player :public GameObject
{
private:
	Vector3 angle = 0;
	
	std::unique_ptr<Sprite2D>crossSpr;
public:
	Player();
	void Update()override;
	void Draw()override;

	void DrawCross();
};

