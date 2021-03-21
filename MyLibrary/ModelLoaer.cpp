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

	//とりあえず、マテリアル読み込みは強制
#pragma region 新読み込み
	vertices.clear();
	indices.clear();
	smoothNormalCalcData.clear();
	

	//読み取って一時的に入れる配列
	std::vector<DirectX::XMFLOAT3>loadPosVector;
	loadPosVector.reserve(1000);
	std::vector<DirectX::XMFLOAT2>loadUvVector;
	loadUvVector.reserve(1000);
	std::vector<DirectX::XMFLOAT3>loadNormalVector;
	loadNormalVector.reserve(1000);


	//添え字
	USHORT index[3];

	//一時的に入れる変数
	DirectX::XMFLOAT3 pos = { 0,0,0 };
	DirectX::XMFLOAT3 normal = { 0,0,0 };
	DirectX::XMFLOAT2 uv = { 0,0 };
	std::string mtlNama = "";

	//読み取ったテキストを格納する文字列
	std::string objText = "";

	//先頭文字
	std::string topStr = "";

	//ファイルストリーム
	std::ifstream obj;
	obj.open(path);
	if(!obj)
	{
		OutputDebugString(L"読み込みに失敗しました。.objファイルが見つかりませんでした。\n");
		return false;
	}

	//マテリアル読み込みでtrueにし、座標読み込み時にtrueだったら配列clear
	bool loadPreparation = false;

	//読み込んだ.obj内のモデル数
	int loadModel = 0;

	//ポリゴンの枚数
	int polygonCount = 0;

	//マテリアルを読み込んだかどうか
	bool loadMtlFile = false;

	//スムースシェーディング用データ格納用
	std::vector<std::unordered_map<int, DirectX::XMFLOAT3>>smoothVertexPos;

	//頂点読み込み数
	int vertexLoadCount = 0;


	//ボーン用
		DirectX::XMFLOAT3 bonePos = { 0,0,0 };
		int boneObjectNum = 0;//何個目のオブジェクトのデータかを入れる変数
		int boneNum = 0;//ボーンの番号
		bool loadBoneData = false;
		std::vector< std::vector<int>>temporaryBoneNumVec;//temporary は　仮
	

	while (std::getline(obj, objText))
	{
		std::istringstream lineStream(objText);

		//先頭文字取得
		topStr.clear();
		lineStream >> topStr;
		
#pragma region 頂点データ取得


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
				OutputDebugString(L"読み込みに失敗しました。.mtlファイルが.objに記述されていません。\n");
				return false;
			}

			if(!loadPreparation)
			{

				loadPreparation = true;

				//初期化
				loadPosVector.clear();
				loadUvVector.clear();
				loadNormalVector.clear();

				//スムーズシェーディング用配列修正
				smoothNormalCalcData.resize(smoothNormalCalcData.size() + 1);
				smoothVertexPos.resize(smoothVertexPos.size() + 1);


				//読み込み数カウントインクリメント
				loadModel++;

				//カウント初期化
				vertexLoadCount = 0;
				polygonCount = 0;

				//参照した配列のメモリ確保
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

#pragma region マテリアル関係

		//マテリアルのファイル名取得
		if (topStr.find("mtllib") != std::string::npos &&
			materialFileName) 
		{
			lineStream >> *materialFileName;
			loadMtlFile = true;
		}
		//マテリアル名取得
		if (topStr.find("usemtl") != std::string::npos) 
		{
			lineStream >> mtlNama;
			materialName.push_back(mtlNama);
			
			
			loadPreparation = false;
			
		}

#pragma endregion

#pragma region 割り当て
		
		if (topStr.find("f") != std::string::npos)
		{
			//アルゴリズムメモ
			//文字列の/の数で、三角形とか判断する?

			//メモここまで

			
			//スラッシュ入れる用
			char sluch;
			
			//頂点。これに一時的に入れてから、配列に入れる
			Vertex vertex;

			//三角形ポリゴンを読み込むので3
			int polygonValue = 3;

			for (int i = 0; i < polygonValue; i++)
			{
				//インデックス読み込み
				lineStream >> index[0];
				lineStream >> sluch;
				if(loadUV)
				lineStream >> index[1];
				lineStream >> sluch;
				lineStream >> index[2];

				//参照してる配列に代入
				vertex.pos = loadPosVector[index[0] - 1];
				vertex.normal = loadNormalVector[index[2] - 1];
				if (loadUV)
					vertex.uv = loadUvVector[index[1] - 1];
				else
					vertex.uv = { 0,0 };

				//格納
				vertices[loadModel - 1].push_back(vertex);
				indices[loadModel - 1].push_back(polygonCount);
				polygonCount++;
			}
			

		}
#pragma endregion
		
#pragma region ボーン読み込み
		//読み込み準備
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


		//bpあったらボーン座標追加
		if (objText.find("bp") != std::string::npos && bonePosVector)
		{
			lineStream >> bonePos.x;
			lineStream >> bonePos.y;
			lineStream >> bonePos.z;
			bonePosVector->push_back(bonePos);
		}

		if (objText.find("bd") != std::string::npos && boneNumVector)
		{

			//何個目のオブジェクトのデータか確認
			lineStream >> boneObjectNum;

			//番号取得
			lineStream >> boneNum;

			//仮配列にボーン番号追加
			temporaryBoneNumVec[boneObjectNum].push_back(boneNum);

		}

#pragma endregion

	}

	if(loadNum)
	*loadNum = loadModel;

	//仮配列から移動
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

#pragma region スムースシェーディング用データ格納

	//サイズをオブジェクトを読み込んだ回数分にしておく
	//smoothNormalCalcData.resize(smoothVertexPos.size());
	int v1Count = 0;

	for (int i = 0; i < loadModel; i++)
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


	

#pragma region 旧読み込み

	//
	//	//読み込みながらだとresizeできないから、
	//	//先にstringの配列に格納して全部見てから、stringの配列見てデータ格納したほうがいい?
	//
	//	auto deleteSizeZero = [&]()
	//	{
	//		//size0は消す(MAYAモデル読み込みによる無駄な配列を削除)
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
	//	//何個読み込んだか
	//	int loadCount = 0;
	//
	//	//リセット
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
	//	std::vector< std::vector<int>>temporaryBoneNumVec;//temporary は　仮
	//
	//	//std::vector<std::string>data;
	//	char slush;
	//	char top[20];
	//	char boneTopStr[2];
	//	int boneObjectNum = 0;//何個目のオブジェクトのデータかを入れる変数
	//	int boneNum = 0;//ボーンの番号
	//	int loopCount = 0;//ループ数(2次元以上の配列を拡張forでまわした時の添え字用)
	//	bool loadBoneData = false;
	//	int index = 0;
	//	std::string vertexData;
	//	std::ifstream obj;
	//
	//	//fのスラッシュ群(1/1/1)を読んだ回数
	//	int loadFCount = 0;
	//	int loadFLine = 0;//fの行を読んだ回数
	//
	//	//頂点読み込み数
	//	int vertexLoadCount = 0;
	//
	//	//スムースシェーディング用データ格納用
	//	std::vector<std::unordered_map<int, DirectX::XMFLOAT3>>smoothVertexPos;
	//
	//	obj.open(path);
	//
	//	while (std::getline(obj, vertexData))
	//	{
	//		std::istringstream lineStream(vertexData);
	//
	//		//先頭文字を入れる(vとかvtのこと)
	//		lineStream >> top;
	//
	//#pragma region 行の最初の文字を見て読み込み
	//
	//		//マテリアルファイル読み込み
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
	//		//別のオブジェクトが来たらカウント増やす&resize
	//		if (vertexData[0] == 'o' || vertexData[0] == 'g')
	//		{
	//			loadCount++;
	//			loadFCount = 0;
	//			loadFLine = 0;
	//			index = 0;
	//
	//			//毎回resizeするから遅くなる
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
	//			bool tryPolygon = false;//三角形ポリゴンかどうか
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
	//					std::vector<int>num;//読み取った数値を入れる
	//					
	//					//数値を桁ずつ読み込む
	//					while(1)
	//					{
	//						lineStream >> check;
	//
	//						//スラッシュだったら抜ける
	//						if (check == '/')
	//						{
	//							break; 
	//						}
	//						//三角形ポリゴンだったら抜ける
	//						if (check == '\0')
	//						{
	//							tryPolygon = true;
	//							loadFCount-= 2;
	//							break;
	//						}
	//						//数字を入れる
	//						num.push_back(atoi(&check));
	//					}
	//
	//					//三角形ポリゴンの場合、これ以上読み込む必要がないので、抜ける
	//					if (tryPolygon)break;
	//
	//					int numSize = num.size();
	//					//ひっくり返す
	//					std::reverse(num.begin(), num.end());
	//					
	//					//頂点の要素数計算
	//					for (int i = numSize - 1; i > -1; i--)
	//					{
	//						if (num[i] != -1)
	//						{
	//							if(i != 0)posNum += num[i] * (10 * i);
	//							else posNum += num[i];
	//						}
	//					}
	//
	//					//頂点を代入
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
	//						//四角形の場合、同じでも入れなおさないと、下の三角形化で要素数を超えた指定をして引っかかる
	//						//今のままだと三角形オンリーでもelse内に入らないので、修正する(修正した)
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
	//				//四角形から三角形へ
	//				if (i == 3) 
	//				{
	//					
	//
	//					//上で読み込んだ同じ頂点値だったら、それの頂点とインデックスをセットするようにしているが、
	//					//ここで再設定してしまい、同じでも別の頂点扱いになっているので直す
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
	//					//仮のvectorを作って、一旦移す
	//					//ここでエラー出る
	//					int che = loadFCount - 3 + 2 * loadFLine;
	//
	//					std::vector<Vertex>kariVert;
	//					kariVert.resize(4);
	//					kariVert[0] = vertices[loadCount - 1][loadFCount - 3 + 2 * loadFLine];
	//					kariVert[1] = vertices[loadCount - 1][loadFCount - 2 + 2 * loadFLine];
	//					kariVert[2] = vertices[loadCount - 1][loadFCount - 1 + 2 * loadFLine];
	//					kariVert[3] = vertices[loadCount - 1][loadFCount + 2 * loadFLine];
	//				
	//					//サイズを2増やす
	//					vertices[loadCount - 1].resize(vertices[loadCount - 1].size() + 2);
	//					
	//					//割り当て
	//					vertices[loadCount - 1][loadFCount - 3 + 2 * loadFLine] = kariVert[0];
	//					vertices[loadCount - 1][loadFCount - 2 + 2 * loadFLine] = kariVert[1];
	//					vertices[loadCount - 1][loadFCount - 1 + 2 * loadFLine] = kariVert[3];
	//					vertices[loadCount - 1][loadFCount + 2 * loadFLine] = kariVert[1];
	//					vertices[loadCount - 1][loadFCount + 1 + 2 * loadFLine] = kariVert[2];
	//					vertices[loadCount - 1][loadFCount +2 +2 * loadFLine] = kariVert[3];
	//
	//					//頂点2個増やしたので、2増やす
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
	//#pragma region ボーン読み込み
	//		//読み込み準備
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
	//		//bpあったらボーン座標追加
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
	//			//何個目のオブジェクトのデータか確認
	//			lineStream >> boneObjectNum;
	//
	//			//番号取得
	//			lineStream >> boneNum;
	//
	//			//仮配列にボーン番号追加
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
	//	//仮配列から移動
	//	if(boneNumVector)
	//		*boneNumVector = temporaryBoneNumVec;
	//	
	//	//ボーン読まなかったらここで無駄を消す
	//	if(!loadBoneData)deleteSizeZero();
	//
	//	obj.close();
	//#pragma endregion
	//
	//
	//#pragma region スムースシェーディング用データ格納
	//	//サイズをオブジェクトを読み込んだ回数分にしておく
	//	smoothNormalCalcData.resize(smoothVertexPos.size());
	//	int v1Count = 0;
	//	
	//	for (int i = 0; i < smoothNormalCalcData.size(); i++) 
	//	{
	//		for (auto& v1 : vertices[i]) 
	//		{
	//			auto itr = smoothVertexPos[i].begin();
	//			int itrNum = 0;
	//			//smoothVertexPosと比較して、smoothNormalCalcDataに入れていく
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
	//	//objの中身のモデル数を代入
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
