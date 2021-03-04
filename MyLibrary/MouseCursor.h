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

	void update()override;
	
	Vector2 getPosition2D() { return position2D; }
	Vector3 getNearPos() { return nearPos; }
	Vector3 getFarPos() { return farPos; }
};

