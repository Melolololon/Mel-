#pragma once
#include<DirectXTex.h>
#include"DirectXStruct.h"
class DirectXTexLoader
{
public:

	static const DirectX::Image* loadTexture(const wchar_t* path, DirectX::TexMetadata* meta, DirectX::ScratchImage* scratchImage);
	
};

