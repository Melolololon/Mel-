#pragma once
#include"Sprite.h"
#include"ModelObject.h"

namespace MelLib 
{
	class DrawManager final
	{
	private:
		std::vector<ModelObject*>pModelObjects;
		std::vector<Sprite*> pSpriteDatas;

	private:
		DrawManager() {}
		~DrawManager() {}

	public:

		DrawManager(DrawManager& d) = delete;
		DrawManager& operator=(DrawManager& d) = delete;

		void SetDraw(Sprite* sprite);
		void SetDraw(ModelObject* model);

	};

}