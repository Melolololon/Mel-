#pragma once
#include"Sprite.h"
#include"PipelineState.h"
class Sprite3D :public Sprite
{
private:

	static DirectX::XMMATRIX viewAndProjectionMatrix;
	static PipelineState defaultPipeline;


public:
	Sprite3D();
	~Sprite3D();


	void CreateSprite(const Vector2& size);

	static bool Initialize();
	void Draw(Texture* texture);
	void SelectDrawAreaDraw(const Vector2& leftUpPos, const Vector2& rightDownPos, Texture* texture);

#pragma region ëÄçÏ
	void SetPosition(const Vector3& pos) { constData.position = pos.ToXMFLOAT3(); }
	void SetAngle(const Vector3& angle) { constData.angle = angle.ToXMFLOAT3(); }
	void SetScale(const Vector2& scale) { constData.scale = scale.ToXMFLOAT2(); }
#pragma endregion


	static PipelineState GetDefaultPipeline() { return defaultPipeline; }

	static void SetViewAndProjectionMatrix(const DirectX::XMMATRIX& cameraMat) { viewAndProjectionMatrix = cameraMat; }
};

