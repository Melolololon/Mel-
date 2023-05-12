#pragma once
#include "GameObject.h"
#include"Library.h"

namespace MelLib
{
	// マウスカーソル
	class MouseCursor
	{
	private:
		Vector2 position2D;
		Vector3 nearPos;
		Vector3 farPos;


	public:
		MouseCursor();
		~MouseCursor();

		//void Initialize()override;
		void Update();

		Vector2 GetPosition2D() { return position2D; }
		Vector3 GetNearPos() { return nearPos; }
		Vector3 GetFarPos() { return farPos; }

	};
}
