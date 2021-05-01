#include "FbxLoader.h"

#include<cassert>




FbxLoader::FbxLoader()
{
}


FbxLoader::~FbxLoader()
{
}

FbxLoader* FbxLoader::getInstance()
{
	static FbxLoader inst;
	return &inst;
}

void FbxLoader::initialize(ID3D12Device* device)
{
	assert(!fbxManager);

	this->device = device;

	fbxManager = FbxManager::Create();

	FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(ios);

	fbxImporter = FbxImporter::Create(fbxManager, "");
	
}

void FbxLoader::end()
{
	//Importer���J�����Ă���Manager���J������
	fbxImporter->Destroy();
	fbxManager->Destroy();
}

void FbxLoader::parseNodeRecursive
(
	FbxModel* model,
	FbxNode* fbxNode, 
	Node* parentNode
)
{
	using namespace DirectX;

	model->nodes.emplace_back();
	Node& node = model->nodes.back();

	std::string name = fbxNode->GetName();

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
			model->meshNode = &node;
			parseMesh(model, fbxNode);
		}
	}


	for (int i = 0; i < fbxNode->GetChildCount(); i++)
		parseNodeRecursive(model, fbxNode->GetChild(i), &node);
}


void FbxLoader::loadFbxModel(const std::string& modelPath)
{
	if (!fbxImporter->Initialize
	(
		modelPath.c_str(), 
		-1,
		fbxManager->GetIOSettings()
	))
		assert(0);

	FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxScene");
	fbxImporter->Import(fbxScene);

	FbxModel* model = new FbxModel();
	model->modelName = "";
	
	//�m�[�h���擾
	int nodeCount = fbxScene->GetNodeCount();
	//parseNodeRecursive()�Ŕz����Q�Ƃ��Ă�̂ŁA�������̊m�ۂ��Ȃ����Ȃ��悤�ɂ��邽�߂�reserve���Ă�
	model->nodes.reserve(nodeCount);

	parseNodeRecursive(model, fbxScene->GetRootNode());

	fbxScene->Destroy();
}



void FbxLoader::parseMesh(FbxModel* model, FbxNode* node)
{
	FbxMesh* fbxMesh = node->GetMesh();

	parseMeshVertices(model, fbxMesh);
}

void FbxLoader::parseMeshVertices(FbxModel* model, FbxMesh* fbxMesh)
{
	//ControlPoint�Ƃ́A���_�f�[�^�̂���(���̃��C�u�����ł���Vertex�\����)

	//���_���擾
	const int vertexNum = fbxMesh->GetControlPointsCount();

	auto& vertices = model->vertices;
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

void FbxLoader::parseMeshFaces(FbxModel* model, FbxMesh* fbxMesh)
{
	auto& vertices = model->vertices;
	auto& indices = model->getIndices();

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

void FbxLoader::parseMaterial(FbxModel* model, FbxMesh* fbxMesh)
{

}

void FbxLoader::parseTexture(FbxModel* model, const std::string& path)
{

}
