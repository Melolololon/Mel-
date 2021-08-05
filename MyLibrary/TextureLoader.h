#pragma once
#include<vector>
#include"DirectXStruct.h"

namespace melLib 
{
	class TextureLoader
	{

	public:
		static BMPData LoadBMP(const char* texturepath);

	};
}
