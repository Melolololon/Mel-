#include"ModelObject.h"
#include"CreateBuffer.h"
#include"FbxLoader.h"
#include"DirectionalLight.h"

std::unordered_map<std::string, std::unique_ptr<ModelObject>>ModelObject::pModelObjects;

ID3D12Device* ModelObject::device;
std::vector<ID3D12GraphicsCommandList*>ModelObject::cmdLists;
ComPtr<ID3D12RootSignature> ModelObject::rootSignature;
PipelineState ModelObject::defaultPipeline;

ModelObject::ModelObject(ModelData* pModelData, ConstBufferData* userConstBufferData)
{
	Create(pModelData, userConstBufferData);
}

void ModelObject::CreateConstBuffer()
{
	ConstBufferData::BufferType modelConstBufferType = modelConstBufferData.bufferType;
	ConstBufferData::BufferType userConstBufferType = userConstBufferData.bufferType;

#pragma region リサイズ

	constBuffer.resize(modelFileObjectNum);
	materialConstBuffer.resize(modelFileObjectNum);
	pbrMaterialConstBuffer.resize(modelFileObjectNum);

	if (modelConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	{
		modelConstBuffer.resize(1);
	}
	else if (modelConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
	{
		modelConstBuffer.resize(modelFileObjectNum);
	}


	if (userConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	{
		userConstBuffer.resize(1);
	}
	else if (userConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
	{
		userConstBuffer.resize(modelFileObjectNum);
	}

#pragma endregion


	auto CreateBuffer = [&]
	(
		ID3D12Resource** pBuffer,
		const size_t& dataSize
		)
	{
		CreateBuffer::GetInstance()->CreateConstBuffer
		(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			dataSize,
			pBuffer
		);
	};


	//モデル
	if (modelConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	{
		CreateBuffer
		(
			&modelConstBuffer[0],
			modelConstBufferData.bufferDataSize
		);
	}

	//ユーザー
	if (userConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	{
		CreateBuffer
		(
			&userConstBuffer[0],
			userConstBufferData.bufferDataSize
		);
	}

	//モデル内のオブジェクト分ループ
	for (int i = 0; i < modelFileObjectNum; i++)
	{
#pragma region 生成


		//メイン
		CreateBuffer
		(
			&constBuffer[i],
			sizeof(ModelConstBufferData)
		);

		//マテリアル
		CreateBuffer
		(
			&materialConstBuffer[i],
			sizeof(MaterialConstBufferData)
		);

		//PBRマテリアル
		CreateBuffer
		(
			&pbrMaterialConstBuffer[i],
			sizeof(PbrMaterialConstBufferData)
		);

		//モデル
		if (modelConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
		{
			CreateBuffer
			(
				&modelConstBuffer[i],
				modelConstBufferData.bufferDataSize
			);
		}

		//ユーザー
		if (userConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
		{
			CreateBuffer
			(
				&userConstBuffer[i],
				userConstBufferData.bufferDataSize
			);
		}
#pragma endregion

#pragma region 初期化

		
		ModelConstBufferData* constBufferData;
		constBuffer[i]->Map(0, nullptr, (void**)&constBufferData);
		
		//ライト
		for (int j = 0, size = 20; j < size; j++)
		{
			constBufferData->light[j] = DirectX::XMFLOAT4(0, 0, 0, 0);
			constBufferData->lightColor[j] = DirectX::XMFLOAT4(0, 0, 0, 0);
		}
		
		constBuffer[i]->Unmap(0, nullptr);
#pragma endregion
	}


}

void ModelObject::DrawCommonProcessing(const std::string& rtName)
{
	if (!pModelData)return;
	MapConstData(RenderTarget::Get(rtName)->GetCamera());
	SetCmdList();
}

void ModelObject::MapConstData(const Camera* camera)
{
	ModelConstBufferData* constBufferData;

	for (int i = 0; i < modelFileObjectNum; i++)
	{
#pragma region 基本的な情報のマップ

		constBuffer[i]->Map(0, nullptr, (void**)&constBufferData);
		constBufferData->addColor = modelConstDatas[i].addColor;
		constBufferData->subColor = modelConstDatas[i].subColor;
		constBufferData->mulColor = modelConstDatas[i].mulColor;
		constBufferData->ex = modelConstDatas[i].pushPolygonNum;

		std::vector<DirectionalLight*> pLights = DirectionalLight::GetAll();
		for (int i = 0, size = pLights.size(); i < size; i++) 
		{
			Vector3 lightDir = pLights[i]->GetDirection();
			constBufferData->light[i] = DirectX::XMFLOAT4(lightDir.x, lightDir.y, lightDir.z, 0);
			Color lightCor = pLights[i]->GetColor();
			constBufferData->lightColor[i] = DirectX::XMFLOAT4
			(
				(float)lightCor.r / 255.0f,
				(float)lightCor.g / 255.0f,
				(float)lightCor.b / 255.0f,
				(float)lightCor.a / 255.0f
			);


		}

		Vector3 cameraPos = camera->GetCameraPosition();
		constBufferData->cameraPos = DirectX::XMFLOAT4(cameraPos.x, cameraPos.y, cameraPos.z, 0);

#pragma region 行列計算
		DirectX::XMMATRIX matWorld = DirectX::XMMatrixIdentity();

		matWorld *= DirectX::XMMatrixScaling
		(
			modelConstDatas[i].scale.x,
			modelConstDatas[i].scale.y,
			modelConstDatas[i].scale.z
		);
		matWorld *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(modelConstDatas[i].angle.z));
		matWorld *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(modelConstDatas[i].angle.x));
		matWorld *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(modelConstDatas[i].angle.y));

		constBufferData->normalMat = matWorld;

		matWorld *= DirectX::XMMatrixTranslation
		(
			modelConstDatas[i].position.x,
			modelConstDatas[i].position.y,
			modelConstDatas[i].position.z
		);


		constBufferData->mat = matWorld * camera->GetViewAndProjectionMat();
		constBufferData->worldMat = matWorld;

#pragma endregion


		constBuffer[i]->Unmap(0, nullptr);

#pragma endregion

#pragma region マテリアルのマップ
		MaterialConstBufferData* materialConstData;

		materialConstBuffer[i]->Map(0, nullptr, (void**)&materialConstData);

		materialConstData->ambient = materials[i].ambient;
		materialConstData->diffuse = materials[i].diffuse;
		materialConstData->specular = materials[i].specular;
		materialConstData->alpha = materials[i].alpha;

		materialConstBuffer[i]->Unmap(0, nullptr);

#pragma endregion


#pragma region PBRマテリアルのマップ
		PbrMaterialConstBufferData* pbrMaterialConstData;

		pbrMaterialConstBuffer[i]->Map(0, nullptr, (void**)&pbrMaterialConstData);
	

		pbrMaterialConstData->baseColor = pbrMaterials[i].baseColor;
		pbrMaterialConstData->metalness = pbrMaterials[i].metalness;
		pbrMaterialConstData->fSpecular = pbrMaterials[i].fSpecular;
		pbrMaterialConstData->roughness = pbrMaterials[i].roughness;

		//テスト用
		pbrMaterialConstData->baseColor = pbrMaterials[i].baseColor;
		pbrMaterialConstData->metalness = 1;
		pbrMaterialConstData->fSpecular = 0.5;
		pbrMaterialConstData->roughness = 0.3;

		pbrMaterialConstBuffer[i]->Unmap(0, nullptr);
#pragma endregion



#pragma region ボーンのマップ

		int boneNum = pModelData->GetBoneNumber();

		if (boneNum == 0
			|| pModelData->GetModelFormat() != ModelData::ModelFormat::MODEL_FORMAT_OBJ
			&& i != 0)return;

		SkinConstBufferData* skinConstData;

		modelConstBuffer[i]->Map(0, nullptr, (void**)&skinConstData);

		if (pModelData->GetModelFormat() == ModelData::ModelFormat::MODEL_FORMAT_OBJ)
		{
			std::vector<Vector3>objBonePositions = pModelData->GetObjBonePosition();

			DirectX::XMMATRIX boneMat = DirectX::XMMatrixIdentity();
			DirectX::XMFLOAT3 boneScale;
			DirectX::XMFLOAT3 boneAngle;
			DirectX::XMFLOAT3 boneMoveVector;

			//親ボーンの行列乗算
			int parentBoneNum = 0;
			int bone = 0;

			DirectX::XMFLOAT3 bonePos = { 0,0,0 };


			for (UINT j = 0; j < boneNum; j++)
			{
				boneMat = DirectX::XMMatrixIdentity();

				//ボーンから頂点の距離分移動
				bonePos = objBonePositions[j].ToXMFLOAT3();
				boneMat *= DirectX::XMMatrixTranslation(-bonePos.x, -bonePos.y, -bonePos.z);

				boneScale = boneDatas[j].scale;
				boneMat *= DirectX::XMMatrixScaling(boneScale.x, boneScale.y, boneScale.z);

				boneAngle = boneDatas[j].angle;
				boneMat *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(boneAngle.z));
				boneMat *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(boneAngle.x));
				boneMat *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(boneAngle.y));

				boneMoveVector = boneDatas[j].moveVector;

				//モデル自体のスケールの乗算により、ボーンの平行移動の値にスケールが乗算されるため、
				//割って増減を抑えている
				//ボーン→行列の順番で掛けるからモデルの倍率が影響しちゃう
				boneMat *= DirectX::XMMatrixTranslation
				(
					boneMoveVector.x / modelConstDatas[i].scale.x,
					boneMoveVector.y / modelConstDatas[i].scale.y,
					boneMoveVector.z / modelConstDatas[i].scale.z
				);

				//回転させたら戻す
				boneMat *= DirectX::XMMatrixTranslation(bonePos.x, bonePos.y, bonePos.z);

#pragma region 親の処理


				DirectX::XMFLOAT3 boneAngleImpact = { 0.0f,0.0f,0.0f };
				DirectX::XMFLOAT3 boneScaleImpact = { 1.0f,1.0f,1.0f };
				DirectX::XMFLOAT3 boneMoveVectorImpact = { 0.0f,0.0f,0.0f };
				boneAngleImpact = parentBoneDatas[j].angleImpact;
				boneScaleImpact = parentBoneDatas[j].scaleImpact;
				boneMoveVectorImpact = parentBoneDatas[j].moveVectorImpact;

				//親のボーン番号代入
				parentBoneNum = parentBoneDatas[j].parentBoneNum;

				//これを使って影響度とか加算してく
				//子　→　親の順で入れる
				std::vector<int>parentNums;
				parentNums.reserve(50);

				//こちらは親　→　子
				std::vector<DirectX::XMMATRIX>parentMat;
				parentMat.reserve(50);


				//先にすべての親を取得&影響度取得
				while (1)
				{
					//-1だったら(親がセットされてなかったら)抜ける
					if (parentBoneNum == -1)break;
					parentNums.push_back(parentBoneNum);

					//親のボーン番号代入
					parentBoneNum = parentBoneDatas[parentBoneNum].parentBoneNum;

				}
				if (parentNums.size() != 0)
				{

					DirectX::XMMATRIX mulMat = DirectX::XMMatrixIdentity();

					//親数
					const int maxParentSize = static_cast<int>(parentNums.size());

					DirectX::XMFLOAT3 pAngle = { 0,0,0 };
					DirectX::XMFLOAT3 pScale = { 1,1,1 };
					DirectX::XMFLOAT3 pMoveVector = { 0,0,0 };
					DirectX::XMFLOAT3 pPos = { 0,0,0 };

					DirectX::XMFLOAT3 pAngleImpact = { 1,1,1 };
					DirectX::XMFLOAT3 pScaleImpact = { 1,1,1 };
					DirectX::XMFLOAT3 pMoveVectorImpact = { 1,1,1 };

					mulMat = DirectX::XMMatrixIdentity();

					//最後にある親のボーンを基準に回すので、入れる
					pPos.x = objBonePositions[parentNums[maxParentSize - 1]].x;
					pPos.y = objBonePositions[parentNums[maxParentSize - 1]].y;
					pPos.z = objBonePositions[parentNums[maxParentSize - 1]].z;

					int impactIndex = j;
					for (auto& pNum : parentNums)
					{
						pAngle.x += boneDatas[pNum].angle.x;
						pAngle.y += boneDatas[pNum].angle.y;
						pAngle.z += boneDatas[pNum].angle.z;

						pScale.x *= boneDatas[pNum].scale.x;
						pScale.y *= boneDatas[pNum].scale.y;
						pScale.z *= boneDatas[pNum].scale.z;

						pMoveVector.x += boneDatas[pNum].moveVector.x;
						pMoveVector.y += boneDatas[pNum].moveVector.y;
						pMoveVector.z += boneDatas[pNum].moveVector.z;



						pAngleImpact.x *= parentBoneDatas[impactIndex].angleImpact.x;
						pAngleImpact.y *= parentBoneDatas[impactIndex].angleImpact.y;
						pAngleImpact.z *= parentBoneDatas[impactIndex].angleImpact.z;
						pScaleImpact.x *= parentBoneDatas[impactIndex].scaleImpact.x;
						pScaleImpact.y *= parentBoneDatas[impactIndex].scaleImpact.y;
						pScaleImpact.z *= parentBoneDatas[impactIndex].scaleImpact.z;
						pMoveVectorImpact.x *= parentBoneDatas[impactIndex].moveVectorImpact.x;
						pMoveVectorImpact.y *= parentBoneDatas[impactIndex].moveVectorImpact.y;
						pMoveVectorImpact.z *= parentBoneDatas[impactIndex].moveVectorImpact.z;

						impactIndex = pNum;
					}

					pAngle.x *= pAngleImpact.x;
					pAngle.y *= pAngleImpact.y;
					pAngle.z *= pAngleImpact.z;

					pScale.x *= pScaleImpact.x;
					pScale.y *= pScaleImpact.y;
					pScale.z *= pScaleImpact.z;

					pMoveVector.x *= pMoveVectorImpact.x;
					pMoveVector.y *= pMoveVectorImpact.y;
					pMoveVector.z *= pMoveVectorImpact.z;


					//ボーンから頂点の距離分移動
					mulMat *= DirectX::XMMatrixTranslation(-pPos.x, -pPos.y, -pPos.z);

					mulMat *= DirectX::XMMatrixScaling(pScale.x, pScale.y, pScale.z);

					mulMat *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(pAngle.z));
					mulMat *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(pAngle.x));
					mulMat *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(pAngle.y));


					//親はまとめて掛けているので、モデルと自分の拡縮だけ掛ければいい
					//modelConstDatas[i][0].scale に boneScale掛ける必要あるか要確認
					mulMat *= DirectX::XMMatrixTranslation
					(
						pMoveVector.x / (modelConstDatas[i].scale.x * boneScale.x),
						pMoveVector.y / (modelConstDatas[i].scale.y * boneScale.y),
						pMoveVector.z / (modelConstDatas[i].scale.z * boneScale.z)
					);

					//回転させたら戻す
					mulMat *= DirectX::XMMatrixTranslation(pPos.x, pPos.y, pPos.z);


					boneMat *= mulMat;
				}
#pragma endregion

				skinConstData->bones[j + 1] = boneMat;

			}


		}
		else if (pModelData->GetModelFormat() == ModelData::ModelFormat::MODEL_FORMAT_FBX)
		{

			std::vector<ModelData::FbxBone> bones = pModelData->GetFbxBone();

			for (int i = 0; i < boneNum; i++)
			{
				//変換
				DirectX::XMMATRIX matCurrentPose;
				FbxAMatrix fbxCurrentPose =
					bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(fbxAnimationData.currentTime);
				FbxLoader::GetInstance()->ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);

				//乗算
				skinConstData->bones[i] = bones[i].invInitialPose * matCurrentPose;

			}
		}

		modelConstBuffer[i]->Unmap(0, nullptr);
#pragma endregion

	}


}

void ModelObject::SetCmdList()
{

	cmdLists[0]->SetGraphicsRootSignature(rootSignature.Get());
	cmdLists[0]->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	ID3D12DescriptorHeap* textureDescHeap = pModelData->GetTextureDesctiptorHeap();
	std::vector<ID3D12DescriptorHeap*> ppHeaps;
	ppHeaps.push_back(textureDescHeap);
	cmdLists[0]->SetDescriptorHeaps(1, &ppHeaps[0]);


	//モデル特有バッファセット
	if (modelConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
		cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, modelConstBuffer[0]->GetGPUVirtualAddress());

	//ユーザーモデルバッファセット
	if (userConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
		cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, userConstBuffer[0]->GetGPUVirtualAddress());



	const std::vector<VertexBufferSet>& vertexBufferSets = pModelData->GetVertexBufferSet();
	const std::vector<IndexBufferSet>& indexBufferSets = pModelData->GetIndexBufferSet();


	//モデルのオブジェクトごとをセット
	//頂点バッファ分ループ
	for (int i = 0; i < modelFileObjectNum; i++)
	{
		cmdLists[0]->SetPipelineState(pPipeline[i]->GetPipelineState().Get());


		cmdLists[0]->IASetVertexBuffers(0, 1, &vertexBufferSets[i].vertexBufferView);
		cmdLists[0]->IASetIndexBuffer(&indexBufferSets[i].indexBufferView);

#pragma region テクスチャ

		////紐づけ
		//for (int j = 0; j < vertexBufferNum; j++)
		//{
		//	//マテリアル紐づけ
		//	//紐づけ失敗(マテリアル名未設定などで)だと、一番最初のテクスチャが選ばれる
		//	if (vertexBufferSet[i].materialName == materials[j].materialName)
		//	{
		//		textureBufferHandleNum = j;
		//		break;
		//	}
		//}
		//読み取ったモデルとモデルのテクスチャの順番同じだろうから紐づけいらない?

		D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
		(
			textureDescHeap->GetGPUDescriptorHandleForHeapStart(),
			i,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);
		cmdLists[0]->SetGraphicsRootDescriptorTable(TEXURE_ROOTPARAM_NUM, gpuDescHandle);



		////2枚目(テスト用)
		//textureBufferHandleNum = 1;
		//gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
		//(
		//	desHeap->GetGPUDescriptorHandleForHeapStart(),
		//	textureBufferHandleNum,
		//	device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		//);
		//cmdLists[0]->SetGraphicsRootDescriptorTable(5, gpuDescHandle);

#pragma endregion

#pragma region 定数



		//定数バッファ
		cmdLists[0]->SetGraphicsRootConstantBufferView(CONST_BUFFER_REGISTER, constBuffer[i]->GetGPUVirtualAddress());

		//マテリアルバッファ
		cmdLists[0]->SetGraphicsRootConstantBufferView(MATERIAL_BUFFER_REGISTER, materialConstBuffer[i]->GetGPUVirtualAddress());

		//PBRマテリアル
		cmdLists[0]->SetGraphicsRootConstantBufferView(PBR_MATERIAL_BUFFER_REGISTER, pbrMaterialConstBuffer[i]->GetGPUVirtualAddress());

		//モデルバッファ
		if (modelConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
			cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, modelConstBuffer[i]->GetGPUVirtualAddress());

		//ユーザー定数バッファ
		if (userConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
			cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, userConstBuffer[i]->GetGPUVirtualAddress());


#pragma endregion


		//描画
		cmdLists[0]->DrawIndexedInstanced(static_cast<UINT>(pModelData->GetIndices()[i].size()), 1, 0, 0, 0);

	}

}

void ModelObject::Draw(const std::string& rtName)
{
	FbxAnimation();

	DrawCommonProcessing(rtName);
}

void ModelObject::SetPosition(const Vector3& position)
{
	for (int i = 0; i < modelFileObjectNum; i++)
	{
		modelConstDatas[i].position = position.ToXMFLOAT3();
	}
}

void ModelObject::SetScale(const Vector3& scale)
{
	for (int i = 0; i < modelFileObjectNum; i++)
	{
		modelConstDatas[i].scale = scale.ToXMFLOAT3();
	}
}

void ModelObject::SetAngle(const Vector3& angle)
{
	for (int i = 0; i < modelFileObjectNum; i++)
	{
		modelConstDatas[i].angle = angle.ToXMFLOAT3();
	}
}


bool ModelObject::Initialize(ID3D12Device* dev, const std::vector<ID3D12GraphicsCommandList*>& cmdList)
{
	device = dev;
	cmdLists = cmdList;


#pragma region ディスクリプタレンジ_ルートパラメーター
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER rootparam[6] = {};

	//行列や色など
	rootparam[CONST_BUFFER_REGISTER].InitAsConstantBufferView(CONST_BUFFER_REGISTER, 0, D3D12_SHADER_VISIBILITY_ALL);
	//ユーザー定数
	rootparam[USER_BUFFER_REGISTER].InitAsConstantBufferView(USER_BUFFER_REGISTER, 0, D3D12_SHADER_VISIBILITY_ALL);
	//マテリアル
	rootparam[MATERIAL_BUFFER_REGISTER].InitAsConstantBufferView(MATERIAL_BUFFER_REGISTER, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	//PBRマテリアル
	rootparam[PBR_MATERIAL_BUFFER_REGISTER].InitAsConstantBufferView(PBR_MATERIAL_BUFFER_REGISTER, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	//モデルごと
	rootparam[MODEL_BUFFER_REGISTER].InitAsConstantBufferView(MODEL_BUFFER_REGISTER, 0, D3D12_SHADER_VISIBILITY_ALL);

	//テクスチャ
	rootparam[TEXURE_ROOTPARAM_NUM].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_PIXEL);

#pragma endregion

#pragma region ルートシグネチャ


	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	rootSignatureDesc.pParameters = rootparam;
	rootSignatureDesc.NumParameters = _countof(rootparam);


	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	ComPtr<ID3DBlob>rootSigBlob;
	ComPtr<ID3DBlob>errorBlob;

	auto result = D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);


	if (FAILED(result))
	{
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";

		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	result = device->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));

#pragma endregion

	PipelineState::SetModelRootSignature(rootSignature.Get());

#pragma region パイプライン
	PipelineData data;
	data.alphaWrite = true;
	data.blendMode = BlendMode::ADD;
	data.cullMode = CullMode::BACK;
	data.depthTest = true;
	data.drawMode = DrawMode::SOLID;

	//インプットレイアウト
	std::vector<InputLayoutData> ilData(5);
	ilData[0].formatType = FORMAT_TYPE::FLOAT;
	ilData[0].number = 3;
	ilData[0].semantics = "POSITION";
	ilData[1].formatType = FORMAT_TYPE::FLOAT;
	ilData[1].number = 2;
	ilData[1].semantics = "TEXCOORD";
	ilData[2].formatType = FORMAT_TYPE::FLOAT;
	ilData[2].number = 3;
	ilData[2].semantics = "NORMAL";

	ilData[3].formatType = FORMAT_TYPE::UNSIGNED_INT;
	ilData[3].number = 4;
	ilData[3].semantics = "BONEINDICES";
	ilData[4].formatType = FORMAT_TYPE::FLOAT;
	ilData[4].number = 4;
	ilData[4].semantics = "BONEWEIGHTS";

	bool pResult = defaultPipeline.CreatePipeline
	(
		data,
		{ L"../MyLibrary/FbxVertexShader.hlsl","main","vs_5_0" },
		{ L"../MyLibrary/FbxGeometryShader.hlsl","main","gs_5_0" },
		{ L"LIB","","" },
		{ L"LIB","","" },
		{ L"../MyLibrary/FbxPixelShader.hlsl","main","ps_5_0" },
		PipelineType::MODEL,
		&ilData,
		typeid(ModelObject).name(),
		1
	);

	if (!pResult)
	{
		OutputDebugString(L"ModelObjectの初期化に失敗しました。デフォルトパイプラインを生成できませんでした。\n");
		return false;
	}
	return true;
#pragma endregion

}

void ModelObject::SetPipeline(PipelineState* pipelineState)
{
	for (int i = 0; i < modelFileObjectNum; i++)
	{
		pPipeline[i] = pipelineState;
	}
}

bool ModelObject::Create(ModelData* pModelData, ConstBufferData* userConstBufferData, const std::string& name)
{
	if(!pModelData)
	{
#ifdef _DEBUG
		
		OutputDebugStringA(name.data());
		OutputDebugStringW(L"の生成に失敗しました。\n");
#endif // _DEBUG
		return false;
	}

	pModelObjects.emplace(name, std::make_unique<ModelObject>(pModelData, userConstBufferData));
	

	return true;
}

void ModelObject::Delete(const std::string& name)
{
	pModelObjects.erase(name);
}

void ModelObject::ResetAnimation()
{
	fbxAnimationData.currentTime = fbxAnimationData.animationTimes.startTime;
}

void ModelObject::SetCurrentFream(const UINT fream)
{
	FbxTime setTime = fbxAnimationData.animationTimes.startTime * fream;
	if (setTime > fbxAnimationData.animationTimes.endTime)
		setTime = fbxAnimationData.animationTimes.endTime;

	fbxAnimationData.currentTime = setTime;
}



void ModelObject::FbxAnimation()
{
	if (!isAnimation)return;

	//タイムを進める
	fbxAnimationData.currentTime += fbxAnimationData.animationTimes.freamTime * fbxAnimationData.timeMag;

	if (fbxAnimationData.currentTime  > fbxAnimationData.animationTimes.endTime)
		fbxAnimationData.currentTime  = fbxAnimationData.animationTimes.startTime;
	if (fbxAnimationData.currentTime  < fbxAnimationData.animationTimes.startTime)
		fbxAnimationData.currentTime  = fbxAnimationData.animationTimes.endTime;
}

bool ModelObject::Create(ModelData* pModelData, ConstBufferData* userConstBufferData)
{
	if (!pModelData)
	{
#ifdef _DEBUG
		OutputDebugStringW(L"pModelDataがnullptrです。生成に失敗しました\n");
#endif // _DEBUG
		return false;
	}

#pragma region データセット
	if (userConstBufferData)this->userConstBufferData = *userConstBufferData;

	//objのボーンのMoveVectorの割る処理のために、オブジェクトごとにバッファ作成
	if (pModelData->GetModelFormat() == ModelData::ModelFormat::MODEL_FORMAT_OBJ)
	{
		modelConstBufferData.bufferType = ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT;
	}
	else
	{
		modelConstBufferData.bufferType = ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL;
	}
	modelConstBufferData.bufferDataSize = sizeof(SkinConstBufferData);

	modelFileObjectNum = pModelData->GetModelFileObjectNumber();
	this->pModelData = pModelData;

#pragma endregion



	CreateConstBuffer();
	
	modelConstDatas.resize(modelFileObjectNum);
	
#pragma region マテリアル


	materials.resize(modelFileObjectNum);

	for(int i = 0; i < modelFileObjectNum;i++)
	{
		materials[i] = pModelData->GetMaterial(i);
	}

#pragma endregion

#pragma region PBRマテリアル
	pbrMaterials.resize(modelFileObjectNum);

	for (int i = 0; i < modelFileObjectNum; i++)
	{
		pbrMaterials[i] = pModelData->GetPbrMaterial(i);
	}
#pragma endregion


#pragma region アニメーション関係


	boneDatas.resize(pModelData->GetBoneNumber());
	parentBoneDatas.resize(pModelData->GetBoneNumber());
	SkinConstBufferData* skinConstData = nullptr;
	modelConstBuffer[0]->Map(0, nullptr, (void**)&skinConstData);
	for (int i = 0; i < BONE_MAX; i++)
	{
		skinConstData->bones[i] = DirectX::XMMatrixIdentity();
	}
	modelConstBuffer[0]->Unmap(0, nullptr);

	fbxAnimationData.animationTimes = pModelData->GetFbxAnimationTimes();
#pragma endregion

	pPipeline.resize(modelFileObjectNum, &defaultPipeline);

	return true;
}
