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


//三角形ポリゴンと四角形ポリゴン混ざってたらエラー出るかも
bool  ModelLoader::loadOBJModel
(
	const char* path, bool loadUV, bool loadNormal, 
	std::vector<std::vector<Vertex>>& vertices, std::vector<std::vector<unsigned short>>& indices,
	std::string* materialFileName, std::vector<std::string>&materialName, 
	std::vector<std::unordered_map < USHORT, std::vector<USHORT>>>& smoothNormalCalcData,
	int* loadNum
)
{
	//何個読み込んだか
	int loadCount = 0;

	for (int i = 0; i < vertices.size(); i++)vertices[i].clear();
	for (int i = 0; i < indices.size(); i++)indices[i].clear();
	if (vertices.size() != 0)vertices.clear();
	if (indices.size() != 0)indices.clear();

	smoothNormalCalcData.clear();

	DirectX::XMFLOAT3 pos = { 0,0,0 };
	DirectX::XMFLOAT2 uv = { 0,0 };
	DirectX::XMFLOAT3 normal = { 0,0,0 };
	std::vector<DirectX::XMFLOAT3 >vecPos;
	std::vector<DirectX::XMFLOAT2 >vecUV;
	std::vector<DirectX::XMFLOAT3 >vecNormal;


	//std::vector<std::string>data;
	char slush;
	char top[20];
	int index = 0;
	std::string vertexData;
	std::ifstream obj;

	//fのスラッシュ群(1/1/1)を読んだ回数
	int loadFCount = 0;
	int loadFLine = 0;//fの行を読んだ回数

	//頂点読み込み数
	int vertexLoadCount = 0;

	//スムースシェーディング用データ格納用
	std::vector<std::unordered_map<int, DirectX::XMFLOAT3>>smoothVertexPos;

	obj.open(path);

	while (std::getline(obj, vertexData))
	{
		std::istringstream lineStream(vertexData);

		//先頭文字を入れる(vとかvtのこと)
		lineStream >> top;

		//マテリアルファイル読み込み
		if (vertexData[0] == 'm')
		{
			if (materialFileName) 
			{
				lineStream >> *materialFileName;
			}
		}

		if (vertexData[0] == 'u') 
		{
			lineStream >> materialName[loadCount - 1];
		}

		//別のやつが来たらカウント増やす&resize
		if (vertexData[0] == 'o' || vertexData[0] == 'g')
		{
			loadCount++;
			loadFCount = 0;
			loadFLine = 0;
			index = 0;
			vertices.resize(loadCount);
			indices.resize(loadCount);
			materialName.resize(loadCount);

			smoothNormalCalcData.resize(loadCount);
			smoothVertexPos.resize(loadCount);
		}

		//UV
		if (vertexData[0] == 'v' && vertexData[1] == 't')
		{
			lineStream >> uv.x;
			lineStream >> uv.y;
			vecUV.push_back(uv);
		}
		//Normal
		else if (vertexData[0] == 'v'&& vertexData[1] == 'n')
		{
			lineStream >> normal.x;
			lineStream >> normal.y;
			lineStream >> normal.z;
			vecNormal.push_back(normal);
		}
		//Vertex
		else
		if (vertexData[0] == 'v')
		{

			lineStream >> pos.x;
			lineStream >> pos.y;
			lineStream >> pos.z;
			vecPos.push_back(pos);

			std::vector<USHORT>u;
			smoothNormalCalcData[loadCount - 1].emplace(vertexLoadCount, u);
			smoothVertexPos[loadCount - 1].emplace(vertexLoadCount, pos);
			vertexLoadCount++;
		}
		//index
		else
		if (vertexData[0] == 'f')
		{
			Vertex vertex;
			int posNum = 0;
			int uvNum = 0;
			int normalNum = 0;
			bool tryPolygon = false;//三角形ポリゴンかどうか
			int slushCount = 0;
			for (int i = 0; i < vertexData.size(); i++) 
			{
				if (vertexData[i] == '/')slushCount++;
			}
			if (slushCount == 6)tryPolygon = true;

			for (int i = 0; i < 4; i++)
			{
				if (i == 3) 
				{
					posNum = 0;
					char check = '\0';
					std::vector<int>num;//読み取った数値を入れる
					
					//数値を桁ずつ読み込む
					while(1)
					{
						lineStream >> check;

						//スラッシュだったら抜ける
						if (check == '/')
						{
							break; 
						}
						//三角形ポリゴンだったら抜ける
						if (check == '\0')
						{
							tryPolygon = true;
							loadFCount-= 2;
							break;
						}
						//数字を入れる
						num.push_back(atoi(&check));
					}

					//三角形ポリゴンの場合、これ以上読み込む必要がないので、抜ける
					if (tryPolygon)break;

					int numSize = num.size();
					//ひっくり返す
					std::reverse(num.begin(), num.end());
					
					//頂点の要素数計算
					for (int i = numSize - 1; i > -1; i--)
					{
						if (num[i] != -1)
						{
							if(i != 0)posNum += num[i] * (10 * i);
							else posNum += num[i];
						}
					}

					//頂点を代入
					vertex.pos = vecPos[posNum - 1];
				}

				if (i != 3) 
				{
					lineStream >> posNum;
					vertex.pos = vecPos[posNum - 1];
					lineStream >> slush;
				}

				if (loadUV)
				{
					lineStream >> uvNum;
					vertex.uv = vecUV[uvNum - 1];
				}
				else
				{
					vertex.uv = { 0,0 };
				}

				lineStream >> slush;

				if (loadNormal)
				{
					lineStream >> normalNum;
					vertex.normal = vecNormal[normalNum - 1];
				}
				else
				{
					vertex.normal = { 0,0,0 };
				}

				bool isOnazi = false;
				int size = vertices[loadCount - 1].size();
				for (UINT i = 0; i < size; i++)
				{
					

					if (vertex.pos.x == vertices[loadCount - 1][i].pos.x&&
						vertex.pos.y == vertices[loadCount - 1][i].pos.y&&
						vertex.pos.z == vertices[loadCount - 1][i].pos.z&&
						vertex.uv.x == vertices[loadCount - 1][i].uv.x&&
						vertex.uv.y == vertices[loadCount - 1][i].uv.y&&
						vertex.normal.x == vertices[loadCount - 1][i].normal.x&&
						vertex.normal.y == vertices[loadCount - 1][i].normal.y&&
						vertex.normal.z == vertices[loadCount - 1][i].normal.z
						)
					{
						isOnazi = true;

						//四角形の場合、同じでも入れなおさないと、下の三角形化で要素数を超えた指定をして引っかかる
						//今のままだと三角形オンリーでもelse内に入らないので、修正する(修正した)
						if (!tryPolygon)
						{
							vertices[loadCount - 1].push_back(vertices[loadCount - 1][i]);
							indices[loadCount - 1].push_back(vertices[loadCount - 1].size() - 1); 
							index++;
						}
						else
						{
							indices[loadCount - 1].push_back(i);
						}
						break;
					}
				}

				if (!isOnazi)
				{
					indices[loadCount - 1].push_back(index);
					vertices[loadCount - 1].push_back(vertex);
					index++;
				}

				//四角形から三角形へ
				if (i == 3) 
				{
					

					//上で読み込んだ同じ頂点値だったら、それの頂点とインデックスをセットするようにしているが、
					//ここで再設定してしまい、同じでも別の頂点扱いになっているので直す

					USHORT indNum = indices[loadCount - 1][loadFCount - 3 + 2 * loadFLine];
					indices[loadCount - 1].resize(indices[loadCount - 1].size() + 2);
					indices[loadCount - 1][loadFCount - 3 + 2 * loadFLine] = indNum;
					indices[loadCount - 1][loadFCount - 2 + 2 * loadFLine] = indNum + 1;
					indices[loadCount - 1][loadFCount - 1 + 2 * loadFLine] = indNum + 2;
					indices[loadCount - 1][loadFCount + 2 * loadFLine] = indNum + 3;
					indices[loadCount - 1][loadFCount + 1 + 2 * loadFLine] = indNum + 4;
					indices[loadCount - 1][loadFCount + 2 + 2 * loadFLine] = indNum + 5;


					//仮のvectorを作って、一旦移す
					//ここでエラー出る
					int che = loadFCount - 3 + 2 * loadFLine;

					std::vector<Vertex>kariVert;
					kariVert.resize(4);
					kariVert[0] = vertices[loadCount - 1][loadFCount - 3 + 2 * loadFLine];
					kariVert[1] = vertices[loadCount - 1][loadFCount - 2 + 2 * loadFLine];
					kariVert[2] = vertices[loadCount - 1][loadFCount - 1 + 2 * loadFLine];
					kariVert[3] = vertices[loadCount - 1][loadFCount + 2 * loadFLine];
				
					//サイズを2増やす
					vertices[loadCount - 1].resize(vertices[loadCount - 1].size() + 2);
					
					//割り当て
					vertices[loadCount - 1][loadFCount - 3 + 2 * loadFLine] = kariVert[0];
					vertices[loadCount - 1][loadFCount - 2 + 2 * loadFLine] = kariVert[1];
					vertices[loadCount - 1][loadFCount - 1 + 2 * loadFLine] = kariVert[3];
					vertices[loadCount - 1][loadFCount + 2 * loadFLine] = kariVert[1];
					vertices[loadCount - 1][loadFCount + 1 + 2 * loadFLine] = kariVert[2];
					vertices[loadCount - 1][loadFCount +2 +2 * loadFLine] = kariVert[3];

					//頂点2個増やしたので、2増やす
					index+= 2;

				}
				loadFCount++;
			}
			loadFLine++;


		}
	}

	obj.close();
#pragma endregion


#pragma region スムースシェーディング用データ格納
	//サイズをオブジェクトを読み込んだ回数分にしておく
	smoothNormalCalcData.resize(smoothVertexPos.size());
	int v1Count = 0;
	
	for (int i = 0; i < smoothNormalCalcData.size(); i++) 
	{
		for (auto& v1 : vertices[i]) 
		{
			auto itr = smoothVertexPos[i].begin();
			int itrNum = 0;
			//smoothVertexPosと比較して、smoothNormalCalcDataに入れていく
			for (; itr != smoothVertexPos[i].end(); ++itr)
			{
				
				if (v1.pos.x == itr->second.x &&
					v1.pos.y == itr->second.y &&
					v1.pos.z == itr->second.z )
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

	//size0は消す
	int num = vertices.size();
	for (int i = 0; i < num ; i++) 
	{
		if (vertices[i].size() == 0) 
		{
			vertices.erase(vertices.begin() + i);
			num--;
			i--;
		}
	}
	vertices.shrink_to_fit();

	num = indices.size();
	for (int i = 0; i < num; i++)
	{
		if (indices[i].size() == 0)
		{
			indices.erase(indices.begin() + i); 
			num--;
			i--;
		}
	}
	indices.shrink_to_fit();

	if (loadNum)
		*loadNum = vertices.size();
	return true;
}

bool ModelLoader::loadObjMaterial(std::string materialDirectoryPath, std::string materialFileName, std::vector<Material>& material,  int* loadCount)
{
	//読み込んだ回数
	int loadNum = 0;

	std::string kari;

	std::ifstream file;
	file.open(materialDirectoryPath + materialFileName);
	if (file.fail()) assert(0);

	std::string line;


	
	//int materialArrayNum = 0;//マテリアル情報を書き込む場所の番号
	//std::string materialNameKari;//マテリアル名を一時的に入れるstring

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

			//名前取得
			lineStream >> material[loadNum - 1].materialName;

			//for (UINT i = 0; i < materialName.size(); i++) 
			//{
			//	//この関数で取得した名前と以前に取得した名前が一致したら、i番目にマテリアル情報を書き込む
			//	//こうすることで、objとmtlでのマテリアルの順番が違くてもちゃんと読み込める
			//	//objで最初に2、後に１  mtlで最初に1、後に2が来ても
			//	//2個目のマテリアルを1個目のオブジェクトに、1個目を2個目にセットできる
			//	materialArrayNum = i;
			//	if (materialName[i] == materialNameKari)
			//	{
			//		material[i].materialName = materialName[i];
			//		break;
			//	}
			//}
		
		}
		if (materialData == "Ka") //アンビエント
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
