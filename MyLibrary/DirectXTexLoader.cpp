#include "DirectXTexLoader.h"



const DirectX::Image* DirectXTexLoader::loadTexture(const wchar_t* path, DirectX::TexMetadata* meta, DirectX::ScratchImage* scratchImage)
{
	auto result = LoadFromWICFile
	(
		path,
		DirectX::WIC_FLAGS_NONE,
		meta,
		*scratchImage
	);

	
	return scratchImage->GetImage(0, 0, 0);
}