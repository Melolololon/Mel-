#pragma once
#include"Sprite.h"
#include"PipelineState.h"
class Sprite3D :public Sprite
{
private:

	static DirectX::XMMATRIX cameraMatrix;
	static PipelineState defaultPipeline;

public:
	Sprite3D(const Vector2& size);
	~Sprite3D();


	void CreateSprite(const Vector2& size);

	static void Initialize();

	static PipelineState GetDefaultPipeline() { return defaultPipeline; }
};

