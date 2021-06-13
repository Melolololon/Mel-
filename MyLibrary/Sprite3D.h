#pragma once
#include"Sprite.h"
#include"PipelineState.h"
class Sprite3D :public Sprite
{
private:

	static DirectX::XMMATRIX viewAndProjectionMatrix;
	static PipelineState defaultPipeline;


	bool billboardX = false;
	bool billboardY = false;
	bool billboardZ = false;
	void Billboard(DirectX::XMMATRIX& worldMat);
	static DirectX::XMFLOAT3 cameraPosition;
	static DirectX::XMFLOAT3 cameraTargetPosition;
	static DirectX::XMFLOAT3 cameraUpVector;

	void MatrixMap(Camera& camera);

	void Create(const Color& color)override;
	void Create(Texture* pTexture)override;

protected:

	void InitializeVertices();
public:
	Sprite3D(const Color& color);
	Sprite3D(Texture* pTexture);
	~Sprite3D();

	//レンダーターゲットでDrawを使うため、仮想関数にしてる
	virtual void Draw(const std::string& rtName = "")override;

#pragma region 開発者用関数


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




#pragma region 操作
	void SetPosition(const Vector3& pos) { constData.position = pos.ToXMFLOAT3(); }
	void SetAngle(const Vector3& angle) { constData.angle = angle.ToXMFLOAT3(); }
	void SetScale(const Vector2& scale) { constData.scale = scale.ToXMFLOAT2(); }
#pragma endregion
	void SetBillboardFlag(const bool flagX, const bool flagY, const bool flagZ);

	static PipelineState GetDefaultPipeline() { return defaultPipeline; }

	
};

