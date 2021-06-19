#include "ObjModelObject.h"



void ObjModelObject::MapBoneMatrix()
{
	if (boneDatas.size() == 0
		|| !pModelData)return;

	std::vector<Vector3>objBonePositions = pObjModelData->GetBonePositions();
	BoneConstBufferData* boneConstBufferData;

	//�{�[���̍s���S�I�u�W�F�N�g�Ɏ�������̂͂��������Ȃ�����A����σo�b�t�@�������悤�ɂ����ق�������
	//(�{�[���̍s��́A�I�u�W�F�N�g�ŋ��ʂ�����)
	for (int i = 0; i < modelFileObjectNum; i++)
	{

#pragma region �{�[���̒l�}�b�v
		modelConstBuffer[i]->Map(0, nullptr, (void**)&boneConstBufferData);



		DirectX::XMMATRIX boneMat = DirectX::XMMatrixIdentity();
		DirectX::XMFLOAT3 boneScale;
		DirectX::XMFLOAT3 boneAngle;
		DirectX::XMFLOAT3 boneMoveVector;

		//�e�{�[���̍s���Z
		int parentBoneNum = 0;
		int bone = 0;

		DirectX::XMFLOAT3 boneAngleImpact = { 0.0f,0.0f,0.0f };
		DirectX::XMFLOAT3 boneScaleImpact = { 1.0f,1.0f,1.0f };
		DirectX::XMFLOAT3 boneMoveVectorImpact = { 0.0f,0.0f,0.0f };
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
			boneMat *= DirectX::XMMatrixTranslation
			(
				boneMoveVector.x / modelConstDatas[i].scale.x,
				boneMoveVector.y / modelConstDatas[i].scale.y,
				boneMoveVector.z / modelConstDatas[i].scale.z
			);

			//��]��������߂�
			boneMat *= DirectX::XMMatrixTranslation(bonePos.x, bonePos.y, bonePos.z);

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
			//�z���0�͐ݒ肵�Ȃ��悤�ɂ���(0�̓{�[���������Ă̒��_�̍s��Ȃ̂ŁA������Ȃ��悤�ɂ���)
			boneConstBufferData->boneMat[j + 1] = boneMat;

		}



		modelConstBuffer[i]->Unmap(0, nullptr);
#pragma endregion
	}

}


ObjModelObject::ObjModelObject(ObjModelData* pModelData, ConstBufferData* userConstBufferData)
	:ModelObject(pModelData, userConstBufferData)
{

	////�{�[���o�b�t�@�̏��Z�b�g
	//std::unique_ptr<ConstBufferData> boneBufferData;
	////�{�[������������{�[���o�b�t�@����
	//if (boneNum != 0)
	//{
	//	boneBufferData = std::make_unique<ConstBufferData>();

	//	//���f���̃I�u�W�F�N�g���Ƃ̃X�P�[�����|���邽�߁A���f���̃I�u�W�F�N�g���Ƃɍ��
	//	boneBufferData->bufferType = ConstBufferData::BUFFER_TYPE_EACH_MODEL_OBJECT;
	//	boneBufferData->bufferDataSize = sizeof(BoneConstBufferData);
	//}


}

ObjModelObject::~ObjModelObject()
{
}

bool ObjModelObject::Initialize()
{
	PipelineData pipelineData;
	pipelineData.alphaWriteMode = ALPHA_WRITE_TRUE;
	pipelineData.blendMode = BLEND_ADD;
	pipelineData.cullMode = CULL_BACK;
	pipelineData.depthMode = DEPTH_TRUE;
	pipelineData.drawMode = DRAW_SOLID;

	std::vector<InputLayoutData> layoutData(4);
	layoutData[0].semantics = "POSITION";
	layoutData[0].formatType = FORMAT_TYPE_FLOAT;
	layoutData[0].number = 3;
	layoutData[1].semantics = "TEXCOORD";
	layoutData[1].formatType = FORMAT_TYPE_FLOAT;
	layoutData[1].number = 2;
	layoutData[2].semantics = "NORMAL";
	layoutData[2].formatType = FORMAT_TYPE_FLOAT;
	layoutData[2].number = 3;
	layoutData[3].semantics = "BONENUM";
	layoutData[3].formatType = FORMAT_TYPE_UNSIGNED_INT;
	layoutData[3].number = 1;

	auto result = defaultPipeline.CreatePipeline
	(
		pipelineData,
		{ L"../MyLibrary/ObjAnimationVertexShader.hlsl","VSmain","vs_5_0" },
		{ L"../MyLibrary/ObjGeometryShader.hlsl","GSmain","gs_5_0" },
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"../MyLibrary/ObjPixelShader.hlsl","PSmain","ps_5_0" },
		PipelineType::PIPELINE_TYPE_MODEL,
		&layoutData,
		typeid(ObjModelObject).name(),
		1
	);

	if (!result)
	{
		OutputDebugString(L"ObjModel�̏������Ɏ��s���܂����B�f�t�H���g�p�C�v���C���𐶐��ł��܂���ł���\n");
		return false;
	}
	return true;
}

void ObjModelObject::Draw(const std::string& rtName)
{
	MapBoneMatrix();
	DrawCommonProcessing(rtName);
}
