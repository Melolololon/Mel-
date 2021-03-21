#include "ModelLoader.h"
#include<fstream>
#include<string>
#include<sstream>
#include<DirectXMath.h>

ModelLoader::ModelLoader()
{
}


ModelLoader::~ModelLoader()
{
}

ModelLoader* ModelLoader::getInstance()
{
	static ModelLoader inst;
	return &inst;
}

bool ModelLoader::loadPMDModel(const char* path, std::vector<PMDVertex>& vertex, std::vector<unsigned short>& indices)
{
	char sig[3];
	PMDHeader pmdHeader;
	int vertNum;
	PMDVertex pmdVertex;
	int indexNum;
	FILE* model;

	auto flag = fopen_s(&model, path, "rb");
	if (flag != 0)return false;

	fread(sig, sizeof(sig), 1, model);
	fread(&pmdHeader, sizeof(pmdHeader), 1, model);
	fread(&vertNum, sizeof(vertNum), 1, model);
	vertex.resize(vertNum);
	fread(vertex.data(), vertex.size() * sizeof(PMDVertex), 1, model);
	fread(&indexNum, sizeof(indexNum), 1, model);
	indices.resize(indexNum);
	fread(indices.data(), indices.size() * sizeof(unsigned short), 1, model);

	fclose(model);

	return true;
}


bool  ModelLoader::loadOBJModel
(
	const char* path,
	bool loadUV, 
	bool loadNormal, 
	std::vector<std::vector<Vertex>>& vertices, 
	std::vector<std::vector<USHORT>>& indices,
	std::string* materialFileName,
	std::vector<std::string>&materialName, 
	std::vector<std::unordered_map<USHORT, std::vector<USHORT>>>& smoothNormalCalcData,
	int* loadNum,
	std::vector<DirectX::XMFLOAT3>* bonePosVector,
	std::vector<std::vector<int>>* boneNumVector
)
{

	//�Ƃ肠�����A�}�e���A���ǂݍ��݂͋���
#pragma region �V�ǂݍ���
	vertices.clear();
	indices.clear();
	smoothNormalCalcData.clear();
	

	//�ǂݎ���Ĉꎞ�I�ɓ����z��
	std::vector<DirectX::XMFLOAT3>loadPosVector;
	loadPosVector.reserve(1000);
	std::vector<DirectX::XMFLOAT2>loadUvVector;
	loadUvVector.reserve(1000);
	std::vector<DirectX::XMFLOAT3>loadNormalVector;
	loadNormalVector.reserve(1000);


	//�Y����
	USHORT index[3];

	//�ꎞ�I�ɓ����ϐ�
	DirectX::XMFLOAT3 pos = { 0,0,0 };
	DirectX::XMFLOAT3 normal = { 0,0,0 };
	DirectX::XMFLOAT2 uv = { 0,0 };
	std::string mtlNama = "";

	//�ǂݎ�����e�L�X�g���i�[���镶����
	std::string objText = "";

	//�擪����
	std::string topStr = "";

	//�t�@�C���X�g���[��
	std::ifstream obj;
	obj.open(path);
	if(!obj)
	{
		OutputDebugString(L"�ǂݍ��݂Ɏ��s���܂����B.obj�t�@�C����������܂���ł����B\n");
		return false;
	}

	//�}�e���A���ǂݍ��݂�true�ɂ��A���W�ǂݍ��ݎ���true��������z��clear
	bool loadPreparation = false;

	//�ǂݍ���.obj���̃��f����
	int loadModel = 0;

	//�|���S���̖���
	int polygonCount = 0;

	//�}�e���A����ǂݍ��񂾂��ǂ���
	bool loadMtlFile = false;

	//�X���[�X�V�F�[�f�B���O�p�f�[�^�i�[�p
	std::vector<std::unordered_map<int, DirectX::XMFLOAT3>>smoothVertexPos;

	//���_�ǂݍ��ݐ�
	int vertexLoadCount = 0;


	//�{�[���p
		DirectX::XMFLOAT3 bonePos = { 0,0,0 };
		int boneObjectNum = 0;//���ڂ̃I�u�W�F�N�g�̃f�[�^��������ϐ�
		int boneNum = 0;//�{�[���̔ԍ�
		bool loadBoneData = false;
		std::vector< std::vector<int>>temporaryBoneNumVec;//temporary �́@��
	

	while (std::getline(obj, objText))
	{
		std::istringstream lineStream(objText);

		//�擪�����擾
		topStr.clear();
		lineStream >> topStr;
		
#pragma region ���_�f�[�^�擾


		if(topStr.find("vt") != std::string::npos &&
			loadUV)
		{
			lineStream >> uv.x;
			lineStream >> uv.y;
			loadUvVector.push_back(uv);
		}
		else
		if(topStr.find("vn") != std::string::npos)
		{
			lineStream >> normal.x;
			lineStream >> normal.y;
			lineStream >> normal.z;
			loadNormalVector.push_back(normal);
		}
		else 
		if (topStr.find("v") != std::string::npos)
		{ 
			if(!loadMtlFile)
			{
				OutputDebugString(L"�ǂݍ��݂Ɏ��s���܂����B.mtl�t�@�C����.obj�ɋL�q����Ă��܂���B\n");
				return false;
			}

			if(!loadPreparation)
			{

				loadPreparation = true;

				//������
				loadPosVector.clear();
				loadUvVector.clear();
				loadNormalVector.clear();

				//�X���[�Y�V�F�[�f�B���O�p�z��C��
				smoothNormalCalcData.resize(smoothNormalCalcData.size() + 1);
				smoothVertexPos.resize(smoothVertexPos.size() + 1);


				//�ǂݍ��ݐ��J�E���g�C���N�������g
				loadModel++;

				//�J�E���g������
				vertexLoadCount = 0;
				polygonCount = 0;

				//�Q�Ƃ����z��̃������m��
				vertices.resize(vertices.size() + 1);
				vertices[vertices.size() - 1].reserve(1000);
				indices.resize(indices.size() + 1);
				indices[indices.size() - 1].reserve(1000);

			}

			lineStream >> pos.x;
			lineStream >> pos.y;
			lineStream >> pos.z;
			loadPosVector.push_back(pos);


			std::vector<USHORT>u;
			smoothNormalCalcData[loadModel - 1].emplace(vertexLoadCount, u);
			smoothVertexPos[loadModel - 1].emplace(vertexLoadCount, pos);
			vertexLoadCount++;
		}

#pragma endregion

#pragma region �}�e���A���֌W

		//�}�e���A���̃t�@�C�����擾
		if (topStr.find("mtllib") != std::string::npos &&
			materialFileName) 
		{
			lineStream >> *materialFileName;
			loadMtlFile = true;
		}
		//�}�e���A�����擾
		if (topStr.find("usemtl") != std::string::npos) 
		{
			lineStream >> mtlNama;
			materialName.push_back(mtlNama);
			
			
			loadPreparation = false;
			
		}

#pragma endregion

#pragma region ���蓖��
		
		if (topStr.find("f") != std::string::npos)
		{
			//�A���S���Y������
			//�������/�̐��ŁA�O�p�`�Ƃ����f����?

			//���������܂�

			
			//�X���b�V�������p
			char sluch;
			
			//���_�B����Ɉꎞ�I�ɓ���Ă���A�z��ɓ����
			Vertex vertex;

			//�O�p�`�|���S����ǂݍ��ނ̂�3
			int polygonValue = 3;

			for (int i = 0; i < polygonValue; i++)
			{
				//�C���f�b�N�X�ǂݍ���
				lineStream >> index[0];
				lineStream >> sluch;
				if(loadUV)
				lineStream >> index[1];
				lineStream >> sluch;
				lineStream >> index[2];

				//�Q�Ƃ��Ă�z��ɑ��
				vertex.pos = loadPosVector[index[0] - 1];
				vertex.normal = loadNormalVector[index[2] - 1];
				if (loadUV)
					vertex.uv = loadUvVector[index[1] - 1];
				else
					vertex.uv = { 0,0 };

				//�i�[
				vertices[loadModel - 1].push_back(vertex);
				indices[loadModel - 1].push_back(polygonCount);
				polygonCount++;
			}
			

		}
#pragma endregion
		
#pragma region �{�[���ǂݍ���
		//�ǂݍ��ݏ���
		if (objText.find("BoneData") != std::string::npos)
		{
			loadBoneData = true;

			if (bonePosVector)bonePosVector->reserve(99);
			if (boneNumVector)temporaryBoneNumVec.resize(vertices.size());

			int loopCount = 0;
			for (auto& num : temporaryBoneNumVec)
			{
				num.reserve(vertices[loopCount].size());
				loopCount++;
			}
		}


		//bp��������{�[�����W�ǉ�
		if (objText.find("bp") != std::string::npos && bonePosVector)
		{
			lineStream >> bonePos.x;
			lineStream >> bonePos.y;
			lineStream >> bonePos.z;
			bonePosVector->push_back(bonePos);
		}

		if (objText.find("bd") != std::string::npos && boneNumVector)
		{

			//���ڂ̃I�u�W�F�N�g�̃f�[�^���m�F
			lineStream >> boneObjectNum;

			//�ԍ��擾
			lineStream >> boneNum;

			//���z��Ƀ{�[���ԍ��ǉ�
			temporaryBoneNumVec[boneObjectNum].push_back(boneNum);

		}

#pragma endregion

	}

	if(loadNum)
	*loadNum = loadModel;

	//���z�񂩂�ړ�
	if (boneNumVector)
		*boneNumVector = temporaryBoneNumVec;
	
	
	vertices.shrink_to_fit();
	for(auto& v : vertices)
		v.shrink_to_fit();
	
	indices.shrink_to_fit();
	for (auto& i : indices)
		i.shrink_to_fit();

	smoothNormalCalcData.shrink_to_fit();
	for (auto& s : smoothNormalCalcData)
		for (auto& s2 : s)
			s2.second.shrink_to_fit();

	obj.close();

#pragma region �X���[�X�V�F�[�f�B���O�p�f�[�^�i�[

	//�T�C�Y���I�u�W�F�N�g��ǂݍ��񂾉񐔕��ɂ��Ă���
	//smoothNormalCalcData.resize(smoothVertexPos.size());
	int v1Count = 0;

	for (int i = 0; i < loadModel; i++)
	{
		for (auto& v1 : vertices[i])
		{
			auto itr = smoothVertexPos[i].begin();
			int itrNum = 0;
			//smoothVertexPos�Ɣ�r���āAsmoothNormalCalcData�ɓ���Ă���
			for (; itr != smoothVertexPos[i].end(); ++itr)
			{

				if (v1.pos.x == itr->second.x &&
					v1.pos.y == itr->second.y &&
					v1.pos.z == itr->second.z)
				{
					smoothNormalCalcData[i][itrNum].push_back(v1Count);
				}
				itrNum++;
			}

			v1Count++;
		}

		v1Count = 0;
	}

#pragma endregion



	return true;
#pragma endregion


	

#pragma region ���ǂݍ���

	//
	//	//�ǂݍ��݂Ȃ��炾��resize�ł��Ȃ�����A
	//	//���string�̔z��Ɋi�[���đS�����Ă���Astring�̔z�񌩂ăf�[�^�i�[�����ق�������?
	//
	//	auto deleteSizeZero = [&]()
	//	{
	//		//size0�͏���(MAYA���f���ǂݍ��݂ɂ�閳�ʂȔz����폜)
	//		int num = vertices.size();
	//		for (int i = 0; i < num; i++)
	//		{
	//			if (vertices[i].size() == 0)
	//			{
	//				vertices.erase(vertices.begin() + i);
	//				num--;
	//				i--;
	//			}
	//		}
	//		vertices.shrink_to_fit();
	//
	//		num = indices.size();
	//		for (int i = 0; i < num; i++)
	//		{
	//			if (indices[i].size() == 0)
	//			{
	//				indices.erase(indices.begin() + i);
	//				num--;
	//				i--;
	//			}
	//		}
	//		indices.shrink_to_fit();
	//
	//	};
	//
	//	//���ǂݍ��񂾂�
	//	int loadCount = 0;
	//
	//	//���Z�b�g
	//	vertices.clear();
	//	indices.clear();
	//	smoothNormalCalcData.clear();
	//
	//	//reserve
	//	vertices.reserve(5);
	//	indices.reserve(5);
	//	smoothNormalCalcData.reserve(5);
	//
	//
	//	DirectX::XMFLOAT3 pos = { 0,0,0 };
	//	DirectX::XMFLOAT2 uv = { 0,0 };
	//	DirectX::XMFLOAT3 normal = { 0,0,0 };
	//	DirectX::XMFLOAT3 bonePos = { 0,0,0 };
	//	std::vector<DirectX::XMFLOAT3 >vecPos;
	//	vecPos.reserve(9999);
	//	std::vector<DirectX::XMFLOAT2 >vecUV;
	//	vecUV.reserve(9999);
	//	std::vector<DirectX::XMFLOAT3 >vecNormal;
	//	vecNormal.reserve(9999);
	//
	//	std::vector< std::vector<int>>temporaryBoneNumVec;//temporary �́@��
	//
	//	//std::vector<std::string>data;
	//	char slush;
	//	char top[20];
	//	char boneTopStr[2];
	//	int boneObjectNum = 0;//���ڂ̃I�u�W�F�N�g�̃f�[�^��������ϐ�
	//	int boneNum = 0;//�{�[���̔ԍ�
	//	int loopCount = 0;//���[�v��(2�����ȏ�̔z����g��for�ł܂킵�����̓Y�����p)
	//	bool loadBoneData = false;
	//	int index = 0;
	//	std::string vertexData;
	//	std::ifstream obj;
	//
	//	//f�̃X���b�V���Q(1/1/1)��ǂ񂾉�
	//	int loadFCount = 0;
	//	int loadFLine = 0;//f�̍s��ǂ񂾉�
	//
	//	//���_�ǂݍ��ݐ�
	//	int vertexLoadCount = 0;
	//
	//	//�X���[�X�V�F�[�f�B���O�p�f�[�^�i�[�p
	//	std::vector<std::unordered_map<int, DirectX::XMFLOAT3>>smoothVertexPos;
	//
	//	obj.open(path);
	//
	//	while (std::getline(obj, vertexData))
	//	{
	//		std::istringstream lineStream(vertexData);
	//
	//		//�擪����������(v�Ƃ�vt�̂���)
	//		lineStream >> top;
	//
	//#pragma region �s�̍ŏ��̕��������ēǂݍ���
	//
	//		//�}�e���A���t�@�C���ǂݍ���
	//		if (vertexData[0] == 'm')
	//		{
	//			if (materialFileName) 
	//			{
	//				lineStream >> *materialFileName;
	//			}
	//		}
	//
	//		if (vertexData[0] == 'u') 
	//		{
	//			lineStream >> materialName[loadCount - 1];
	//		}
	//
	//		//�ʂ̃I�u�W�F�N�g��������J�E���g���₷&resize
	//		if (vertexData[0] == 'o' || vertexData[0] == 'g')
	//		{
	//			loadCount++;
	//			loadFCount = 0;
	//			loadFLine = 0;
	//			index = 0;
	//
	//			//����resize���邩��x���Ȃ�
	//			vertices.resize(loadCount);
	//			indices.resize(loadCount);
	//			materialName.resize(loadCount);
	//
	//			smoothNormalCalcData.resize(loadCount);
	//			smoothVertexPos.resize(loadCount);
	//		}
	//
	//		//UV
	//		if (vertexData[0] == 'v' && vertexData[1] == 't')
	//		{
	//			lineStream >> uv.x;
	//			lineStream >> uv.y;
	//			vecUV.push_back(uv);
	//		}
	//		//Normal
	//		else if (vertexData[0] == 'v'&& vertexData[1] == 'n')
	//		{
	//			lineStream >> normal.x;
	//			lineStream >> normal.y;
	//			lineStream >> normal.z;
	//			vecNormal.push_back(normal);
	//		}
	//		//Vertex
	//		else
	//		if (vertexData[0] == 'v')
	//		{
	//
	//			lineStream >> pos.x;
	//			lineStream >> pos.y;
	//			lineStream >> pos.z;
	//			vecPos.push_back(pos);
	//
	//			std::vector<USHORT>u;
	//			smoothNormalCalcData[loadCount - 1].emplace(vertexLoadCount, u);
	//			smoothVertexPos[loadCount - 1].emplace(vertexLoadCount, pos);
	//			vertexLoadCount++;
	//		}
	//		//index
	//		else
	//		if (vertexData[0] == 'f')
	//		{
	//			Vertex vertex;
	//			int posNum = 0;
	//			int uvNum = 0;
	//			int normalNum = 0;
	//			bool tryPolygon = false;//�O�p�`�|���S�����ǂ���
	//			int slushCount = 0;
	//			for (int i = 0; i < vertexData.size(); i++) 
	//			{
	//				if (vertexData[i] == '/')slushCount++;
	//			}
	//			if (slushCount == 6)tryPolygon = true;
	//
	//			for (int i = 0; i < 4; i++)
	//			{
	//				if (i == 3) 
	//				{
	//					posNum = 0;
	//					char check = '\0';
	//					std::vector<int>num;//�ǂݎ�������l������
	//					
	//					//���l�������ǂݍ���
	//					while(1)
	//					{
	//						lineStream >> check;
	//
	//						//�X���b�V���������甲����
	//						if (check == '/')
	//						{
	//							break; 
	//						}
	//						//�O�p�`�|���S���������甲����
	//						if (check == '\0')
	//						{
	//							tryPolygon = true;
	//							loadFCount-= 2;
	//							break;
	//						}
	//						//����������
	//						num.push_back(atoi(&check));
	//					}
	//
	//					//�O�p�`�|���S���̏ꍇ�A����ȏ�ǂݍ��ޕK�v���Ȃ��̂ŁA������
	//					if (tryPolygon)break;
	//
	//					int numSize = num.size();
	//					//�Ђ�����Ԃ�
	//					std::reverse(num.begin(), num.end());
	//					
	//					//���_�̗v�f���v�Z
	//					for (int i = numSize - 1; i > -1; i--)
	//					{
	//						if (num[i] != -1)
	//						{
	//							if(i != 0)posNum += num[i] * (10 * i);
	//							else posNum += num[i];
	//						}
	//					}
	//
	//					//���_����
	//					vertex.pos = vecPos[posNum - 1];
	//				}
	//
	//				if (i != 3) 
	//				{
	//					lineStream >> posNum;
	//					vertex.pos = vecPos[posNum - 1];
	//					lineStream >> slush;
	//				}
	//
	//				if (loadUV)
	//				{
	//					lineStream >> uvNum;
	//					vertex.uv = vecUV[uvNum - 1];
	//				}
	//				else
	//				{
	//					vertex.uv = { 0,0 };
	//				}
	//
	//				lineStream >> slush;
	//
	//				if (loadNormal)
	//				{
	//					lineStream >> normalNum;
	//					vertex.normal = vecNormal[normalNum - 1];
	//				}
	//				else
	//				{
	//					vertex.normal = { 0,0,0 };
	//				}
	//
	//				bool isOnazi = false;
	//				int size = vertices[loadCount - 1].size();
	//				for (UINT i = 0; i < size; i++)
	//				{
	//					
	//
	//					if (vertex.pos.x == vertices[loadCount - 1][i].pos.x&&
	//						vertex.pos.y == vertices[loadCount - 1][i].pos.y&&
	//						vertex.pos.z == vertices[loadCount - 1][i].pos.z&&
	//						vertex.uv.x == vertices[loadCount - 1][i].uv.x&&
	//						vertex.uv.y == vertices[loadCount - 1][i].uv.y&&
	//						vertex.normal.x == vertices[loadCount - 1][i].normal.x&&
	//						vertex.normal.y == vertices[loadCount - 1][i].normal.y&&
	//						vertex.normal.z == vertices[loadCount - 1][i].normal.z
	//						)
	//					{
	//						isOnazi = true;
	//
	//						//�l�p�`�̏ꍇ�A�����ł�����Ȃ����Ȃ��ƁA���̎O�p�`���ŗv�f���𒴂����w������Ĉ���������
	//						//���̂܂܂��ƎO�p�`�I�����[�ł�else���ɓ���Ȃ��̂ŁA�C������(�C������)
	//						if (!tryPolygon)
	//						{
	//							vertices[loadCount - 1].push_back(vertices[loadCount - 1][i]);
	//							indices[loadCount - 1].push_back(vertices[loadCount - 1].size() - 1); 
	//							index++;
	//						}
	//						else
	//						{
	//							indices[loadCount - 1].push_back(i);
	//						}
	//						break;
	//					}
	//				}
	//
	//				if (!isOnazi)
	//				{
	//					indices[loadCount - 1].push_back(index);
	//					vertices[loadCount - 1].push_back(vertex);
	//					index++;
	//				}
	//
	//				//�l�p�`����O�p�`��
	//				if (i == 3) 
	//				{
	//					
	//
	//					//��œǂݍ��񂾓������_�l��������A����̒��_�ƃC���f�b�N�X���Z�b�g����悤�ɂ��Ă��邪�A
	//					//�����ōĐݒ肵�Ă��܂��A�����ł��ʂ̒��_�����ɂȂ��Ă���̂Œ���
	//
	//					USHORT indNum = indices[loadCount - 1][loadFCount - 3 + 2 * loadFLine];
	//					indices[loadCount - 1].resize(indices[loadCount - 1].size() + 2);
	//					indices[loadCount - 1][loadFCount - 3 + 2 * loadFLine] = indNum;
	//					indices[loadCount - 1][loadFCount - 2 + 2 * loadFLine] = indNum + 1;
	//					indices[loadCount - 1][loadFCount - 1 + 2 * loadFLine] = indNum + 2;
	//					indices[loadCount - 1][loadFCount + 2 * loadFLine] = indNum + 3;
	//					indices[loadCount - 1][loadFCount + 1 + 2 * loadFLine] = indNum + 4;
	//					indices[loadCount - 1][loadFCount + 2 + 2 * loadFLine] = indNum + 5;
	//
	//
	//					//����vector������āA��U�ڂ�
	//					//�����ŃG���[�o��
	//					int che = loadFCount - 3 + 2 * loadFLine;
	//
	//					std::vector<Vertex>kariVert;
	//					kariVert.resize(4);
	//					kariVert[0] = vertices[loadCount - 1][loadFCount - 3 + 2 * loadFLine];
	//					kariVert[1] = vertices[loadCount - 1][loadFCount - 2 + 2 * loadFLine];
	//					kariVert[2] = vertices[loadCount - 1][loadFCount - 1 + 2 * loadFLine];
	//					kariVert[3] = vertices[loadCount - 1][loadFCount + 2 * loadFLine];
	//				
	//					//�T�C�Y��2���₷
	//					vertices[loadCount - 1].resize(vertices[loadCount - 1].size() + 2);
	//					
	//					//���蓖��
	//					vertices[loadCount - 1][loadFCount - 3 + 2 * loadFLine] = kariVert[0];
	//					vertices[loadCount - 1][loadFCount - 2 + 2 * loadFLine] = kariVert[1];
	//					vertices[loadCount - 1][loadFCount - 1 + 2 * loadFLine] = kariVert[3];
	//					vertices[loadCount - 1][loadFCount + 2 * loadFLine] = kariVert[1];
	//					vertices[loadCount - 1][loadFCount + 1 + 2 * loadFLine] = kariVert[2];
	//					vertices[loadCount - 1][loadFCount +2 +2 * loadFLine] = kariVert[3];
	//
	//					//���_2���₵���̂ŁA2���₷
	//					index+= 2;
	//
	//				}
	//				loadFCount++;
	//			}
	//			loadFLine++;
	//
	//
	//		}
	//
	//#pragma region �{�[���ǂݍ���
	//		//�ǂݍ��ݏ���
	//		if (vertexData.find("BoneData") != std::string::npos)
	//		{
	//			deleteSizeZero();
	//			loadBoneData = true;
	//
	//			if (bonePosVector)bonePosVector->reserve(999);
	//			if (boneNumVector)temporaryBoneNumVec.resize(vertices.size());
	//			
	//			loopCount = 0;
	//			for(auto& num : temporaryBoneNumVec)
	//			{
	//				num.reserve(vertices[loopCount].size());
	//				loopCount++;
	//			}
	//		}
	//	
	//
	//		//bp��������{�[�����W�ǉ�
	//		if(vertexData.find("bp") != std::string::npos && bonePosVector)
	//		{
	//			lineStream >> bonePos.x;
	//			lineStream >> bonePos.y;
	//			lineStream >> bonePos.z;
	//			bonePosVector->push_back(bonePos);
	//		}
	//
	//		if(vertexData.find("bd") != std::string::npos && boneNumVector)
	//		{
	//
	//			//���ڂ̃I�u�W�F�N�g�̃f�[�^���m�F
	//			lineStream >> boneObjectNum;
	//
	//			//�ԍ��擾
	//			lineStream >> boneNum;
	//
	//			//���z��Ƀ{�[���ԍ��ǉ�
	//			temporaryBoneNumVec[boneObjectNum].push_back(boneNum);
	//				
	//		}
	//
	//#pragma endregion
	//
	//#pragma endregion
	//
	//
	//	}
	//
	//	//���z�񂩂�ړ�
	//	if(boneNumVector)
	//		*boneNumVector = temporaryBoneNumVec;
	//	
	//	//�{�[���ǂ܂Ȃ������炱���Ŗ��ʂ�����
	//	if(!loadBoneData)deleteSizeZero();
	//
	//	obj.close();
	//#pragma endregion
	//
	//
	//#pragma region �X���[�X�V�F�[�f�B���O�p�f�[�^�i�[
	//	//�T�C�Y���I�u�W�F�N�g��ǂݍ��񂾉񐔕��ɂ��Ă���
	//	smoothNormalCalcData.resize(smoothVertexPos.size());
	//	int v1Count = 0;
	//	
	//	for (int i = 0; i < smoothNormalCalcData.size(); i++) 
	//	{
	//		for (auto& v1 : vertices[i]) 
	//		{
	//			auto itr = smoothVertexPos[i].begin();
	//			int itrNum = 0;
	//			//smoothVertexPos�Ɣ�r���āAsmoothNormalCalcData�ɓ���Ă���
	//			for (; itr != smoothVertexPos[i].end(); ++itr)
	//			{
	//				
	//				if (v1.pos.x == itr->second.x &&
	//					v1.pos.y == itr->second.y &&
	//					v1.pos.z == itr->second.z )
	//				{
	//					smoothNormalCalcData[i][itrNum].push_back(v1Count);
	//				}
	//				itrNum++;
	//			}
	//
	//			v1Count++;
	//		}
	//
	//		v1Count = 0;
	//	}
	//	
	//#pragma endregion
	//
	//
	//	//obj�̒��g�̃��f��������
	//	if (loadNum)
	//		*loadNum = vertices.size();
	//
	//
	//
	//	return true;

#pragma endregion

}

bool ModelLoader::loadObjMaterial(std::string materialDirectoryPath, std::string materialFileName, std::vector<Material>& material,  int* loadCount)
{
	//�ǂݍ��񂾉�
	int loadNum = 0;

	std::string kari;

	std::ifstream file;
	file.open(materialDirectoryPath + materialFileName);
	if (file.fail()) assert(0);

	std::string line;


	
	//int materialArrayNum = 0;//�}�e���A�������������ޏꏊ�̔ԍ�
	//std::string materialNameKari;//�}�e���A�������ꎞ�I�ɓ����string

	while (std::getline(file, line)) 
	{
		std::istringstream lineStream(line);
		std::string materialData;
		std::getline(lineStream, materialData,' ');

		if (materialData[0] == '\t')materialData.erase(materialData.begin());
		if (materialData == "newmtl")
		{
			loadNum++;
			material.resize(loadNum);

			//���O�擾
			lineStream >> material[loadNum - 1].materialName;

			//for (UINT i = 0; i < materialName.size(); i++) 
			//{
			//	//���̊֐��Ŏ擾�������O�ƈȑO�Ɏ擾�������O����v������Ai�ԖڂɃ}�e���A��������������
			//	//�������邱�ƂŁAobj��mtl�ł̃}�e���A���̏��Ԃ��Ⴍ�Ă������Ɠǂݍ��߂�
			//	//obj�ōŏ���2�A��ɂP  mtl�ōŏ���1�A���2�����Ă�
			//	//2�ڂ̃}�e���A����1�ڂ̃I�u�W�F�N�g�ɁA1�ڂ�2�ڂɃZ�b�g�ł���
			//	materialArrayNum = i;
			//	if (materialName[i] == materialNameKari)
			//	{
			//		material[i].materialName = materialName[i];
			//		break;
			//	}
			//}
		
		}
		if (materialData == "Ka") //�A���r�G���g
		{
			lineStream >> material[loadNum - 1].ambient.x;
			lineStream >> material[loadNum - 1].ambient.y;
			lineStream >> material[loadNum - 1].ambient.z;
		}
		if (materialData == "Kd") 
		{
			lineStream >> material[loadNum - 1].diffuse.x;
			lineStream >> material[loadNum - 1].diffuse.y;
			lineStream >> material[loadNum - 1].diffuse.z;
		}
		if (materialData == "Ks") 
		{
			lineStream >> material[loadNum - 1].specular.x;
			lineStream >> material[loadNum - 1].specular.y;
			lineStream >> material[loadNum - 1].specular.z;
		}
		if (materialData == "map_Kd") 
		{
			lineStream >> material[loadNum - 1].textureName;
		}


	}
	file.close();

	*loadCount = loadNum;
	return true;
}
