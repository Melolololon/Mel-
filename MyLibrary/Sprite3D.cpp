#include "Sprite3D.h"
PipelineState Sprite3D::defaultPipeline;
DirectX::XMMATRIX Sprite3D::viewAndProjectionMatrix;
DirectX::XMFLOAT3 Sprite3D::cameraPosition;
DirectX::XMFLOAT3 Sprite3D::cameraTargetPosition;
DirectX::XMFLOAT3 Sprite3D::cameraUpVector;

Sprite3D::Sprite3D()
{
	billboardX = false;
	billboardY = false;
	billboardZ = false;
}

Sprite3D::~Sprite3D()
{
}

void Sprite3D::Create(Texture* pTexture)
{
	this->pTexture = pTexture;
	//テクスチャがあったら描画範囲変更
	if (pTexture) drawRightDownPosition = pTexture->GetTextureSize();
	CreateBuffer();
	pipeline = defaultPipeline.GetPipelineState();
	
}

bool Sprite3D::Initialize()
{
	PipelineData data;
	data.alphaWriteMode = ALPHA_WRITE_TRUE;
	data.blendMode = BLEND_ADD;
	data.cullMode = CULL_NONE;
	data.depthMode = DEPTH_TRUE;
	data.drawMode = DRAW_SOLID;
	auto result = defaultPipeline.CreatePipeline
	(
		data,
		{ L"../MyLibrary/SpriteVertexShader.hlsl","VSmain","vs_5_0" },
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"../MyLibrary/SpritePixelShader.hlsl","PSmain","ps_5_0" },
		PipelineType::PIPELINE_TYPE_SPRITE,
		nullptr,
		typeid(Sprite3D).name()
	);
	if (!result)
	{
		OutputDebugString(L"Sprite3Dの初期化に失敗しました。デフォルトパイプラインを生成できませんでした\n");
		return false;
	}
	return true;
}

void Sprite3D::Draw()
{
	if (!pTexture)
	{
#ifdef _DEBUG
		OutputDebugString(L"描画に失敗しました。スプライト3Dにテクスチャがセットされていません。\n");
#endif // _DEBUG

		return;
	}

#pragma region map

	SpriteVertex* vertex;
	MapVertexBuffer((void**)&vertex);

#pragma region UV座標

	Vector2 textureSize = pTexture->GetTextureSize();
	Vector2 uvLeftUp = { 1.0f / textureSize.x * drawLeftUpPosition.x ,1.0f / textureSize.y * drawLeftUpPosition.y };
	Vector2 uvRightDown = { 1.0f / textureSize.x * drawRightDownPosition.x ,1.0f / textureSize.y * drawRightDownPosition.y };

	vertices[0].uv = { uvLeftUp.x ,uvRightDown.y };
	vertices[1].uv = { uvLeftUp.x,uvLeftUp.y };
	vertices[2].uv = { uvRightDown.x ,uvRightDown.y };
	vertices[3].uv = { uvRightDown.x ,uvLeftUp.y };
#pragma endregion
	auto vertexNum = vertices.size();
	for (int i = 0; i < vertexNum; i++)
		vertex[i].uv = vertices[i].uv;

	UnmapVertexBuffer();
#pragma endregion


	CommonDataMat();
	MatrixMap(pTexture);
	SetCmdList(pTexture);

}


void Sprite3D::MatrixMap(Texture* texture)
{
	SpriteConstBufferData* constBufferData;
	constBuffer->Map(0, nullptr, (void**)&constBufferData);

	DirectX::XMMATRIX matWorld = DirectX::XMMatrixIdentity();
	Billboard(matWorld);
	matWorld *= DirectX::XMMatrixScaling
	(
		constData.scale.x,
		constData.scale.y,
		1
	);
	matWorld *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(constData.angle.z));
	matWorld *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(constData.angle.x));
	matWorld *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(constData.angle.y));

	matWorld *= DirectX::XMMatrixTranslation
	(
		constData.position.x,
		constData.position.y,
		constData.position.z
	);

	constBufferData->mat = matWorld * viewAndProjectionMatrix;


	constBuffer->Unmap(0, nullptr);
}

void Sprite3D::SetBillboardFlag(const bool flagX, const bool flagY, const bool flagZ)
{
	billboardX = flagX;
	billboardY = flagY;
	billboardZ = flagZ;
}

void Sprite3D::Billboard(DirectX::XMMATRIX& worldMat)
{
	if (!billboardX && !billboardY && !billboardZ)return;

	//回転させた座標の取得

	DirectX::XMVECTOR vCPos = DirectX::XMLoadFloat3(&cameraPosition);
	DirectX::XMVECTOR vCTarget = DirectX::XMLoadFloat3(&cameraTargetPosition);
	DirectX::XMVECTOR upVector = DirectX::XMLoadFloat3(&cameraUpVector);
	//Z軸を求める
	DirectX::XMVECTOR cameraAxisZ = DirectX::XMVectorSubtract(vCTarget, vCPos);

	//除外
	assert(!DirectX::XMVector3Equal(cameraAxisZ, DirectX::XMVectorZero()));
	assert(!DirectX::XMVector3IsInfinite(cameraAxisZ));
	assert(!DirectX::XMVector3Equal(upVector, DirectX::XMVectorZero()));
	assert(!DirectX::XMVector3IsInfinite(upVector));

	//ベクトルを正規化
	cameraAxisZ = DirectX::XMVector3Normalize(cameraAxisZ);

	//X軸を求める
	DirectX::XMVECTOR cameraAxisX = DirectX::XMVector3Cross(upVector, cameraAxisZ);

	//正規化
	cameraAxisX = DirectX::XMVector3Normalize(cameraAxisX);

	//Y軸を求める
	DirectX::XMVECTOR cameraAxisY = DirectX::XMVector3Cross(cameraAxisZ, cameraAxisX);

	//全方位ビルボード行列の計算
	DirectX::XMMATRIX billboardMatrix = DirectX::XMMatrixIdentity();
	if (billboardX)billboardMatrix.r[0] = cameraAxisX;
	if (billboardY)billboardMatrix.r[1] = cameraAxisY;
	if (billboardZ)billboardMatrix.r[2] = cameraAxisZ;
	billboardMatrix.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

	worldMat *= billboardMatrix;

}

