#pragma once
#include "Object.h"
#include"Library.h"

class MouseCursor :
	public Object
{
private:
	Vector2 position2D;
	Vector3 nearPos;
	Vector3 farPos;
	

public:
	MouseCursor();
	~MouseCursor();

	void Initialize()override;
	void Update()override;
	
	Vector2 GetPosition2D() { return position2D; }
	Vector3 GetNearPos() { return nearPos; }
	Vector3 GetFarPos() { return farPos; }
};

