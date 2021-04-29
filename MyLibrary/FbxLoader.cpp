#include "FbxLoader.h"

#include<cassert>
#include<filesystem>



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

void FbxLoader::LoadFbxModel(const std::string& modelPath, FbxModel* fbxModel)
{

	ExtractFileName(modelPath, &modelDirectryPath, nullptr);

	if (!fbxImporter->Initialize
	(
		modelPath.c_str(),
		-1,
		fbxManager->GetIOSettings()
	)) assert(0);


	FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScene");
	fbxImporter->Import(fbxScene);

	FbxModel* model = fbxModel;
	model->modelName = "";

	//�m�[�h���擾
	int nodeCount = fbxScene->GetNodeCount();
	//parseNodeRecursive()�Ŕz����Q�Ƃ��Ă�̂ŁA�������̊m�ۂ��Ȃ����Ȃ��悤�ɂ��邽�߂�reserve���Ă�
	model->nodes.reserve(nodeCount);

	ParseNodeRecursive(model, fbxScene->GetRootNode());

	fbxScene->Destroy();
}


void FbxLoader::ParseNodeRecursive
(
	FbxModel* fbxModel,
	FbxNode* fbxNode, 
	Node* parentNode
)
{
	using namespace DirectX;

	fbxModel->nodes.emplace_back();
	Node& node = fbxModel->nodes.back();

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
			fbxModel->meshNode = &node;
			ParseMesh(fbxModel, fbxNode);
		}
	}

	for (int i = 0; i < fbxNode->GetChildCount(); i++)
		ParseNodeRecursive(fbxModel, fbxNode->GetChild(i), &node);
}



void FbxLoader::ParseMesh(FbxModel* fbxModel, FbxNode* node)
{
	FbxMesh* fbxMesh = node->GetMesh();

	ParseMeshVertices(fbxModel, fbxMesh);
	ParseMeshFaces(fbxModel, fbxMesh);
	ParseMaterial(fbxModel, node);

}

void FbxLoader::ParseMeshVertices(FbxModel* fbxModel, FbxMesh* fbxMesh)
{
	//���_���擾
	const int vertexNum = fbxMesh->GetControlPointsCount();

	auto& vertices = fbxModel->vertices;
	vertices.resize(vertexNum);

	//���W�擾
	FbxVector4* pCount = fbxMesh->GetControlPoints();

	//�R�s�[
	for(int i = 0; i < vertexNum;i++)
	{
		vertices[i].pos.x = (float)pCount[i][0];
		vertices[i].pos.y = (float)pCount[i][1];
		vertices[i].pos.z = (float)pCount[i][2];
	}
}

void FbxLoader::ParseMeshFaces(FbxModel* fbxModel, FbxMesh* fbxMesh)
{
	auto& vertices = fbxModel->vertices;
	auto& m = fbxModel->getMaterial();
	auto& indicesVector = fbxModel->getIndices();
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
				vertices[index].normal.x = (float)normal[0];
				vertices[index].normal.y = (float)normal[1];
				vertices[index].normal.z = (float)normal[2];
			}


			//UV�ǂݍ���
			if(textureUVCount > 0)
			{
				FbxVector2 uvs;
				bool lUnmappedUV;

				if(fbxMesh->GetPolygonVertexUV(i,j,uvNames[0],uvs,lUnmappedUV))
				{
					vertices[index].uv.x = (float)uvs[0];
					vertices[index].uv.y = (float)uvs[1];
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

void FbxLoader::ParseMaterial(FbxModel* fbxModel, FbxNode* fbxNode)
{
	const int materialCount = fbxNode->GetMaterialCount();

	if(materialCount > 0)
	{
		FbxSurfaceMaterial* material = fbxNode->GetMaterial(0);

		bool textureLoader = false;

		if(material)
		{
			if(material->GetClassId().Is(FbxSurfaceLambert::ClassId))
			{
				FbxSurfacePhong* phong = static_cast<FbxSurfacePhong*>(material);
				auto& materialVector = fbxModel->getMaterial();
				materialVector.resize(1);
				Material& modelMaterial = materialVector[0];

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


			const FbxProperty diffuseProperty =
				material->FindProperty(FbxSurfaceMaterial::sDiffuse);

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

					//�ǂݍ��ݏ����܂���FbxModel�N���X�Ƀp�X��n��������������
					//�t�@�C�����̂݋L�q����Ă�
					
					fbxModel->textures.resize(1);
					fbxModel->textures[0] = std::make_unique<Texture>();
					fbxModel->textures[0]->LoadModelTexture(modelDirectryPath + name);

					textureLoader = true;
				}
			}

		}

		if(!textureLoader)
		{
			//�e�N�X�`���Ƀe�N�X�`���o�b�t�@�������A
			//���C�u�����Ŏ������Ă�ׂ��h��e�N�X�`���Ƀo�b�t�@����������悤�ɂ���̂ŁA
			//���̂��������ύX����
			fbxModel->textures.resize(1);
			fbxModel->textures[0] = std::make_unique<Texture>();
			fbxModel->textures[0]->LoadModelTexture(modelDirectryPath + "WriteTex.png");
		}
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
