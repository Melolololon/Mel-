#include"ModelObject.h"
#include"CreateBuffer.h"
#include"FbxLoader.h"
#include"DirectionalLight.h"
#include"Matrix.h"
#include"CollisionType.h"
#include"LibMath.h"
#include"Values.h"
#include"Collision.h"

using namespace MelLib;

std::unordered_map<std::string, std::unique_ptr<ModelObject>>ModelObject::pModelObjects;


ID3D12Device* ModelObject::device;
std::vector<ID3D12GraphicsCommandList*>ModelObject::cmdLists;
ComPtr<ID3D12RootSignature> ModelObject::rootSignature;

ModelObject::ModelObject(ModelData* pModelData, ConstBufferData* userConstBufferData)
{
	Create(pModelData, userConstBufferData);
}

void ModelObject::CreateConstBuffer()
{
	ConstBufferData::BufferType modelConstBufferType = modelConstBufferData.bufferType;
	ConstBufferData::BufferType userConstBufferType = userConstBufferData.bufferType;

#pragma region ���T�C�Y

	constBuffer.resize(modelFileObjectNum);


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


	//���f��
	if (modelConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	{
		CreateBuffer
		(
			&modelConstBuffer[0],
			modelConstBufferData.bufferDataSize
		);
	}

	//���[�U�[
	if (userConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	{
		CreateBuffer
		(
			&userConstBuffer[0],
			userConstBufferData.bufferDataSize
		);
	}

	//���f�����̃I�u�W�F�N�g�����[�v
	for (int i = 0; i < modelFileObjectNum; i++)
	{
#pragma region ����


		//���C��
		CreateBuffer
		(
			&constBuffer[i],
			sizeof(ModelConstBufferData)
		);


		//���f��
		if (modelConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
		{
			CreateBuffer
			(
				&modelConstBuffer[i],
				modelConstBufferData.bufferDataSize
			);
		}

		//���[�U�[
		if (userConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
		{
			CreateBuffer
			(
				&userConstBuffer[i],
				userConstBufferData.bufferDataSize
			);
		}
#pragma endregion

#pragma region ������

		ModelConstBufferData* constBufferData;
		constBuffer[i]->Map(0, nullptr, (void**)&constBufferData);

		//���C�g
		for (int j = 0, size = DirectionalLight::LIGTH_MAX; j < size; j++)
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
#pragma region ��{�I�ȏ��̃}�b�v

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

#pragma region �s��v�Z
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


#pragma region �����_�����O����
		//�㉺���E�O��
		std::array<float, 6>dirPos = pModelData->GetDirectionMaxPosition()[i];

		//���ɑO�����
		constBufferData->tex3DStart = dirPos[4];
		constBufferData->tex3DEnd = dirPos[5];
#pragma endregion


		constBuffer[i]->Unmap(0, nullptr);

#pragma endregion

#pragma region �{�[���̃}�b�v

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

			//�e�{�[���̍s���Z
			int parentBoneNum = 0;
			int bone = 0;

			DirectX::XMFLOAT3 bonePos = { 0,0,0 };


			for (UINT j = 0; j < boneNum; j++)
			{
				boneMat = DirectX::XMMatrixIdentity();

				//�{�[�����璸�_�̋������ړ�
				bonePos = objBonePositions[j].ToXMFLOAT3();
				boneMat *= DirectX::XMMatrixTranslation(-bonePos.x, -bonePos.y, -bonePos.z);

				boneScale = boneDatas[j].scale;
				boneMat *= DirectX::XMMatrixScaling(boneScale.x, boneScale.y, boneScale.z);

				boneAngle = boneDatas[j].angle;
				boneMat *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(boneAngle.z));
				boneMat *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(boneAngle.x));
				boneMat *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(boneAngle.y));

				boneMoveVector = boneDatas[j].moveVector;

				//���f�����̂̃X�P�[���̏�Z�ɂ��A�{�[���̕��s�ړ��̒l�ɃX�P�[������Z����邽�߁A
				//�����đ�����}���Ă���
				//�{�[�����s��̏��ԂŊ|���邩�烂�f���̔{�����e�������Ⴄ
				boneMat *= DirectX::XMMatrixTranslation
				(
					boneMoveVector.x / modelConstDatas[i].scale.x,
					boneMoveVector.y / modelConstDatas[i].scale.y,
					boneMoveVector.z / modelConstDatas[i].scale.z
				);

				//��]��������߂�
				boneMat *= DirectX::XMMatrixTranslation(bonePos.x, bonePos.y, bonePos.z);

#pragma region �e�̏���


				DirectX::XMFLOAT3 boneAngleImpact = { 0.0f,0.0f,0.0f };
				DirectX::XMFLOAT3 boneScaleImpact = { 1.0f,1.0f,1.0f };
				DirectX::XMFLOAT3 boneMoveVectorImpact = { 0.0f,0.0f,0.0f };
				boneAngleImpact = parentBoneDatas[j].angleImpact;
				boneScaleImpact = parentBoneDatas[j].scaleImpact;
				boneMoveVectorImpact = parentBoneDatas[j].moveVectorImpact;

				//�e�̃{�[���ԍ����
				parentBoneNum = parentBoneDatas[j].parentBoneNum;

				//������g���ĉe���x�Ƃ����Z���Ă�
				//�q�@���@�e�̏��œ����
				std::vector<int>parentNums;
				parentNums.reserve(50);

				//������͐e�@���@�q
				std::vector<DirectX::XMMATRIX>parentMat;
				parentMat.reserve(50);


				//��ɂ��ׂĂ̐e���擾&�e���x�擾
				while (1)
				{
					//-1��������(�e���Z�b�g����ĂȂ�������)������
					if (parentBoneNum == -1)break;
					parentNums.push_back(parentBoneNum);

					//�e�̃{�[���ԍ����
					parentBoneNum = parentBoneDatas[parentBoneNum].parentBoneNum;

				}
				if (parentNums.size() != 0)
				{

					DirectX::XMMATRIX mulMat = DirectX::XMMatrixIdentity();

					//�e��
					const int maxParentSize = static_cast<int>(parentNums.size());

					DirectX::XMFLOAT3 pAngle = { 0,0,0 };
					DirectX::XMFLOAT3 pScale = { 1,1,1 };
					DirectX::XMFLOAT3 pMoveVector = { 0,0,0 };
					DirectX::XMFLOAT3 pPos = { 0,0,0 };

					DirectX::XMFLOAT3 pAngleImpact = { 1,1,1 };
					DirectX::XMFLOAT3 pScaleImpact = { 1,1,1 };
					DirectX::XMFLOAT3 pMoveVectorImpact = { 1,1,1 };

					mulMat = DirectX::XMMatrixIdentity();

					//�Ō�ɂ���e�̃{�[������ɉ񂷂̂ŁA�����
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


					//�{�[�����璸�_�̋������ړ�
					mulMat *= DirectX::XMMatrixTranslation(-pPos.x, -pPos.y, -pPos.z);

					mulMat *= DirectX::XMMatrixScaling(pScale.x, pScale.y, pScale.z);

					mulMat *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(pAngle.z));
					mulMat *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(pAngle.x));
					mulMat *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(pAngle.y));


					//�e�͂܂Ƃ߂Ċ|���Ă���̂ŁA���f���Ǝ����̊g�k�����|����΂���
					//modelConstDatas[i][0].scale �� boneScale�|����K�v���邩�v�m�F
					mulMat *= DirectX::XMMatrixTranslation
					(
						pMoveVector.x / (modelConstDatas[i].scale.x * boneScale.x),
						pMoveVector.y / (modelConstDatas[i].scale.y * boneScale.y),
						pMoveVector.z / (modelConstDatas[i].scale.z * boneScale.z)
					);

					//��]��������߂�
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
				//�ϊ�
				DirectX::XMMATRIX matCurrentPose;
				FbxAMatrix fbxCurrentPose =
					bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(fbxAnimationData.currentTime);
				FbxLoader::GetInstance()->ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);

				//��Z
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



	//���f�����L�o�b�t�@�Z�b�g
	if (modelConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
		cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, modelConstBuffer[0]->GetGPUVirtualAddress());

	//���[�U�[���f���o�b�t�@�Z�b�g
	if (userConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
		cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, userConstBuffer[0]->GetGPUVirtualAddress());



	const std::vector<VertexBufferSet>& vertexBufferSets = pModelData->GetVertexBufferSet();
	const std::vector<IndexBufferSet>& indexBufferSets = pModelData->GetIndexBufferSet();


	//���f���̃I�u�W�F�N�g���Ƃ��Z�b�g
	//���_�o�b�t�@�����[�v
	for (int i = 0; i < modelFileObjectNum; i++)
	{
		//cmdLists[0]->SetPipelineState(pPipeline[i]->GetPipelineState().Get());
		cmdLists[0]->SetPipelineState(materials[i]->GetPPipelineState()->GetPipelineState().Get());


		cmdLists[0]->IASetVertexBuffers(0, 1, &vertexBufferSets[i].vertexBufferView);
		cmdLists[0]->IASetIndexBuffer(&indexBufferSets[i].indexBufferView);

#pragma region �e�N�X�`��

		ID3D12DescriptorHeap* textureDescHeap = materials[i]->GetPTextureHeap();
		std::vector<ID3D12DescriptorHeap*> ppHeaps;
		ppHeaps.push_back(textureDescHeap);
		cmdLists[0]->SetDescriptorHeaps(1, &ppHeaps[0]);


		D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
		(
			materials[i]->GetPTextureHeap()->GetGPUDescriptorHandleForHeapStart(),
			i,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);
		cmdLists[0]->SetGraphicsRootDescriptorTable(TEXURE_ROOTPARAM_NUM, gpuDescHandle);
#pragma endregion

#pragma region �萔



		//�萔�o�b�t�@
		cmdLists[0]->SetGraphicsRootConstantBufferView(CONST_BUFFER_REGISTER, constBuffer[i]->GetGPUVirtualAddress());

		//�}�e���A���o�b�t�@
		cmdLists[0]->SetGraphicsRootConstantBufferView
		(MATERIAL_BUFFER_REGISTER, materials[i]->GetPConstBuffer(Material::MaterialConstBufferType::MATERIAL_DATA)->GetGPUVirtualAddress());

		//Color�}�e���A��
		cmdLists[0]->SetGraphicsRootConstantBufferView
		(COLOR_MATERIAL_BUFFER_REGISTER, materials[i]->GetPConstBuffer(Material::MaterialConstBufferType::COLOR)->GetGPUVirtualAddress());

		//���f���o�b�t�@
		if (modelConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
			cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, modelConstBuffer[i]->GetGPUVirtualAddress());

		//���[�U�[�萔�o�b�t�@
		if (userConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
			cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, userConstBuffer[i]->GetGPUVirtualAddress());


#pragma endregion


		//�`��
		cmdLists[0]->DrawIndexedInstanced(static_cast<UINT>(pModelData->GetIndices()[i].size()), 1, 0, 0, 0);

	}

}

void ModelObject::Draw(const std::string& rtName)
{
	RenderTarget::ChangeCurrentRenderTarget(RenderTarget::Get(rtName));

	FbxAnimation();

	DrawCommonProcessing(rtName);
}

void MelLib::ModelObject::MeshCat(const PlaneData& plane, ModelData*& pFront, ModelData*& pBack)
{
	//�ً}(�����D��x���߂̌��݂̖��_)
	//1.���������l�p�`�ł��a����ɂ���Ă�5�p�`�ɂȂ�̂ŁA�ǂ����݂��ʂ̍Č`���͕K�v
	//�����́A�a��ꂽ�ӂ̎�����(���Α�����Ȃ���)�̒��_�ƏՓ˓_�ő��p�`�̎O�p�`�����s��
	//�a��ꂽ�ӂɊւ�镔�������s���΂悢

	//��邱��
	//1.���������ӂ�������Ȃ��A�S���_�ɑ΂��ĕ\��������s���B
	//(���Α��̒��_�����ׂď����Ȃ��Ƃ����Ȃ�����)

	//2.�f�ʂɎg�p����Փ˓_�Ɠ���ւ��钸�_���ǂ����̔��f�B
	//�g�����g��Ȃ��������킩��΂����B
	//�����́A�|�[�g�t�H���I�ɏ����Ă���
	//���f�ł�����A�z��̉��Ԗڂ��Ƃ��̐��l���擾���Ƃ��āA�f�ʂ̌`�����Ɏ擾���ăC���f�b�N�X�Ƃ��Ďg����悤�ɂ��Ƃ�
	//��������ւ����ɏ�����΂��āA�ォ��ǉ����Ă�����

	//3.������΂�����A���_�����ł͂Ȃ��C���f�b�N�X���ύX����B
	//���Α��̒��_���֌W���Ă�C���f�b�N�X�̃Z�b�g(�O�p�`���`������Y����3��)��������΂��B
	//���ʂ��܂����ł镔���͏�������_���B

	//���݂̖��_
	//1.�f�ʂ��`������̂Ɏg�����_�Ƃ�������Ȃ����_�����邪�A�f�ʂ��`������̂Ɏg�����_���ǂ����o�����B
	//�@�������̒��_�̂���g���܂킻���Ǝv�������A�f�ʂ̂悤�ɖʂ��`�����ĂȂ��\��������̂Ŗ����������B
	//(�l�p�`�͒f�ʂƓ����C���f�b�N�X�̖ʂ������炠�邩��ł���)
	//���������f�ʂ̂悤�ɖʂ��`�����ĂȂ��ꍇ�́A���̒��_��ǉ����Ȃ��Ƃ����Ȃ�
	//���������ؒf���ĂȂ��Ȃ钸�_�����ւ����ɏ���?
	//���ǒf�ʂ��`������̂Ɏg�����g��Ȃ����𔻒f���Ȃ��Ƃ����Ȃ��B
	//���ǎO�p�`���a��Ƃ��Ɠ����悤�Ɍ����炠��ʂ��`�����Ȃ����Ȃ��Ƃ����Ȃ�?

	//2,�a��������3�ȏ㕪�����ꍇ�͂ǂ����邩(w�̐^�񒆎a�������Ȃ�5�ɕ������)


	//�v����������
	//1.3�p�`���a�������ɖʂ������邩��A����ւ���΂ł���?
	//���ǎl�p�`�ɕς��Ȃ��Ƃ����Ȃ����疳���B
	//���p�`���O�p�`�̏W���̂ɂ����Ŗʂ��`�����Ȃ����΂ł���?

	//���̏�����Box��Board�ōs��

	//�Ƃ肠���������蔻�菈����������΂��āA
	//�C���f�b�N�X�̃Z�b�g�ƒf�ʂ̕`����s��(�f�ʗp�̃e�N�X�`���͊��蓖�ĂȂ�)


	//�X�P�[�������ł͂Ȃ��A��]�ƕ��s�ړ����l�����Ȃ��Ƃ����Ȃ�
	//���s�ړ��͖��Ȃ��B��]�́A���ʂ���]������΂���

	//���_�ɗ��\�̏�����������?
	//�V�F�[�_�[�ŕ�������?

	//�V�F�[�_�[�ŕ������āA�X�g���[���o�͂ŁA�C���f�b�N�X���󂯎���Ώ����̖��͉����B
	//�󂯎�ꂻ���ȋC�͂��邯�ǁB�������Ȃ��ƕ������Ă������Ȃ���


	//�Փ˓_�����߂�
	//���̈ʒu�����ɁA�V�F�[�_�[�œ_�𐶐��B
	//�X�g���[���ŕ����������_���o��
	//�o�͂������_�̕\������
	//2�̃o�b�t�@�ɕ�����

	//��������O�p�`���ǂ������ʂ��Ȃ��Ƃ����Ȃ����߁A
	//���ǒ��_�ɏ���ǉ����Ȃ��Ƃ����Ȃ�?
	//����Ƃ��A�V�F�[�_�[�ŋ��߂���?

	//�X�g���[���o�͂��ƃo�b�t�@�ɏ������ނ���A���_�f�[�^�擾�Ƒ��삪�ł��Ȃ�?����Ƃ��e�N�X�`���o�b�t�@�݂����ɓǂݎ���?

	//�W�I���g���V�F�[�_�[�ł�鏈����������Ŏ�������Ίy�ɕ����ł���?


	//��ɎO�p�`���Ƃɕ����ď��������ق������₷������
	//�^�v���ɁA�C���f�b�N�X���Q�l��Value3�Œ��_�A�C���f�b�N�X�̓Y����(�C���f�b�N�X�����������ɃA�N�Z�X����p)


	//�ŏ��̒i�K�ŁA�ׂ����߂��ق�������
	//�����̓_(���X�̎O�p�`�̓_)�ƏՓ˓_�ŋ��������߁A���̏������ɁA���������߂�

	//����ł�����������A�ӂ��ƂɏՓ˓_�i�[���āA
	//�O�p�`�̍��W�ǂ������A���̍��W���g���Ă�ӂ̏Փ˓_�A������̏Փ˓_�A�c��̍��W�Ƃ����������g���΂悢


	//�O�p�`�����ɒ��_�����邩�Ȃ����̊m�F�������Ƃ��Ă�



	// ���ʏ��(��]�K���̂��߁A��蒼��)
	PlaneData rotPlane;
	rotPlane.SetPosition(plane.GetPosition());
	rotPlane.SetNormal(LibMath::RotateZXYVector3(plane.GetNormal(),
		DirectX::XMFLOAT3(-modelConstDatas[0].angle.x, -modelConstDatas[0].angle.y, -modelConstDatas[0].angle.z)));

	//���f���̒��_���O�p�`���Ƃɂ܂Ƃ߂�����
	struct ModelTri
	{
		//���_
		Value3<FbxVertex> vertData;

		//�\�����茋��
		Value3<char>vertFB;

		//�O�p�`�̕ӂ̏��
		Value3<Segment3DData>segmentData;

		//���ʂƂ̏Փ˔���
		Value3<bool>hitResult;

		//�Փ˓_�̒��_���
		Value3<FbxVertex>hitPosVert;

		//�C���f�b�N�X�̔z��̃C���f�b�N�X
		Value3<int> indicesIndex = 0;
	};


	std::vector<std::vector<FbxVertex>>vertPos = pModelData->GetVertices();
	std::vector<std::vector<USHORT>>indices = pModelData->GetIndices();

	std::vector<ModelTri>modelTri(indices[0].size() / 3);

	//�O�p�`���ƂɃf�[�^�i�[
	for (int i = 0, size = indices[0].size(); i < size; i += 3)
	{
		int triIndex = 0;
		if (i == 0)triIndex = 0;
		else triIndex = i / 3;

		//���_���
		modelTri[triIndex].vertData.v1 = vertPos[0][indices[0][i]];
		modelTri[triIndex].vertData.v2 = vertPos[0][indices[0][i + 1]];
		modelTri[triIndex].vertData.v3 = vertPos[0][indices[0][i + 2]];

		//�g�k�A���s�ړ��K��
		//���̂܂܂��ƃ��f���f�[�^�̒��_���{������������ԂɂȂ�̂ŁA����
		modelTri[triIndex].vertData.v1.pos.x *= modelConstDatas[0].scale.x;
		modelTri[triIndex].vertData.v1.pos.y *= modelConstDatas[0].scale.y;
		modelTri[triIndex].vertData.v1.pos.z *= modelConstDatas[0].scale.z;
		modelTri[triIndex].vertData.v2.pos.x *= modelConstDatas[0].scale.x;
		modelTri[triIndex].vertData.v2.pos.y *= modelConstDatas[0].scale.y;
		modelTri[triIndex].vertData.v2.pos.z *= modelConstDatas[0].scale.z;
		modelTri[triIndex].vertData.v3.pos.x *= modelConstDatas[0].scale.x;
		modelTri[triIndex].vertData.v3.pos.y *= modelConstDatas[0].scale.y;
		modelTri[triIndex].vertData.v3.pos.z *= modelConstDatas[0].scale.z;

		modelTri[triIndex].vertData.v1.pos.x += modelConstDatas[0].position.x;
		modelTri[triIndex].vertData.v1.pos.y += modelConstDatas[0].position.y;
		modelTri[triIndex].vertData.v1.pos.z += modelConstDatas[0].position.z;
		modelTri[triIndex].vertData.v2.pos.x += modelConstDatas[0].position.x;
		modelTri[triIndex].vertData.v2.pos.y += modelConstDatas[0].position.y;
		modelTri[triIndex].vertData.v2.pos.z += modelConstDatas[0].position.z;
		modelTri[triIndex].vertData.v3.pos.x += modelConstDatas[0].position.x;
		modelTri[triIndex].vertData.v3.pos.y += modelConstDatas[0].position.y;
		modelTri[triIndex].vertData.v3.pos.z += modelConstDatas[0].position.z;

		//�ӏ��
		modelTri[triIndex].segmentData.v1.SetPosition
		(Value2<Vector3>(modelTri[triIndex].vertData.v1.pos, modelTri[triIndex].vertData.v2.pos));
		modelTri[triIndex].segmentData.v2.SetPosition
		(Value2<Vector3>(modelTri[triIndex].vertData.v2.pos, modelTri[triIndex].vertData.v3.pos));
		modelTri[triIndex].segmentData.v3.SetPosition
		(Value2<Vector3>(modelTri[triIndex].vertData.v3.pos, modelTri[triIndex].vertData.v1.pos));

		//�C���f�b�N�X�̔z��̃C���f�b�N�X
		modelTri[triIndex].indicesIndex.v1 = i;
		modelTri[triIndex].indicesIndex.v2 = i + 1;
		modelTri[triIndex].indicesIndex.v3 = i + 2;

		//���\���茋��
		modelTri[triIndex].vertFB.v1 = LibMath::PointPlaneFrontBackCheck(modelTri[triIndex].vertData.v1.pos, rotPlane);
		modelTri[triIndex].vertFB.v2 = LibMath::PointPlaneFrontBackCheck(modelTri[triIndex].vertData.v2.pos, rotPlane);
		modelTri[triIndex].vertFB.v3 = LibMath::PointPlaneFrontBackCheck(modelTri[triIndex].vertData.v3.pos, rotPlane);

		//�ӂ̏Փˊm�F�A�Փ˓_�擾
		Segment3DCalcResult res;
		modelTri[triIndex].hitResult.v1
			= Collision::PlaneAndSegment3D(rotPlane, modelTri[triIndex].segmentData.v1, &res);
		modelTri[triIndex].hitPosVert.v1.pos = res.planeHitPos.ToXMFLOAT3();
		modelTri[triIndex].hitPosVert.v1.normal = modelTri[triIndex].vertData.v1.normal;
		//modelTri[triIndex].hitPosVert.v1.uv = modelTri[triIndex].vertData.v1.normal;

		modelTri[triIndex].hitResult.v2
			= Collision::PlaneAndSegment3D(rotPlane, modelTri[triIndex].segmentData.v2, &res);
		modelTri[triIndex].hitPosVert.v2.pos = res.planeHitPos.ToXMFLOAT3();
		modelTri[triIndex].hitPosVert.v2.normal = modelTri[triIndex].vertData.v2.normal;
		//modelTri[triIndex].hitPosVert.v2.uv = modelTri[triIndex].vertData.v2.normal;

		modelTri[triIndex].hitResult.v3
			= Collision::PlaneAndSegment3D(rotPlane, modelTri[triIndex].segmentData.v3, &res);
		modelTri[triIndex].hitPosVert.v3.pos = res.planeHitPos.ToXMFLOAT3();
		modelTri[triIndex].hitPosVert.v3.normal = modelTri[triIndex].vertData.v3.normal;
		//modelTri[triIndex].hitPosVert.v3.uv = modelTri[triIndex].vertData.v3.normal;
	}

	//�S���_���܂Ƃ߂�z��B�\�Ɨ��ŕ�����
	std::vector<FbxVertex>frontVertices;
	std::vector<FbxVertex>backVertices;
	std::vector<USHORT>frontIndices;
	std::vector<USHORT>backIndices;

	//�O�p�`�����Ă����āA�i�[���Ă���
	USHORT frontIndex = 0;
	USHORT backIndex = 0;
	for (const auto& tri : modelTri)
	{
		//�ꎞ�I�Ɋi�[���邽�߂̔z��
		std::vector<FbxVertex>fVert;
		std::vector<FbxVertex>bVert;


		//�Е��ɕ΂��Ă���(�a���ĂȂ�������)�C���f�b�N�X��ʏ�ʂ�i�[���Ď���
		if (!tri.hitResult.v1 && !tri.hitResult.v2 && !tri.hitResult.v3)
		{
			if (tri.vertFB.v1 == 1)
			{
				frontVertices.push_back(tri.vertData.v1);
				frontVertices.push_back(tri.vertData.v2);
				frontVertices.push_back(tri.vertData.v3);

				frontIndices.push_back(frontIndex);
				frontIndices.push_back(frontIndex + 1);
				frontIndices.push_back(frontIndex + 2);

				frontIndex += 3;
			}
			else
			{
				backVertices.push_back(tri.vertData.v1);
				backVertices.push_back(tri.vertData.v2);
				backVertices.push_back(tri.vertData.v3);

				backIndices.push_back(backIndex);
				backIndices.push_back(backIndex + 1);
				backIndices.push_back(backIndex + 2);

				backIndex += 3;
			}
			continue;
		}


		//�΂��ĂȂ�������A�\�����ƂɊi�[

		//�������߂�

		
		//���_�̃C���f�b�N�X(frontInd�AbackInd�Ɋi�[����l)
		std::vector<USHORT>fVertInd;
		std::vector<USHORT>bVertInd;


		//�O�p�`�̓_���ǂ���ɕ΂��Ă邩�𒲂ׂ�
		if (tri.vertFB.v1 == 1)
		{
			fVert.push_back(tri.vertData.v1);
		}
		else
		{
			bVert.push_back(tri.vertData.v1);
		}
		if (tri.vertFB.v2 == 1)
		{
			fVert.push_back(tri.vertData.v2);
		}
		else
		{
			bVert.push_back(tri.vertData.v2);
		}
		if (tri.vertFB.v3 == 1)
		{
			fVert.push_back(tri.vertData.v3);
		}
		else
		{
			bVert.push_back(tri.vertData.v3);
			
		}
		
		//�Е��̏ꍇ(���p�`�̖ʌ`������Ȃ��ꍇ)
		if(fVert.size() == 1)
		{
			frontVertices.push_back(fVert[0]);

			if (tri.hitResult.v1)
			{
				frontVertices.push_back(tri.hitPosVert.v1);
				fVert.push_back(tri.hitPosVert.v1);
			}
			if (tri.hitResult.v2)
			{
				frontVertices.push_back(tri.hitPosVert.v2);
				fVert.push_back(tri.hitPosVert.v2);
			}
			if (tri.hitResult.v3)
			{
				frontVertices.push_back(tri.hitPosVert.v3);
				fVert.push_back(tri.hitPosVert.v3);
			}

			Vector3 cross;
			cross = LibMath::CalcNormal(fVert[0].pos, fVert[1].pos, fVert[2].pos);
			if (Vector3(tri.vertData.v1.normal) == cross)
			{
				frontIndices.push_back(frontIndex);
				frontIndices.push_back(frontIndex + 1);
				frontIndices.push_back(frontIndex + 2);
			}
			else//�@�����t��������A���т��t�ɂ��Ċi�[
			{
				frontIndices.push_back(frontIndex + 2);
				frontIndices.push_back(frontIndex + 1);
				frontIndices.push_back(frontIndex);
			}

			frontIndex += 3;
		}
		else
		{
			frontVertices.push_back(fVert[0]);

			int fVertVNum = 0;
			//fVert[0]���܂܂�ĂĂ����������ʂɓ������Ă���Փ˓_���i�[
			if (tri.hitResult.v1) 
			{
				if (tri.segmentData.v1.GetPosition().v1 == fVert[0].pos
					|| tri.segmentData.v1.GetPosition().v2 == fVert[0].pos)
				{
					//fVert0Seg = 
					fVertVNum = 1;
					frontVertices.push_back(tri.hitPosVert.v1);
				}
			}
			 if (tri.hitResult.v2)
			{
				if (tri.segmentData.v2.GetPosition().v1 == fVert[0].pos
					|| tri.segmentData.v2.GetPosition().v2 == fVert[0].pos)
				{
					fVertVNum = 2;
					frontVertices.push_back(tri.hitPosVert.v2);
				}
			}
			 if (tri.hitResult.v3)
			{
				if (tri.segmentData.v3.GetPosition().v1 == fVert[0].pos
					|| tri.segmentData.v3.GetPosition().v2 == fVert[0].pos)
				{
					fVertVNum = 3;
					frontVertices.push_back(tri.hitPosVert.v3);
				}
			}

			//������̏Փ˓_�Ǝc��̒��_���i�[
			if (tri.hitResult.v1 && fVertVNum != 1)
			{
				frontVertices.push_back(tri.hitPosVert.v1);
			}
			else if (tri.hitResult.v2 && fVertVNum != 2)
			{
				frontVertices.push_back(tri.hitPosVert.v2);
			}
			else if (tri.hitResult.v3 && fVertVNum != 3)
			{
				frontVertices.push_back(tri.hitPosVert.v3);
			}

			frontVertices.push_back(fVert[1]);



			//�O�p�`���`��
			
			//fVert���N���A���āA����ǉ������O�p�`��ǉ�
			fVert.clear();
			fVert.resize(4);
			for (int i = frontIndex; i < frontIndex + 4; i++)
			{
				fVert[i - frontIndex] = frontVertices[i];
			}
			 
			//n���p�`�̎O�p�`�����𗘗p���ăC���f�b�N�X�����߂�
			//���_�����ԉ������_
			FbxVertex farVertex;

			//��ԉ������_�̍��W
			float farVertDis = 0.0f;

			//farVertex�̓Y����(�`����̍폜�p)
			int farVertIndex = 0;

			std::vector<int>skipVertIndices;
			//�\
			while (1)
			{
				farVertDis = 0.0f;
				//�O�p�`�����ɒ��_�����邩�Ȃ����̊m�F�������Ƃ��Ă�

				//��ԉ������_�����߂�
				for (int i = 0, size = fVert.size(); i < size; i++)
				{
					for (const auto& ind : skipVertIndices)
					{
						if (i == ind)i++;
					}


					float dis = Vector3(fVert[i].pos).Length();
					if (farVertDis <= dis)
					{
						farVertex = fVert[i];
						farVertDis = dis;
						farVertIndex = i;
					}
				}

				//10/11 �������������Ȃ��Ƃ����Ȃ�(��������Ȃ��Ĕz��̃C���f�b�N�X���狁�߂�)
				//�ׂ̒��_�����߂�
				int farAddIndex = farVertIndex + 1;
			/*	if (farAddIndex >= fVert.size())farAddIndex = 0;
				for (const auto& ind : skipVertIndices)
				{
					if (ind == farAddIndex)
					{
						farAddIndex++;
						break;
					}
				}*/

				if (farAddIndex >= frontVertices.size())farAddIndex = 0;

				bool whileEnd = false;
				while (!whileEnd && skipVertIndices.size() != 0)
				{
					for (const auto& ind : skipVertIndices)
					{
						if (ind == farAddIndex || farAddIndex == farVertIndex)
						{
							farAddIndex++;
							if (farAddIndex >= frontVertices.size())farAddIndex = 0;
						}
						else
						{
							whileEnd = true;
						}
					}
				}





				int farSubIndex = farVertIndex - 1;
			/*	if (farSubIndex <= 0)farSubIndex = fVert.size() - 1;
				for (const auto& ind : skipVertIndices)
				{
					if (ind == farSubIndex)
					{
						farSubIndex--;
						break;
					}
				}*/
				if (farSubIndex <= 0)farSubIndex = frontVertices.size() - 1;
				whileEnd = false;
				while (!whileEnd && skipVertIndices.size() != 0)
				{
					for (const auto& ind : skipVertIndices)
					{
						if (ind == farSubIndex || farSubIndex == farVertIndex || farSubIndex == farAddIndex)
						{
							farSubIndex--;
							if (farSubIndex <= 0)farSubIndex = frontVertices.size() - 1;
						}
						else
						{
							whileEnd = true;
						}
					}
				}

				


				//�O�p�`���`��

				//���߂��O�ςƁA�ʂ̊O�ς���v������v�Z�I��
				//�O�p�`�̊O��
				Vector3 cross;

				//sub,far,add
				cross = LibMath::CalcNormal(frontVertices[farSubIndex].pos,farVertex.pos, frontVertices[farAddIndex].pos);
				if (Vector3(farVertex.normal) == cross)
				{
					frontIndices.push_back(frontIndex + farSubIndex);
					frontIndices.push_back(frontIndex + farVertIndex);
					frontIndices.push_back(frontIndex + farAddIndex);
				}
				else//�@�����t��������A�t�ɂ��Ċi�[ 2,1,far
				{
					frontIndices.push_back(frontIndex + farAddIndex);
					frontIndices.push_back(frontIndex + farVertIndex);
					frontIndices.push_back(frontIndex + farSubIndex);
				}

				//���_�����ԉ����_���폜
				fVert.erase(fVert.begin() + farVertIndex);
				skipVertIndices.push_back(farVertIndex);

				//�`���s�ɂȂ�����I��
				if (fVert.size() == 2)break;

			}
			frontIndex += 6;

		}

		if (bVert.size() == 1)
		{
			backVertices.push_back(bVert[0]);

			if (tri.hitResult.v1)
			{
				backVertices.push_back(tri.hitPosVert.v1);
				bVert.push_back(tri.hitPosVert.v1);
			}
			if (tri.hitResult.v2)
			{
				backVertices.push_back(tri.hitPosVert.v2);
				bVert.push_back(tri.hitPosVert.v2);
			}
			if (tri.hitResult.v3)
			{
				backVertices.push_back(tri.hitPosVert.v3);
				bVert.push_back(tri.hitPosVert.v3);
			}

			Vector3 cross;
			cross = LibMath::CalcNormal(bVert[0].pos, bVert[1].pos, bVert[2].pos);
			if (Vector3(tri.vertData.v1.normal) == cross)
			{
				backIndices.push_back(backIndex);
				backIndices.push_back(backIndex + 1);
				backIndices.push_back(backIndex + 2);
			}
			else//�@�����t��������A���т��t�ɂ��Ċi�[
			{
				backIndices.push_back(backIndex + 2);
				backIndices.push_back(backIndex + 1);
				backIndices.push_back(backIndex);
			}

			backIndex += 3;
		}
		else
		{
			backVertices.push_back(bVert[0]);

			int bVertVNum = 0;
			//bVert[0]���܂܂�ĂĂ����������ʂɓ������Ă���Փ˓_���i�[
			if (tri.hitResult.v1)
			{
				if (tri.segmentData.v1.GetPosition().v1 == bVert[0].pos
					|| tri.segmentData.v1.GetPosition().v2 == bVert[0].pos)
				{
					//bVert0Seg = 
					bVertVNum = 1;
					backVertices.push_back(tri.hitPosVert.v1);
				}
			}
			 if (tri.hitResult.v2)
			{
				if (tri.segmentData.v2.GetPosition().v1 == bVert[0].pos
					|| tri.segmentData.v2.GetPosition().v2 == bVert[0].pos)
				{
					bVertVNum = 2;
					backVertices.push_back(tri.hitPosVert.v2);
				}
			}
			 if (tri.hitResult.v3)
			{
				if (tri.segmentData.v3.GetPosition().v1 == bVert[0].pos
					|| tri.segmentData.v3.GetPosition().v2 == bVert[0].pos)
				{
					bVertVNum = 3;
					backVertices.push_back(tri.hitPosVert.v3);
				}
			}

			//������̏Փ˓_�Ǝc��̒��_���i�[
			if (tri.hitResult.v1 && bVertVNum != 1)
			{
				backVertices.push_back(tri.hitPosVert.v1);
			}
			else if (tri.hitResult.v2 && bVertVNum != 2)
			{
				backVertices.push_back(tri.hitPosVert.v2);
			}
			else if (tri.hitResult.v3 && bVertVNum != 3)
			{
				backVertices.push_back(tri.hitPosVert.v3);
			}

			backVertices.push_back(bVert[1]);



			//�O�p�`���`��

			//bVert���N���A���āA����ǉ������O�p�`��ǉ�
			bVert.clear();
			bVert.resize(4);
			for (int i = backIndex; i < backIndex + 4; i++)
			{
				bVert[i - backIndex] = backVertices[i];
			}

			//n���p�`�̎O�p�`�����𗘗p���ăC���f�b�N�X�����߂�
			//���_�����ԉ������_
			FbxVertex farVertex;

			//��ԉ������_�̍��W
			float farVertDis = 0.0f;

			//farVertex�̓Y����(�`����̍폜�p)
			int farVertIndex = 0;

			std::vector<int>skipVertIndices;
			//��
			while (1)
			{
				farVertDis = 0.0f;
				//�O�p�`�����ɒ��_�����邩�Ȃ����̊m�F�������Ƃ��Ă�

				//��ԉ������_�����߂�
				for (int i = 0, size = bVert.size(); i < size; i++)
				{
					for(const auto& ind : skipVertIndices)
					{
						if (i == ind)i++;
					}

					float dis = Vector3(bVert[i].pos).Length();
					if (farVertDis <= dis)
					{
						farVertex = bVert[i];
						farVertDis = dis;
						farVertIndex = i;

					}
				}

				//10/11 �������������Ȃ��Ƃ����Ȃ�(��������Ȃ��Ĕz��̃C���f�b�N�X���狁�߂�)
				//�ׂ̒��_�����߂�
				int farAddIndex = farVertIndex + 1;
				if (farAddIndex >= backVertices.size())farAddIndex = 0;

				/*for (const auto& ind : skipVertIndices)
				{
					if (ind == farAddIndex)
					{
						farAddIndex++;

						if (farAddIndex == farVertIndex)farAddIndex++;
					}
				}*/

				bool whileEnd = false;
				while(!whileEnd && skipVertIndices.size() != 0)
				{
					for (const auto& ind : skipVertIndices)
					{
						if (ind == farAddIndex || farAddIndex == farVertIndex)
						{
							farAddIndex++;
							if (farAddIndex >= backVertices.size())farAddIndex = 0;
						}
						else
						{
							whileEnd = true;
						}
					}
				}



				int farSubIndex = farVertIndex - 1;
				if (farSubIndex <= 0)farSubIndex = backVertices.size() - 1;
				/*for (const auto& ind : skipVertIndices)
				{
					if (ind == farSubIndex)
					{
						farSubIndex--;

						if (farSubIndex == farVertIndex || farSubIndex == farAddIndex)farSubIndex--;
					}
				}*/
				whileEnd = false;
				while (!whileEnd && skipVertIndices.size() != 0)
				{
					for (const auto& ind : skipVertIndices)
					{
						if (ind == farSubIndex || farSubIndex == farVertIndex || farSubIndex == farAddIndex)
						{
							farSubIndex--;
							if (farSubIndex <= 0)farSubIndex = backVertices.size() - 1;
						}
						else
						{
							whileEnd = true;
						}
					}
				}


				//�O�p�`���`��

				//���߂��O�ςƁA�ʂ̊O�ς���v������v�Z�I��
				//�O�p�`�̊O��
				Vector3 cross;

				//sub,far,add
				cross = LibMath::CalcNormal(backVertices[farSubIndex].pos, farVertex.pos, backVertices[farAddIndex].pos);
				if (Vector3(farVertex.normal) == cross)
				{
					backIndices.push_back(backIndex + farSubIndex);
					backIndices.push_back(backIndex + farVertIndex);
					backIndices.push_back(backIndex + farAddIndex);
				}
				else//�@�����t��������A�t�ɂ��Ċi�[ 2,1,far
				{
					backIndices.push_back(backIndex + farAddIndex);
					backIndices.push_back(backIndex + farVertIndex);
					backIndices.push_back(backIndex + farSubIndex);
				}

				//���_�����ԉ����_���폜
				bVert.erase(bVert.begin() + farVertIndex);
				skipVertIndices.push_back(farVertIndex);


				//�`���s�ɂȂ�����I��
				if (bVert.size() == 2)break;

			}
			backIndex += 6;

		}


#pragma region ��



		//���ʂƕӂ��������Ă�����Փ˓_���i�[
		//if (tri.hitResult.v1)
		//{
		//	frontVertices.push_back(tri.hitPosVert.v1);
		//	fVert.push_back(tri.hitPosVert.v1);
		//	fVertInd.push_back(frontIndex);
		//	frontIndex++;

		//	backVertices.push_back(tri.hitPosVert.v1);
		//	bVert.push_back(tri.hitPosVert.v1);
		//	bVertInd.push_back(backIndex);
		//	backIndex++;

		//}
		//if (tri.hitResult.v2)
		//{
		//	frontVertices.push_back(tri.hitPosVert.v2);
		//	fVert.push_back(tri.hitPosVert.v2);
		//	fVertInd.push_back(frontIndex);
		//	frontIndex++;

		//	backVertices.push_back(tri.hitPosVert.v2);
		//	bVert.push_back(tri.hitPosVert.v2);
		//	bVertInd.push_back(backIndex);
		//	backIndex++;
		//}
		//if (tri.hitResult.v3)
		//{
		//	frontVertices.push_back(tri.hitPosVert.v3);
		//	fVert.push_back(tri.hitPosVert.v3);
		//	fVertInd.push_back(frontIndex);
		//	frontIndex++;

		//	backVertices.push_back(tri.hitPosVert.v3);
		//	bVert.push_back(tri.hitPosVert.v3);
		//	bVertInd.push_back(backIndex);
		//	backIndex++;
		//}

		////�O�p�`�̒��_���i�[������łɁA�Փ˓_���i�[����悤�ɂ���
		//if (tri.vertFB.v1 == 1)
		//{
		//	frontVertices.push_back(tri.vertData.v1);

		//	//tri.vertData.v1���܂܂�Ă邩�A�ؒf
		//}
		//else
		//{
		//	backVertices.push_back(tri.vertData.v1);
		//	bVert.push_back(tri.vertData.v1);

		//	bVertInd.push_back(backIndex);
		//	backIndex += 2;
		//}
		//if (tri.vertFB.v2 == 1)
		//{
		//	frontVertices.push_back(tri.vertData.v2);
		//	fVert.push_back(tri.vertData.v2);

		//	fVertInd.push_back(frontIndex);
		//	frontIndex += 2;
		//}
		//else
		//{
		//	backVertices.push_back(tri.vertData.v2);
		//	bVert.push_back(tri.vertData.v2);

		//	bVertInd.push_back(backIndex);

		//	backIndex += 2;
		//}
		//if (tri.vertFB.v3 == 1)
		//{
		//	frontVertices.push_back(tri.vertData.v3);
		//	fVert.push_back(tri.vertData.v3);

		//	fVertInd.push_back(frontIndex);
		//	frontIndex += 2;
		//}
		//else
		//{
		//	backVertices.push_back(tri.vertData.v3);
		//	bVert.push_back(tri.vertData.v3);

		//	bVertInd.push_back(backIndex);

		//	backIndex += 2;
		//}

#pragma endregion

#pragma region �ύX�O



		//////�O�p�`�̒��_���i�[
		////if (tri.vertFB.v1 == 1)
		////{
		////	frontVertices.push_back(tri.vertData.v1);
		////	fVert.push_back(tri.vertData.v1);

		////	fVertInd.push_back(frontIndex);
		////	frontIndex += 2;
		////}
		////else
		////{
		////	backVertices.push_back(tri.vertData.v1);
		////	bVert.push_back(tri.vertData.v1);

		////	bVertInd.push_back(backIndex);
		////	backIndex+= 2;
		////}
		////if (tri.vertFB.v2 == 1)
		////{
		////	frontVertices.push_back(tri.vertData.v2);
		////	fVert.push_back(tri.vertData.v2);

		////	fVertInd.push_back(frontIndex);
		////	frontIndex += 2;
		////}
		////else
		////{
		////	backVertices.push_back(tri.vertData.v2);
		////	bVert.push_back(tri.vertData.v2);

		////	bVertInd.push_back(backIndex);

		////	backIndex += 2;
		////}
		////if (tri.vertFB.v3 == 1)
		////{
		////	frontVertices.push_back(tri.vertData.v3);
		////	fVert.push_back(tri.vertData.v3); 
		////	
		////	fVertInd.push_back(frontIndex);
		////	frontIndex += 2;
		////}
		////else
		////{
		////	backVertices.push_back(tri.vertData.v3);
		////	bVert.push_back(tri.vertData.v3);

		////	bVertInd.push_back(backIndex);

		////	backIndex += 2;
		////}


		//////���ʂƕӂ��������Ă�����Փ˓_���i�[
		////if (tri.hitResult.v1)
		////{
		////	frontVertices.push_back(tri.hitPosVert.v1);
		////	fVert.push_back(tri.hitPosVert.v1);
		////	fVertInd.push_back(frontIndex);
		////	frontIndex++;

		////	backVertices.push_back(tri.hitPosVert.v1);
		////	bVert.push_back(tri.hitPosVert.v1);
		////	bVertInd.push_back(backIndex);
		////	backIndex++;

		////}
		////if (tri.hitResult.v2)
		////{
		////	frontVertices.push_back(tri.hitPosVert.v2);
		////	fVert.push_back(tri.hitPosVert.v2);
		////	fVertInd.push_back(frontIndex);
		////	frontIndex++;

		////	backVertices.push_back(tri.hitPosVert.v2);
		////	bVert.push_back(tri.hitPosVert.v2);
		////	bVertInd.push_back(backIndex);
		////	backIndex++;
		////}
		////if (tri.hitResult.v3)
		////{
		////	frontVertices.push_back(tri.hitPosVert.v3);
		////	fVert.push_back(tri.hitPosVert.v3);
		////	fVertInd.push_back(frontIndex);
		////	frontIndex++;

		////	backVertices.push_back(tri.hitPosVert.v3);
		////	bVert.push_back(tri.hitPosVert.v3);
		////	bVertInd.push_back(backIndex);
		////	backIndex++;
		////}


		////n���p�`�̎O�p�`�����𗘗p���Čv�Z
		////�\

		////���_�����ԉ������_
		//FbxVertex farVertex;

		////��ԉ������_�̍��W
		//float farVertDis = 0.0f;

		////farVertex�̓Y����(�`����̍폜�p)
		//int farVertIndex = 0;

		////fer����1�A2�Ԗڂɋ߂����_�����߂�ׂ̃f�[�^�i�[�p�ϐ�
		//float farFirDir = FLT_MAX;
		//FbxVertex farFirVertex;
		//int farFirVertIndex = 0;

		//float farSecDir = FLT_MAX;
		//FbxVertex farSecVertex;
		//int farSecVertIndex = 0;

		////�\
		//while (1)
		//{
		//	//�O�p�`�����ɒ��_�����邩�Ȃ����̊m�F�������Ƃ��Ă�

		//	//��ԉ������_�����߂�
		//	for (int i = 0, size = fVert.size(); i < size; i++)
		//	{
		//		float dis = Vector3(fVert[i].pos).Length();
		//		if (farVertDis <= dis)
		//		{
		//			farVertex = fVert[i];
		//			farVertDis = dis;
		//			farVertIndex = i;
		//		}
		//	}

		//	//far����1�A2�Ԗڂɋ߂����_(�ׂ̒��_)�����߂�
		//	for (int i = 0, size = fVert.size(); i < size; i++)
		//	{
		//		//������r�h�~
		//		if (i == farVertIndex)continue;

		//		float dis = Vector3(farVertex.pos - fVert[i].pos).Length();
		//		if (farFirDir >= dis)
		//		{
		//			farSecVertex = farFirVertex;
		//			farSecDir = farFirDir;
		//			farSecVertIndex = farFirVertIndex;

		//			farFirVertex = fVert[i];
		//			farFirDir = dis;
		//			farFirVertIndex = i;
		//		}
		//		else if (farSecDir >= dis)
		//		{
		//			farSecVertex = fVert[i];
		//			farSecDir = dis;
		//			farSecVertIndex = i;
		//		}
		//	}


		//	//�O�p�`���`��

		//	//���߂��O�ςƁA�ʂ̊O�ς���v������v�Z�I��
		//	//�O�p�`�̊O��
		//	Vector3 cross;

		//	//far,1,2
		//	cross = LibMath::CalcNormal(farVertex.pos, farFirVertex.pos, farSecVertex.pos);
		//	if(Vector3(farVertex.normal) == cross)
		//	{
		//		frontIndices.push_back(fVertInd[farVertIndex]);
		//		frontIndices.push_back(fVertInd[farFirVertIndex]);
		//		frontIndices.push_back(fVertInd[farSecVertIndex]);
		//	}
		//	else//�@�����t��������A�t�ɂ��Ċi�[ 2,1,far
		//	{
		//		frontIndices.push_back(fVertInd[farSecVertIndex]);
		//		frontIndices.push_back(fVertInd[farFirVertIndex]);
		//		frontIndices.push_back(fVertInd[farVertIndex]);
		//	}

		//	//���_�����ԉ����_���폜
		//	fVert.erase(fVert.begin() + farVertIndex);

		//	//�`���s�ɂȂ�����I��
		//	if (fVert.size() == 2)break;
		//	
		//}

		////��
		//while(1)
		//{//�O�p�`�����ɒ��_�����邩�Ȃ����̊m�F�������Ƃ��Ă�

		//	//��ԉ������_�����߂�
		//	for (int i = 0, size = bVert.size(); i < size; i++)
		//	{
		//		float dis = Vector3(bVert[i].pos).Length();
		//		if (farVertDis <= dis)
		//		{
		//			farVertex = bVert[i];
		//			farVertDis = dis;
		//			farVertIndex = i;
		//		}
		//	}

		//	//far����1�A2�Ԗڂɋ߂����_(�ׂ̒��_)�����߂�
		//	farFirDir = FLT_MAX;
		//	farSecDir = FLT_MAX;
		//	for (int i = 0, size = bVert.size(); i < size; i++)
		//	{
		//		//������r�h�~
		//		if (i == farVertIndex)continue;

		//		float dis = Vector3(farVertex.pos - bVert[i].pos).Length();
		//		if (farFirDir >= dis)
		//		{
		//			farSecVertex = farFirVertex;
		//			farSecDir = farFirDir;
		//			farSecVertIndex = farFirVertIndex;

		//			farFirVertex = bVert[i];
		//			farFirDir = dis;
		//			farFirVertIndex = i;
		//		}
		//		else if (farSecDir >= dis)
		//		{
		//			farSecVertex = bVert[i];
		//			farSecDir = dis;
		//			farSecVertIndex = i;
		//		}
		//	}


		//	//�O�p�`���`��

		//	//���߂��O�ςƁA�ʂ̊O�ς���v������v�Z�I��
		//	//�O�p�`�̊O��
		//	Vector3 cross;

		//	//far,1,2
		//	cross = LibMath::CalcNormal(farVertex.pos, farFirVertex.pos, farSecVertex.pos);
		//	if (Vector3(farVertex.normal) == cross)
		//	{
		//		backIndices.push_back(bVertInd[farVertIndex]);
		//		backIndices.push_back(bVertInd[farFirVertIndex]);
		//		backIndices.push_back(bVertInd[farSecVertIndex]);
		//	}
		//	else//�@�����t��������A�t�ɂ��Ċi�[ 2,1,far
		//	{
		//		backIndices.push_back(bVertInd[farSecVertIndex]);
		//		backIndices.push_back(bVertInd[farFirVertIndex]);
		//		backIndices.push_back(bVertInd[farVertIndex]);
		//	}

		//	//���_�����ԉ����_���폜
		//	bVert.erase(bVert.begin() + farVertIndex);

		//	//�`���s�ɂȂ�����I��
		//	if (bVert.size() == 2)break;
		//}

#pragma endregion

	}

	//�����ŁA�f�ʂ̏����Z�b�g���Ă���

	if (!pFront && !catFrontModelData)
	{
		//���_�ƃC���f�b�N�X�����Ƀo�b�t�@���쐬&�X�V(Map)
		//�o�b�t�@�쐬��胂�f���f�[�^���쐬���銴���ɂ���?
		std::vector<std::vector<FbxVertex>> vert(1, frontVertices);
		std::vector<std::vector<USHORT>> ind(1, frontIndices);
		catFrontModelData = std::make_unique<ModelData>();
		catFrontModelData->Create(vert, ind);
		pFront = catFrontModelData.get();
	}
	if (!pBack && !catBackModelData)
	{
		std::vector<std::vector<FbxVertex>> vert(1, backVertices);
		std::vector<std::vector<USHORT>> ind(1, backIndices);
		catBackModelData = std::make_unique<ModelData>();
		catBackModelData->Create(vert, ind);
		pBack = catBackModelData.get();
	}


	//// �S���_�̕\������
	//std::vector<std::vector<Vector3>> vertices = pModelData->GetVerticesPosition();
	//size_t size = vertices[0].size();
	//std::vector<char>verticesFB(size);
	//
	//for (int i = 0; i < size; i++)
	//{
	//	verticesFB[i] = LibMath::PointPlaneFrontBackCheck(vertices[0][i] * modelConstDatas[0].scale + modelConstDatas[0].position,planeData);
	//}

	////�S�ӂ��v�Z
	//std::vector<Segment3DData>sDatas;
	//std::vector<bool>sDatasHit;
	//std::vector<Vector3>sDatasHitPos;
	////��̊m��
	//sDatas.reserve(size / 2);

	//std::vector<std::vector<USHORT>>indices = pModelData->GetIndices();
	//size_t iSize = indices.size();
	//for(int i = 0; i < iSize;i+= 3)
	//{
	//	Segment3DData sData;
	//	sData.SetPosition(Value2<Vector3>(vertices[0][indices[0][i]], vertices[0][indices[0][i + 1]]));
	//	sDatas.push_back(sData);

	//	sData.SetPosition(Value2<Vector3>(vertices[0][indices[0][i + 1]], vertices[0][indices[0][i + 2]]));
	//	sDatas.push_back(sData);

	//	sData.SetPosition(Value2<Vector3>(vertices[0][indices[0][i + 2]], vertices[0][indices[0][i]]));
	//	sDatas.push_back(sData);
	//}

	////����
	//size_t sDatasSize = sDatas.size();
	//sDatasHit.resize(sDatasSize);
	//sDatasHitPos.resize(sDatasSize, FLT_MAX);
	//for (int i = 0; i < sDatasSize; i++)
	//{
	//	Segment3DCalcResult result;
	//	sDatasHit[i] = Collision::PlaneAndSegment3D(planeData, sDatas[i], &result);
	//	sDatasHitPos[i] = result.planeHitPos;
	//}


	//�����n���p�`�̎O�p�`�����`���ōs����
	//�ʂ��Ƃɂ����Ǝw�肷���


	//int z = 0;
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
		if (modelConstDatas[i].scale.x == 0.0f)modelConstDatas[i].scale.x = 0.0001f;
		if (modelConstDatas[i].scale.y == 0.0f)modelConstDatas[i].scale.y = 0.0001f;
		if (modelConstDatas[i].scale.z == 0.0f)modelConstDatas[i].scale.z = 0.0001f;
	}
}

void ModelObject::SetAngle(const Vector3& angle)
{
	for (int i = 0; i < modelFileObjectNum; i++)
	{
		modelConstDatas[i].angle = angle.ToXMFLOAT3();
	}
}


MelLib::ModelObject::ModelObject( ModelObject& obj)
{
	Create(obj.pModelData, nullptr);
	modelConstDatas = obj.modelConstDatas;

	if(obj.catFrontModelData)
	{
		catFrontModelData = std::make_unique<ModelData>();
		*catFrontModelData = *obj.catFrontModelData;
	}
	if (obj.catBackModelData)
	{
		catBackModelData = std::make_unique<ModelData>();
		*catBackModelData = *obj.catBackModelData;
	}
}

ModelObject& MelLib::ModelObject::operator=(ModelObject& obj)
{
	return ModelObject(obj);
}

bool ModelObject::Initialize(ID3D12Device* dev, const std::vector<ID3D12GraphicsCommandList*>& cmdList)
{
	device = dev;
	cmdLists = cmdList;


#pragma region �f�B�X�N���v�^�����W_���[�g�p�����[�^�[
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER rootparam[6] = {};

	//�s���F�Ȃ�
	rootparam[CONST_BUFFER_REGISTER].InitAsConstantBufferView(CONST_BUFFER_REGISTER, 0, D3D12_SHADER_VISIBILITY_ALL);
	//���[�U�[�萔
	rootparam[USER_BUFFER_REGISTER].InitAsConstantBufferView(USER_BUFFER_REGISTER, 0, D3D12_SHADER_VISIBILITY_ALL);
	//�}�e���A��
	rootparam[MATERIAL_BUFFER_REGISTER].InitAsConstantBufferView(MATERIAL_BUFFER_REGISTER, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	//PBR�}�e���A��
	rootparam[COLOR_MATERIAL_BUFFER_REGISTER].InitAsConstantBufferView(COLOR_MATERIAL_BUFFER_REGISTER, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	//���f������
	rootparam[MODEL_BUFFER_REGISTER].InitAsConstantBufferView(MODEL_BUFFER_REGISTER, 0, D3D12_SHADER_VISIBILITY_ALL);

	//�e�N�X�`��
	rootparam[TEXURE_ROOTPARAM_NUM].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_PIXEL);

#pragma endregion

#pragma region ���[�g�V�O�l�`��


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




}

void MelLib::ModelObject::SetMaterial(Material* mtl, const UINT index)
{
	if (!mtl)
	{
#ifdef _DEBUG
		OutputDebugString(L"�}�e���A���̃Z�b�g�Ɏ��s���܂����B�}�e���A����nullptr�ł��B\n");
#endif // _DEBUG
		return;
	}
	if (index >= materials.size())
	{
#ifdef _DEBUG
		OutputDebugString(L"�}�e���A���̃Z�b�g�Ɏ��s���܂����Bindex�̒l���傫���ăZ�b�g�ł��܂���B\n");
#endif // _DEBUG
		return;
	}

	materials[index] = mtl;
}



bool ModelObject::Create(ModelData* pModelData, ConstBufferData* userConstBufferData, const std::string& name)
{
	if (!pModelData)
	{
#ifdef _DEBUG

		OutputDebugStringA(name.data());
		OutputDebugStringW(L"�̐����Ɏ��s���܂����B\n");
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

	//�^�C����i�߂�
	fbxAnimationData.currentTime += fbxAnimationData.animationTimes.freamTime * fbxAnimationData.timeMag;

	if (fbxAnimationData.currentTime > fbxAnimationData.animationTimes.endTime)
		fbxAnimationData.currentTime = fbxAnimationData.animationTimes.startTime;
	if (fbxAnimationData.currentTime < fbxAnimationData.animationTimes.startTime)
		fbxAnimationData.currentTime = fbxAnimationData.animationTimes.endTime;
}

bool ModelObject::Create(ModelData* pModelData, ConstBufferData* userConstBufferData)
{
	if (!pModelData)
	{
#ifdef _DEBUG
		OutputDebugStringW(L"pModelData��nullptr�ł��B�����Ɏ��s���܂���\n");
#endif // _DEBUG
		return false;
	}

#pragma region �f�[�^�Z�b�g
	if (userConstBufferData)this->userConstBufferData = *userConstBufferData;

	//obj�̃{�[����MoveVector�̊��鏈���̂��߂ɁA�I�u�W�F�N�g���ƂɃo�b�t�@�쐬
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


	//�}�e���A���擾
	std::vector<ADSAMaterial*>modelDataMtl = pModelData->GetPMaterial();

	size_t size = modelDataMtl.size();
	materials.resize(size);
	for (int i = 0; i < size; i++)
	{
		materials[i] = modelDataMtl[i];
	}

#pragma region �A�j���[�V�����֌W


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



	return true;
}



std::vector<std::vector<Vector3>> MelLib::ModelObject::GetVerticesData(const bool scaleImpact, const bool angleImpact, const bool transformImpact)
{
	std::vector<std::vector<Vector3>>verticesPos = pModelData->GetVerticesPosition();

	for (int i = 0, size = verticesPos.size(); i < size; i++)
	{
		for (auto& pos : verticesPos[i])
		{
			Matrix mat = Matrix::GetTranslationMatrix(pos);
			if (scaleImpact) mat *= Matrix::GetScalingMatrix(modelConstDatas[i].scale);
			if (angleImpact)mat *= Matrix::GetRotateZXYMatrix(modelConstDatas[i].angle);
			if (transformImpact)mat *= Matrix::GetTranslationMatrix(modelConstDatas[i].position);
			pos = Vector3(mat[3][0], mat[3][1], mat[3][2]);
		}
	}
	return verticesPos;

}