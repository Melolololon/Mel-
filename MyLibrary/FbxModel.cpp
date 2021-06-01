#include "FbxModel.h"
#include"FbxLoader.h"


PipelineState FbxModel::defaultPipeline;

FbxModel::FbxModel()
{

}

FbxModel::~FbxModel()
{
	fbxScene->Destroy();
}

bool FbxModel::Load
(
	const std::string& path,
	const int createNum,
	//const size_t vertexSize,
	const size_t constDataSize
)
{
	FbxLoader::GetInstance()->LoadFbxModel(path,this);

	std::vector<size_t> verticesNum(1);
	verticesNum[0] = vertices.size();
	
	//�o�b�t�@�쐬
	CreateModelVertexResources
	(
		sizeof(FbxVertex),
		verticesNum,
		indices
	);


	FbxVertex* vertex;
	MapVertexBuffer
	(
		0,
		(void**)&vertex
	);

	auto vertexNum = vertices.size();
	for(int i = 0; i < vertexNum;i++)
		vertex[i] = vertices[i];
	UnmapVertexBuffer(0);


	auto texNum = textures.size();
	std::vector<Texture*>pTextures(texNum);
	for (int i = 0; i < texNum; i++)
		pTextures[i] = textures[i].get();


	//�{�[���o�b�t�@�̏��Z�b�g
	std::unique_ptr<BufferData> boneBufferData;
	//�{�[������������o�b�t�@����
	if (bones.size() != 0)
	{
		boneBufferData = std::make_unique<BufferData>();

		boneBufferData->bufferType = BufferData::BUFFER_TYPE_EACH_MODEL;
		boneBufferData->bufferDataSize = sizeof(SkinConstBufferData);
	}

	CreateModelHeapResourcesSetTexture
	(
		pTextures,
		createNum,
		1,
		boneBufferData.get(),
		nullptr
	);

	//������
	SkinConstBufferData* skinConstBufferData = nullptr;
	for (int i = 0; i < modelNum; i++) 
	{
		modelConstBuffer[i][0]->Map(0, nullptr, (void**)&skinConstBufferData);

		//�P�ʍs�������
		for (int j = 0; j < BONE_MAX; j++)
			skinConstBufferData->bones[j] = DirectX::XMMatrixIdentity();

		modelConstBuffer[i][0]->Unmap(0, nullptr);
	}

	pipeline = defaultPipeline.GetPipelineState();

#pragma region �A�j���[�V�����֌W����
	if (bones.size() != 0) 
	{
		currentTime.resize(createNum);

		FbxAnimStack* animstack = fbxScene->GetSrcObject<FbxAnimStack>(0);
		const char* animstackname = animstack->GetName();
		FbxTakeInfo* takeinfo = fbxScene->GetTakeInfo(animstackname);

		startTime = takeinfo->mLocalTimeSpan.GetStart();
		endTime = takeinfo->mLocalTimeSpan.GetStop();

		//1�b60�t���[���Őݒ肳��Ă�A�j���[�V�����̏ꍇ�AeFream60���Đݒ肷��?
		freamTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);
		animationMag.resize(createNum, 1.0f);

		for (int i = 0; i < createNum; i++)
			currentTime[i] = startTime;
	}


#pragma endregion


	//�ꎞ�I�ɏ����Ă�
	materials[0].ambient = { 0.1f,0.1f,0.1f };
	materials[0].diffuse = { 1.0f,1.0f,1.0f };

	return true;
}

bool FbxModel::Initialize()
{
	PipelineData data;
	data.alphaWriteMode = ALPHA_WRITE_TRUE;
	data.blendMode = BLEND_ADD;
	data.cullMode = CULL_BACK;
	data.depthMode = DEPTH_TRUE;
	data.drawMode = DRAW_SOLID;

	//�C���v�b�g���C�A�E�g
	std::vector<InputLayoutData> ilData(5);
	ilData[0].formatType = FORMAT_TYPE::FORMAT_TYPE_FLOAT;
	ilData[0].number = 3;
	ilData[0].semantics = "POSITION";
	ilData[1].formatType = FORMAT_TYPE::FORMAT_TYPE_FLOAT;
	ilData[1].number = 2;
	ilData[1].semantics = "TEXCOORD";
	ilData[2].formatType = FORMAT_TYPE::FORMAT_TYPE_FLOAT;
	ilData[2].number = 3;
	ilData[2].semantics = "NORMAL";

	ilData[3].formatType = FORMAT_TYPE::FORMAT_TYPE_UNSIGNED_INT;
	ilData[3].number = 4;
	ilData[3].semantics = "BONEINDICES";
	ilData[4].formatType = FORMAT_TYPE::FORMAT_TYPE_FLOAT;
	ilData[4].number = 4;
	ilData[4].semantics = "BONEWEIGHTS";

	auto result = defaultPipeline.CreatePipeline
	(
		data,
		{ L"../MyLibrary/FbxVertexShader.hlsl","main","vs_5_0" },
		{ L"../MyLibrary/FbxGeometryShader.hlsl","main","gs_5_0" },
		{ L"NULL","","" },
		{ L"NULL","","" },
		{ L"../MyLibrary/FbxPixelShader.hlsl","main","ps_5_0" },
		PipelineType::PIPELINE_TYPE_MODEL,
		&ilData,
		typeid(FbxModel).name()
	);

	if (!result)
	{
		OutputDebugString(L"FbxModel�̏������Ɏ��s���܂����B�f�t�H���g�p�C�v���C���𐶐��ł��܂���ł����B\n");
		return false;
	}
	return true;
}

void FbxModel::Draw(const int modelNum)
{
	MapConstData(modelNum);
	MapSkinData(modelNum);
	SetCmdList(modelNum);
}

void FbxModel::MapSkinData(const int modelNum)
{

	auto bonesSize = bones.size();
	if (bonesSize == 0)return;


	SkinConstBufferData* skinConstBufferData = nullptr;
	modelConstBuffer[modelNum][0]->Map(0, nullptr, (void**)&skinConstBufferData);

	for (int i = 0; i < bonesSize; i++)
	{
		//�ϊ�
		DirectX::XMMATRIX matCurrentPose;
		FbxAMatrix fbxCurrentPose =
			bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime[modelNum]);
		FbxLoader::GetInstance()->ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);

		//��Z
		skinConstBufferData->bones[i] = bones[i].invInitialPose * matCurrentPose;

	}
	
	modelConstBuffer[modelNum][0]->Unmap(0, nullptr);
}

void FbxModel::PlayAnimation(const int modelNum)
{
	if (bones.size() == 0)return;

	//�^�C����i�߂�
	currentTime[modelNum] += freamTime * animationMag[modelNum];

	if (currentTime[modelNum] > endTime)
		currentTime[modelNum] = startTime;
	if (currentTime[modelNum] < startTime)
		currentTime[modelNum] = endTime;
}

void FbxModel::ResetAnimation(const int modelNum)
{
	if (bones.size() == 0)return;
	currentTime[modelNum] = startTime; 
}

void FbxModel::SetCurrentFream(const UINT fream, const int modelNum)
{
	if (bones.size() == 0)return;

	FbxTime setTime = startTime * fream;
	if (setTime > endTime)
		setTime = endTime;

	currentTime[modelNum] = setTime;
}

void FbxModel::SetAnimationSpeedMagnification(const int magnification, const int modelNum)
{
	//float�Ŕ{���w�肷��ꍇ�A
	//�{�����牽�t���[����1�x�X�L�b�v���邩�ǂ����v�Z���Ȃ��Ƃ����Ȃ�

	if (bones.size() == 0)return;
	animationMag[modelNum] = magnification;
}

