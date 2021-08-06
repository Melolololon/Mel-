#pragma once
#include<vector>
#include"DirectXStruct.h"

namespace MelLib 
{
	class TextureLoader
	{

	public:
		static BMPData LoadBMP(const char* texturepath);

	};
}
