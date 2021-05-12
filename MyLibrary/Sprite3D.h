#pragma once
#include"Sprite.h"
#include"PipelineState.h"
class Sprite3D :public Sprite
{
private:

	static DirectX::XMMATRIX viewAndProjectionMatrix;
	static PipelineState defaultPipeline;


	bool billboardX;
	bool billboardY;
	bool billboardZ;
	void Billboard(DirectX::XMMATRIX& worldMat);
	static DirectX::XMFLOAT3 cameraPosition;
	static DirectX::XMFLOAT3 cameraTargetPosition;
	static DirectX::XMFLOAT3 cameraUpVector;

	void MatrixMap(Texture* texture);
public:
	Sprite3D();
	~Sprite3D();


	void CreateSprite(const Vector2& size);

#pragma region �J���җp�֐�


	static bool Initialize();

	static void SetViewAndProjectionMatrix(const DirectX::XMMATRIX& cameraMat) { viewAndProjectionMatrix = cameraMat; }
	static void SetCameraPosTargetUpVector
	(
		const DirectX::XMFLOAT3& pos,
		const DirectX::XMFLOAT3& target,
		const DirectX::XMFLOAT3& up
	)
	{
		cameraPosition = pos;
		cameraTargetPosition = target;
		cameraUpVector = up;
	}
#pragma endregion

	void Draw(Texture* texture)override;

	/// <summary>
	/// �\���͈͂��w�肵�ĕ`�悵�܂��B
	/// </summary>
	/// <param name="leftUpPos">������W</param>
	/// <param name="rightDownPos">�E�����W</param>
	/// <param name="texture"></param>
	void SelectDrawAreaDraw(const Vector2& leftUpPos, const Vector2& rightDownPos, Texture* texture);

#pragma region ����
	void SetPosition(const Vector3& pos) { constData.position = pos.ToXMFLOAT3(); }
	void SetAngle(const Vector3& angle) { constData.angle = angle.ToXMFLOAT3(); }
	void SetScale(const Vector2& scale) { constData.scale = scale.ToXMFLOAT2(); }
#pragma endregion
	void SetBillboardFlag(const bool flagX, const bool flagY, const bool flagZ);

	static PipelineState GetDefaultPipeline() { return defaultPipeline; }

	
};

