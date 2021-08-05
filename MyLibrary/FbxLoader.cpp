#include "FbxLoader.h"

#include<cassert>
#include<filesystem>


using namespace melLib;

FbxLoader::FbxLoader()
{
}


FbxLoader::~FbxLoader()
{
}

FbxLoader* FbxLoader::GetInstance()
{
	static FbxLoader instance;
	return &instance;
}

void FbxLoader::Initialize(ID3D12Device* device)
{
	assert(!fbxManager);

	this->device = device;

	fbxManager = FbxManager::Create();

	FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(ios);

	fbxImporter = FbxImporter::Create(fbxManager, "");
	
}

void FbxLoader::Finalize()
{
	//Importer���J�����Ă���Manager���J������
	fbxImporter->Destroy();
	fbxManager->Destroy();
}

bool FbxLoader::LoadFbxModel(const std::string& modelPath, ModelData* fbxModel)
{
	std::string modelName;
	ExtractFileName(modelPath, &modelDirectryPath, &modelName);

	if (!fbxImporter->Initialize
	(
		modelPath.c_str(),
		-1,
		fbxManager->GetIOSettings()
	)) return false;

	ModelData* model = fbxModel;

	FbxScene*& fbxScene = model->fbxData.fbxScene;
	fbxScene = FbxScene::Create(fbxManager, "fbxScene");
	fbxImporter->Import(fbxScene);

	model->modelName = modelName;

	//�m�[�h���擾
	int nodeCount = fbxScene->GetNodeCount();
	//parseNodeRecursive()�Ŕz����Q�Ƃ��Ă�̂ŁA���������m�ۂ��Ȃ����Ȃ��悤�ɂ��邽�߂�reserve���Ă�
	model->fbxData.bones.reserve(nodeCount);

	ParseNodeRecursive(model, fbxScene->GetRootNode());

	return true;
}


void FbxLoader::ParseNodeRecursive
(
	ModelData* fbxModel,
	FbxNode* fbxNode, 
	Node* parentNode
)
{
	using namespace DirectX;

	fbxModel->fbxData.nodes.emplace_back();
	Node& node = fbxModel->fbxData.nodes.back();

	node.nodeName = fbxNode->GetName();

	FbxDouble3 rotation = fbxNode->LclRotation.Get();
	FbxDouble3 scaling = fbxNode->LclScaling.Get();
	FbxDouble3 translation = fbxNode->LclTranslation.Get();

	node.rotation =
	{
		(float)rotation[0],
		(float)rotation[1],
		(float)rotation[2],
		0.0f
	};

	node.scaling =
	{
		(float)scaling[0],
		(float)scaling[1],
		(float)scaling[2],
		0.0f
	};

	node.translation =
	{
		(float)translation[0],
		(float)translation[1],
		(float)translation[2],
		0.0f
	};

	XMMATRIX matScaling, matRotation, matTranslation;
	matScaling = XMMatrixScalingFromVector(node.scaling);
	matRotation = XMMatrixRotationRollPitchYawFromVector(node.rotation);
	matTranslation = XMMatrixScalingFromVector(node.translation);

	node.transform = XMMatrixIdentity();
	node.transform *= matScaling;
	node.transform *= matRotation;
	node.transform *= matTranslation;

	node.globalTransform = node.transform;

	if(parentNode)
	{
		//�e�m�[�h���
		node.parentNode = parentNode;

		//�e�̍s�����Z
		node.globalTransform = parentNode->globalTransform;
	}

	//FbxNodeAttribute �m�[�h�̒ǉ����
	FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();
	if(fbxNodeAttribute)
	{
		if(fbxNodeAttribute->GetAttributeType() == 
			FbxNodeAttribute::eMesh)
		{
			fbxModel->fbxData.meshNode = &node;
			ParseMesh(fbxModel, fbxNode);
		}
	}

	for (int i = 0; i < fbxNode->GetChildCount(); i++)
		ParseNodeRecursive(fbxModel, fbxNode->GetChild(i), &node);
}



void FbxLoader::ParseMesh(ModelData* fbxModel, FbxNode* node)
{
	FbxMesh* fbxMesh = node->GetMesh();

	ParseMeshVertices(fbxModel, fbxMesh);
	ParseMeshFaces(fbxModel, fbxMesh);
	ParseMaterial(fbxModel, node);
	ParseSkin(fbxModel, fbxMesh);
}

void FbxLoader::ParseMeshVertices(ModelData* fbxModel, FbxMesh* fbxMesh)
{
	//���_���擾
	const int vertexNum = fbxMesh->GetControlPointsCount();

	auto& vertices = fbxModel->vertices;
	vertices.resize(1);
	vertices[0].resize(vertexNum);

	//���W�擾
	FbxVector4* pCount = fbxMesh->GetControlPoints();

	//�R�s�[
	for(int i = 0; i < vertexNum;i++)
	{
		vertices[0][i].pos.x = (float)pCount[i][0];
		vertices[0][i].pos.y = (float)pCount[i][1];
		vertices[0][i].pos.z = (float)pCount[i][2];
	}
}

void FbxLoader::ParseMeshFaces(ModelData* fbxModel, FbxMesh* fbxMesh)
{
	auto& vertices = fbxModel->vertices;
	auto& indicesVector = fbxModel->indices;
	indicesVector.resize(1);
	auto& indices = indicesVector[0];

	assert(indices.size() == 0);

	//�ʐ�
	const int polygonCount = fbxMesh->GetPolygonCount();
	const int textureUVCount = fbxMesh->GetTextureUVCount();

	//UV���̃��X�g
	FbxStringList uvNames;
	fbxMesh->GetUVSetNames(uvNames);

	//�ʂ��Ƃ̏��ǂݍ���
	for(int i = 0; i < polygonCount;i++)
	{
		//�ʂ̃C���f�b�N�X��
		const int polygonSize = fbxMesh->GetPolygonSize(i);
		assert(polygonSize <= 4);

		for(int j = 0; j < polygonSize;j++)
		{
			//vertices�̓Y����
			int index = fbxMesh->GetPolygonVertex(i, j);
			assert(index >= 0);


			//�@���ǂݍ���
			FbxVector4 normal;

			//GetPolygonVertexNormal(�ʔԍ�,�ʂ̉��ڂ̒��_��)
			if (fbxMesh->GetPolygonVertexNormal(i, j, normal))
			{
				vertices[0][index].normal.x = (float)normal[0];
				vertices[0][index].normal.y = (float)normal[1];
				vertices[0][index].normal.z = (float)normal[2];
			}


			//UV�ǂݍ���
			if(textureUVCount > 0)
			{
				FbxVector2 uvs;
				bool lUnmappedUV;

				if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappedUV))
				{
					vertices[0][index].uv.x = (float)uvs[0];
					vertices[0][index].uv.y = (float)uvs[1];
				}
			}

			if (j < 3)
				indices.push_back(index);
			else
			{
				int index2 = indices[indices.size() - 1];
				int index3 = index;
				int index0 = indices[indices.size() - 3];
				indices.push_back(index2);
				indices.push_back(index3);
				indices.push_back(index0);
			}
		}

	}
}

void FbxLoader::ParseMaterial(ModelData* fbxModel, FbxNode* fbxNode)
{
	auto& materialVector = fbxModel->materials;
	materialVector.resize(1);
	Material& modelMaterial = materialVector[0];

	//�e�X�g�p
	auto& pbrMaterialVector = fbxModel->pbrMaterials;
	pbrMaterialVector.resize(1);
	PbrMaterial& pbrModelMaterial = pbrMaterialVector[0];

	const int materialCount = fbxNode->GetMaterialCount();

	if(materialCount > 0)
	{
		FbxSurfaceMaterial* materials = fbxNode->GetMaterial(0);

		bool textureLoader = false;

		if(materials)
		{
			
			if (materials->GetClassId().Is(FbxSurfaceLambert::ClassId))
			{
				FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(materials);


				FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
				modelMaterial.ambient.x = (float)ambient.Get()[0];
				modelMaterial.ambient.y = (float)ambient.Get()[1];
				modelMaterial.ambient.z = (float)ambient.Get()[2];

				FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
				modelMaterial.diffuse.x = (float)diffuse.Get()[0];
				modelMaterial.diffuse.y = (float)diffuse.Get()[1];
				modelMaterial.diffuse.z = (float)diffuse.Get()[2];
				
				

			}
			else if(materials->GetClassId().Is(FbxSurfacePhong::ClassId))
			{
				FbxSurfacePhong* phong = static_cast<FbxSurfacePhong*>(materials);
				

				FbxPropertyT<FbxDouble3> ambient = phong->Ambient;
				modelMaterial.ambient.x = (float)ambient.Get()[0];
				modelMaterial.ambient.y = (float)ambient.Get()[1];
				modelMaterial.ambient.z = (float)ambient.Get()[2];

				FbxPropertyT<FbxDouble3> diffuse = phong->Diffuse;
				modelMaterial.diffuse.x = (float)diffuse.Get()[0];
				modelMaterial.diffuse.y = (float)diffuse.Get()[1];
				modelMaterial.diffuse.z = (float)diffuse.Get()[2];

				FbxPropertyT<FbxDouble3> specular = phong->Specular;
				modelMaterial.specular.x = (float)specular.Get()[0];
				modelMaterial.specular.y = (float)specular.Get()[1];
				modelMaterial.specular.z = (float)specular.Get()[2];
			}
			else
			{
				const FbxProperty propBaseColor = 
					FbxSurfaceMaterialUtils::GetProperty("baseColor", materials);
				if (propBaseColor.IsValid())
				{

					FbxDouble3 baseColor = propBaseColor.Get<FbxDouble3>();

					pbrModelMaterial.baseColor.x = (float)baseColor.Buffer()[0];
					pbrModelMaterial.baseColor.y = (float)baseColor.Buffer()[1];
					pbrModelMaterial.baseColor.z = (float)baseColor.Buffer()[2];
				}

				const FbxProperty propMetalness =
					FbxSurfaceMaterialUtils::GetProperty("metalness", materials);
				if (propMetalness.IsValid())
				{
					pbrModelMaterial.metalness = propMetalness.Get<float>();
				}


				const FbxProperty propSpecular =
					FbxSurfaceMaterialUtils::GetProperty("specular", materials);
				if (propSpecular.IsValid())
				{
					pbrModelMaterial.fSpecular = propSpecular.Get<float>();
				}

				const FbxProperty propSuecularRoughness =
					FbxSurfaceMaterialUtils::GetProperty("specularRoughness", materials);
				if (propSuecularRoughness.IsValid())
				{
					pbrModelMaterial.roughness = propSuecularRoughness.Get<float>();
				}

			}


			const FbxProperty diffuseProperty =
				materials->FindProperty(FbxSurfaceMaterial::sDiffuse);

			if(diffuseProperty.IsValid())
			{
				const FbxFileTexture* texture =
					diffuseProperty.GetSrcObject<FbxFileTexture>();

				if(texture)
				{
					const char* filePath = texture->GetFileName();

					std::string path_str(filePath);
					std::string name;
					ExtractFileName(path_str,nullptr, &name);

					//�ǂݍ��ݏ����܂���ModelData�N���X�Ƀp�X��n��������������
					//�t�@�C�����̂݋L�q����Ă�
					
					fbxModel->pTextures.resize(1);
					fbxModel->pTextures[0] = std::make_unique<Texture>();
					fbxModel->pTextures[0]->LoadModelTexture(modelDirectryPath + name);

					textureLoader = true;
				}
			}

		}
		

		if(!textureLoader)
		{
			//���C�u�����Ŏ������Ă�ׂ��h��e�N�X�`������������悤�ɂ���̂ŁA
			//���̂��������ύX����
			
			fbxModel->pTextures.resize(1);
			fbxModel->pTextures[0] = std::make_unique<Texture>();
			fbxModel->pTextures[0]->LoadModelTexture("Resources/Texture/WhiteTex.png");
		}
	}
}

void FbxLoader::ParseSkin(ModelData* fbxModel, FbxMesh* fbxMesh)
{
	//�X�L�j���O���擾
	FbxSkin* fbxSkin = 
		static_cast<FbxSkin*>(fbxMesh->GetDeformer(0, FbxDeformer::eSkin));

	//�X�L�j���O��񂪂Ȃ��ꍇreturn
	if (!fbxSkin)
	{
		auto vertSize = fbxModel->vertices[0].size();
		for(int i = 0; i < vertSize;i++)
		{
			//�ŏ��̃{�[���̉e���x��100%�ɂ���(�P�ʍs��̌��ʂ𔽉f�����邽��)
			fbxModel->vertices[0][i].boneIndex[0] = 0;
			fbxModel->vertices[0][i].boneWeight[0] = 1.0f;
		}
		return;
	}


	std::vector<ModelData::FbxBone>& bones = fbxModel->fbxData.bones;

	const int clusterCount = fbxSkin->GetClusterCount();
	bones.reserve(clusterCount);

	//�{�[���ԍ��ƃX�L���E�F�C�g
	struct WeightSet
	{
		UINT index;
		float weight;
	};
	std::vector<std::list<WeightSet>>weightLists(fbxModel->vertices[0].size());

	for(int i = 0; i < clusterCount;i++)
	{
		//�{�[�����(�N���X�^�[��SDK�Œ�`����Ă���{�[��?)
		FbxCluster* fbxCluster = fbxSkin->GetCluster(i);

		//�{�[���̃m�[�h���擾
		const char* boneName = fbxCluster->GetLink()->GetName();

		//�{�[���ǉ�
		bones.emplace_back(ModelData::FbxBone(boneName));
		ModelData::FbxBone& bone = bones.back();
		//����{�[����fbx�̃{�[���Ƃ̕R�t��
		bone.fbxCluster = fbxCluster;

		//�����p���s��̎擾
		FbxAMatrix fbxMat;
		fbxCluster->GetTransformLinkMatrix(fbxMat);

		//XMMATRIX�ɕϊ�
		DirectX::XMMATRIX initialPose;
		ConvertMatrixFromFbx(&initialPose, fbxMat);

		//�t�s����{�[���ɓn��
		bone.invInitialPose = DirectX::XMMatrixInverse(nullptr, initialPose);


		const int controlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();
		int* controlPointIndices = fbxCluster->GetControlPointIndices();
		double* controlPointWeight = fbxCluster->GetControlPointWeights();
		for(int j = 0; j < controlPointIndicesCount;j++)
		{
			const int vertIndex = controlPointIndices[j];
			const float weight = (float)controlPointWeight[j];

			weightLists[vertIndex].emplace_back(WeightSet{ (UINT)i,weight });
		}
	}


	auto& vertices = fbxModel->vertices;
	auto verticesSize = vertices[0].size();
	for(int i = 0; i < verticesSize;i++)
	{
		auto& weightList = weightLists[i];
		weightList.sort
		(
			[](auto const& lhs,auto const& rhs)
			{
				return lhs.weight > rhs.weight;
			}
		);

		int weightArrayIndex = 0;
		for(auto& weightSet: weightList)
		{
			vertices[0][i].boneIndex[weightArrayIndex] = weightSet.index;
			vertices[0][i].boneWeight[weightArrayIndex] = weightSet.weight;

			if(++weightArrayIndex >= FbxVertex::MAX_BONE_INDICES)
			{
				float weight = 0.0f;

				for(int j = 1; j < FbxVertex::MAX_BONE_INDICES;j++)
					weight += vertices[0][i].boneWeight[j];

				vertices[0][i].boneWeight[0] = 1.0f - weight;
				break;
				
			}
		}

	}
}


void FbxLoader::ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src)
{
	for(int i = 0; i < 4;i++)
	{
		for (int j = 0; j < 4; j++)
			dst->r[i].m128_f32[j] = (float)src.Get(i, j);
	}
	
}

std::string FbxLoader::ExtractFileName
(
	const std::string& path,
	std::string* directry,
	std::string* fileName
)
{
	size_t pos1;

	pos1 = path.rfind('\\');
	if (pos1 != std::string::npos) 
	{
		if(fileName)
			*fileName = path.substr(pos1 + 1, path.size() - pos1 - 1);

		if(directry)
			*directry = path.substr(0 , pos1 + 1);
	}
	pos1 = path.rfind('/');
	if (pos1 != std::string::npos) 
	{
		if (fileName)
			*fileName = path.substr(pos1 + 1, path.size() - pos1 - 1);


		if (directry)
			*directry = path.substr(0, pos1 + 1);
	}

	return path;
}
