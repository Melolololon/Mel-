#include "Library.h"
#include"LibMath.h"

#include"XInputManager.h"

#pragma comment(lib,"winmm.lib")//timeGetTimeと
#include"ObjectManager.h"
#include"TimerManager.h"
#include"SceneManager.h"

#include"Random.h"
#include"LibWinAPI.h"


std::unique_ptr<Audio> Library::audio;
DirectX12* Library::dx12;
CreatePolygon* Library::createPolygon;

WNDCLASSEX Library::w;
MSG  Library::msg;
HWND  Library::hwnd;
bool Library::isEnd;

bool Library::isDestroy;
UINT Library::count;

int Library::createPointCount;

bool Library::isSetFPS60;
UINT Library::fps;

int Library::loadFontTextureCounter;
int Library::loadTextureCounter;
int Library::createSpriteConter;

int Library::createPipelineCounter;
int Library::createPostEffectPuiperineCounter;

HDC Library::hdc;
int Library::refReat;
DWORD Library::startProsessTime;
DWORD Library::nowTime;

std::vector<std::tuple<ModelData, int>> Library::modelDatas;

LRESULT Library::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);

}

void Library::Initialize(int windowWidth, int windowHeight, const Color& screenColor, const wchar_t* windowName)
{
	dx12 = DirectX12::GetInstance();
	createPolygon = CreatePolygon::GetInstance();
	createPolygon->Initialize(windowWidth, windowHeight);

	srand((unsigned int)time(NULL));
	Random::Initialize();
	ObjectManager::GetInstance()->Initialize();

	count = 0;
	fps = 60;
	startProsessTime = timeGetTime();
	nowTime = timeGetTime();

#pragma region ウィンドウ処理

	hwnd = LibWinAPI::CreateNormalWindow
	(
		windowName,
		windowName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowWidth,
		windowHeight,
		nullptr,
		WindowProc
	);


	/*w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc;
	w.lpszClassName = windowName;
	w.hInstance = GetModuleHandle(nullptr);
	w.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&w);
	RECT wrc = { 0,0,windowWidth,windowHeight };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	hwnd = CreateWindow(
		w.lpszClassName,
		windowName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		w.hInstance,
		nullptr);

	ShowWindow(hwnd, SW_SHOW);*/



#pragma endregion

#pragma region DC

	//DCを取得
	hdc = GetDC(hwnd);
	//リフレッシュレートを取得
	refReat = GetDeviceCaps(hdc, VREFRESH);
#pragma endregion

#pragma region Input
	DirectInput::Initialize(hwnd, windowWidth, windowHeight);
#pragma endregion


	audio = std::unique_ptr<Audio>(new Audio());

	dx12->SetScreenColor(screenColor);
	dx12->Initialize(hwnd, windowWidth, windowHeight);
	

	createPipelineCounter = dx12->getStartPipelineCreateNum();
	//スプライトフォント300を作るのでカウントを増やしておく
	createSpriteConter = 300;

	//べた塗用テクスチャを0にするために1にする(DrawBoxなどに使う)
	loadTextureCounter = 1;

	//modelDatas.reserve(99999);
}

void Library::LoopStartProcess()
{
	startProsessTime = timeGetTime();

#pragma region ウィンドウ処理

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (msg.message == WM_QUIT)
	{
		isEnd = true;
		isDestroy = true;
	}


#pragma endregion

	dx12->LoopStartProcess();

#pragma region Input

	DirectInput::Update();
	XInputManager::Update();
#pragma endregion

}

void Library::LoopEndProcess()
{
	//ソート
	//dx12->sortModelData(modelDatas);

	//コマンドセット
	/*ModelData mData;
	int number;
	for(const auto& m : modelDatas)
	{
		mData = std::get<0>(m);
		number = std::get<1>(m);
		dx12->map(mData, number);
		dx12->setCmdList(mData.key, number);
	}*/

	//modelDatas.clear();

	dx12->LoopEndProcess();

	if (isSetFPS60)
	{

		//60fpsの時の1フレームの処理時間を計測(この数値は、環境に依存しない)
		float pureTime = 1000.0f / (float)fps * ((float)count - 1.0f);
		float currTime = 1000.0f / (float)fps * (float)count;
		int taikiTime = (int)currTime - (int)pureTime;//待機しないといけない時間
		count++;
		count = count >= fps ? 0 : count;

		//前ループからどのくらい時間が経過したかを計測(preを、startProsessのほうに移し、GetTickCount64入れる)
		//previousTime = currentTime;
		nowTime = timeGetTime();
		DWORD keika = nowTime - startProsessTime;
		//sabun /= 1000;

		//待機時間を計測(taikiTimeとsabunの差だけsleeで待機)
		int sleepTime = taikiTime - keika;
		sleepTime = sleepTime <= 0 ? 0 : sleepTime;

		//スリープ
		timeBeginPeriod(1);
		Sleep(sleepTime);
		timeEndPeriod(1);
	}

	TimerManager::GetInstance()->Update();
}

void Library::EndFlagTrue()
{
	isDestroy = false;
	isEnd = true;
}

bool Library::GetIsEnd()
{
	return isEnd;
}

void Library::Finalize()
{
	ObjectManager::GetInstance()->Finitialize();
	SceneManager::GetInstace()->Finitialize();

	dx12->Finalize();

	if (!isDestroy)
	{
		DestroyWindow(hwnd);
	}

	UnregisterClass(w.lpszClassName, w.hInstance);
}

int Library::GetRandomNumber(int number)
{
	return rand() % number;
}

float Library::GetRandomNumberFloat(int number)
{
	return static_cast<float>((rand() % number));
}

int Library::GetRandomNumberRangeSelect(const int& start, const int& end)
{
	return GetRandomNumber(abs(end - start) + 1) + start;
}

float Library::GetRandomNumberRangeSelectFloat(const int& start, const int& end)
{
	return static_cast<float>(GetRandomNumber(abs(end - start) + 1) + start);
}

#pragma region プライベート関数
bool Library::checkSetKeyName(const std::string& key)
{
	if (key == "")
	{
		OutputDebugString(L"キーの名前が設定されていません\n");
		return false;
	}
	return true;
}

bool Library::checkCreateVertexBuffer(const VertexType& vertexType)
{
	if (vertexType == VERTEX_TYPE_NONE)
	{
		OutputDebugString(L"オブジェクトの生成、またはモデルの読み込みがされていません\n");
		return false;
	}
	return true;
}
#pragma endregion


#pragma region WinAPI関係
HWND Library::GetHWND()
{
	return hwnd;
}
#pragma endregion

Vector2 Library::GetTextureSize(texture textureHandle)
{
	DirectX::XMFLOAT2 size = dx12->GetTextureSize(textureHandle);
	return { size.x,size.y };
}

#pragma region パイプライン設定
void Library::SetDespTestFlag(bool flag)
{
	//dx12->setDespTestFlag(flag);
}
#pragma endregion

#pragma region パイプライン作成
pipeline Library::CreateUserPipelineState(
	PipelineData pipelineData,
	ShaderData vShaderData,
	ShaderData gSyaderData,
	ShaderData pShaderData,
	bool useUserInputLayout
)
{
	bool result = dx12->CreateUserPipelineState
	(
		pipelineData,
		vShaderData,
		gSyaderData,
		pShaderData,
		useUserInputLayout
	);

	createPipelineCounter++;


	if (!result)return -1;
	return createPipelineCounter - 1;
}

void Library::SetPipeline(pipeline num)
{
	dx12->setPipelineNumber(num);
}


//void Library::setConstMapData(void** dataP, unsigned int dataSize)
//{
//	dx12->setConstMapData(dataP, dataSize);
//}

void Library::GetMatrix(float matrix[4][4], const ModelData& modelData, int number)
{
	dx12->GetMatrix(matrix, modelData.key, number);
}

void Library::GetCameraMatrix(float matrix[4][4])
{
	dx12->GetCameraMatrix(matrix);
}


void Library::SetInputLayout(const std::vector<InputLayoutData>& inputLayoutData)
{
	dx12->SetInputLayout(inputLayoutData);
}

void Library::DeleteInputLayout()
{
	dx12->DeleteInputLayout();
}



#pragma region ポストエフェクト
void Library::SetPostEffectPipeline(const pipeline& num)
{
	dx12->SetPostEffectPipeline(num);
}

pipeline Library::CreateUserPostEffectPipelineState
(
	const ShaderData& pShaderData
)
{
	createPostEffectPuiperineCounter++;
	dx12->CreateUserPostEffectPipelineState(pShaderData);
	return createPostEffectPuiperineCounter;
}
#pragma endregion


#pragma endregion


#pragma region 設定

void Library::SetFramesPerSecond60(bool flag)
{
	isSetFPS60 = flag;
}
//
//void Library::setScreenColor(Color color)
//{
//	dx12->setScreenColor(color);
//}

#pragma endregion

#pragma region 生成


#pragma region 頂点_インデックスバッファ作成

#pragma region モデル読み込み
void  Library::LoadObjVertex
(
	const char* path,
	bool loadUV,
	bool loadNormal,
	std::string* materialFireName,
	ModelData& modelData
)
{
	if (!checkSetKeyName(modelData.key))return;


	modelData.type = dx12->LoadObjVertex
	(
		path,
		loadUV,
		loadNormal,
		materialFireName,
		modelData.key
	);
	


}
#pragma endregion


void Library::CreatePoint(int createNum, point* p)
{
	*p = new int(createPointCount);
	dx12->CreatePoint(createNum, *p);
	createPointCount++;
}

void Library::CreateBoard(const Vector2& size, ModelData& modelData)
{
	if (!checkSetKeyName(modelData.key))return;

	Vector2 boardSize = { size.x,size.y};

	boardSize.y *= -1;
	float width = boardSize.x;
	float height = boardSize.y;
	width /= 2;
	height /= 2;

	dx12->CreatePolygonData
	(
		createPolygon->SetBoardPolygonVertex
		(
			{ 0 - width,boardSize.y - height,0 },//左下
			{ 0 - width,0 - height, 0 },//左上
			{ boardSize.x - width,boardSize.y - height,0 },//右下
			{ boardSize.x - width,0 - height,0 }//右上
		),
		createPolygon->SetBoardPolygonIndex(),
		modelData.key
	);
	modelData.type = VERTEX_TYPE_NORMAL;
}

//void Library::createCircle(float r, int dimention, ModelData& modelData)
//{
//	if (!checkSetKeyName(modelData.key))return;
//	PolygonData pData;
//
//	pData.fNum1 = r;
//	pData.katatiNum = 2;
//
//	/*p.handle = new int(dx12->getCreateNumber().polyNum);
//	pData.sikibetuNumP = p.handle;*/
//	pData.dimention = dimention;
//	dx12->createPolygonData(pData, modelData.key);
//
//
//	//円の頂点取得プログラム
//	/*temporaryVertices.push_back(createPolygon->setPolygonVertex
//	(
//		polygonData.fNum1,
//		50,
//		{ 0,0 }
//	));
//	temporaryIndices.push_back(createPolygon->setPolygonIndex(50));*/
//
//
//	
//	modelData.type = VERTEX_TYPE_NORMAL;
//}

//
//void Library::create3DBox(Vector3 size, VertexDataKey& p)
//{
//	PolyData pData;
//
//	pData.pos1 = { size.x,size.y,size.z };
//	pData.katatiNum = 10;
//
//	//pData.sikibetuNum = createPolygonNumber;
//
//	p.typr = VERTEX_TYPE_NORMAL;
//	/*p.handle = new int(dx12->getCreateNumber().polyNum);
//	pData.sikibetuNumP = p.handle;*/
//	pData.dimention = dimention3D;
//
//	dx12->createPolygonData(pData, p.key);
//}



void Library::CreateTriangularPyramid
(
	const float& r,
	const int& vertexNumber,
	const Vector3& centerPosition,
	const float& upVertex,
	ModelData& modelData
)
{
	if (!checkSetKeyName(modelData.key))return;
	
	dx12->CreatePolygonData
	(
		createPolygon->GetTriangularPyramidVertex
		(
			r,
			vertexNumber,
			centerPosition.ToXMFLOAT3(),
			upVertex
		),
		createPolygon->SetTriangularPyramidIndex(vertexNumber),
		modelData.key
	);

	
	modelData.type = VERTEX_TYPE_NORMAL;
}


void Library::Create3DBox(const Vector3& size,  ModelData& modelData)
{
	if (!checkSetKeyName(modelData.key))return;
	
	dx12->CreatePolygonData
	(
		createPolygon->GetVertexMany3DBox({ size.x,size.y,size.z }),
		createPolygon->GetVertexMany3DBoxIndex(),
		modelData.key
	);

	
	modelData.type = VERTEX_TYPE_NORMAL;
}

#pragma region ユーザー

void Library::CreateUserObject
(
	const std::vector<Vector3>& vertexPos,
	const std::vector<Vector2>& vertexUV,
	const std::vector<USHORT>& indices,
	ModelData& modelData
)
{
	size_t posSize = vertexPos.size();
	size_t uvSize = vertexUV.size();

	if (!checkSetKeyName(modelData.key))return;
	if (posSize != uvSize)
	{
		OutputDebugString(L"生成に失敗しました。頂点とUVのサイズが違います。\n");
		return;
	}
	
	std::vector<Vertex>vertices(posSize);
	for(int i = 0; i < posSize;i++)
	{
		vertices[i].pos = vertexPos[i].ToXMFLOAT3();
		vertices[i].uv = vertexUV[i].ToXMFLOAT2();
	}

	dx12->CreatePolygonData
	(
		vertices,
		indices,
		modelData.key

	);
	
	
	modelData.type = VERTEX_TYPE_NORMAL;
}

void Library::CreateUserObject2
(
	void** vertexData, 
	UINT vertexDataSize,
	UINT vertexSumDataSize,
	std::vector<USHORT>&index,
	ModelData& modelData
)
{
	if (!checkSetKeyName(modelData.key))return;

	dx12->CreateUserPolygon
	(
		vertexData,
		vertexDataSize, 
		vertexSumDataSize, 
		index, 
		modelData.key
	);
	
	modelData.type = VERTEX_TYPE_USER_VERTEX;
}
#pragma endregion



#pragma endregion

#pragma region ヒープ作成

void Library::LoadObjMaterial
(
	std::string materialDirectoryPath, 
	std::string materialFileName, 
	int objectNum, 
	 ModelData& modelData
)
{
	if (!checkCreateVertexBuffer(modelData.type))return;
	

	dx12->LoadObjMaterial(materialDirectoryPath, materialFileName, false, modelData.key, objectNum, modelData.type);
	
}

void Library::LoadObjMaterialUseUserData
(
	std::string materialDirectoryPath,
	std::string materialFileName,
	int objectNum,
	void** dataP,
	UINT dataSize,
	 ModelData& modelData
)
{
	if (!checkCreateVertexBuffer(modelData.type))return;
	dx12->setConstMapData(dataP, dataSize);
	
	dx12->LoadObjMaterial(materialDirectoryPath, materialFileName, true, modelData.key, objectNum, modelData.type);
	
}

void Library::CreateHeapData
(
	const wchar_t* texturePath, 
	int objectNum,
	ModelData& modelData
)
{
	


	if (!checkCreateVertexBuffer(modelData.type))return;

	dx12->CreateHeapData(false, modelData.key, objectNum, texturePath, nullptr);
	
	
}

void Library::CreateHeapData2
(
	Color color, 
	int objectNum,
	ModelData& modelData
)
{

	if (!checkCreateVertexBuffer(modelData.type))return;

	dx12->CreateHeapData(false, modelData.key, objectNum, L"", &color);

}

void Library::CreateUserHeapData
(
	const wchar_t* texturePath, 
	int objectNum, 
	void** dataP, 
	UINT dataSize,
	ModelData& modelData
)
{

	if (!checkCreateVertexBuffer(modelData.type))return;
	dx12->setConstMapData(dataP, dataSize);


	dx12->CreateHeapData(true, modelData.key, objectNum, texturePath, nullptr);

}

void Library::CreateUserHeapData2
(
	const Color& color,
	int objectNum,
	void** dataP,
	UINT dataSize,
	ModelData& modelData
)
{

	if (!checkCreateVertexBuffer(modelData.type))return;
	dx12->setConstMapData(dataP, dataSize);


	dx12->CreateHeapData(true, modelData.key, objectNum, L"", &color);

}


#pragma endregion

#pragma region 同時作成
//void Library::loadOBJ(const char* path, std::string materialDirectoryPath, bool loadUV, bool loadNormal, int objectNum, vertex* vertP, heap* heapP)
//{
//	//頂点読み込み時にファイル名取得して、heap作成時にマテリアルを読み込むようにする
//	//マテリアル名は、loadしたら受け取れるようにする
//	//同じような処理(ヒープ作成など)は関数化する
//	PolyData pData;
//	*vertP = new int(dx12->getCreateNumber().polyNum);
//	pData.sikibetuNumP = *vertP;
//	pData.dimention = dimention3D;
//
//	HeapData hData;
//	hData.objectNum = objectNum;
//	*heapP = new int(dx12->getCreateNumber().despNum);
//	hData.sikibetuNumP = *heapP
//		;
//	dx12->loadOBJ
//	(
//		path,
//		materialDirectoryPath,
//		loadUV,
//		loadNormal,
//		pData,
//		hData
//	);
//}
#pragma endregion


#pragma region スプライト

font Library::LoadSpriteFont
(
	const wchar_t *const texturePath,
	const Vector2& lineNum
)
{
	loadFontTextureCounter++;
	dx12->LoadSpriteFont(texturePath,lineNum.ToXMFLOAT2());

	return loadFontTextureCounter - 1;

}

//テクスチャの番号を返す
texture Library::LoadTexture(const wchar_t* texturePath)
{
	dx12->LoadTexture(texturePath, { 0,0,0,0 });

	loadTextureCounter++;
	return loadTextureCounter - 1;
}
//
//int Library::createSpriteTexture(Color color)
//{
//	dx12->loadTexture(L"", color);
//
//	loadTextureCounter++;
//	return loadTextureCounter - 1;
//}

//スプライトの識別番号を返す
void Library::CreateSprite(sprite* sprite)
{
	*sprite = new int(createSpriteConter);
	dx12->CreateSprite(*sprite, false);

	createSpriteConter++;
	//return createSpriteConter - 1;
}
#pragma endregion


#pragma endregion

#pragma region 描画

void Library::DrawGraphic
(
	const ModelData& modelData,
	int number
)
{
	/*std::tuple<ModelData, int> mData = std::make_tuple(modelData, number);
	modelDatas.push_back(mData);*/

	dx12->DataMap(modelData, number);
	dx12->SetCmdList(modelData, number);
}

void Library::DrawSprite
(
	const Vector2& position,
	const sprite& spriteNumber,
	const texture& textureNumber
)
{
	//dx12->spriteSetObjectPosition({ position.x,position.y }, *spriteNumber);
	dx12->SpriteDataMap(position.ToXMFLOAT2(), { 0,0 }, *spriteNumber, textureNumber);
	dx12->SpriteSetCmdList(*spriteNumber, textureNumber,false);
}

void Library::DrawSpriteAnimation
(
	const Vector2& position,
	const Vector2& maxSqare,
	const Vector2& currentNum,
	const sprite& spriteNumber,
	const texture& textureNumber
)
{
	//この順番じゃないとuvがちゃんとセットされない
	dx12->SpriteDataMap({ position.x,position.y }, { 0,0 }, *spriteNumber, textureNumber);
	dx12->SetSpriteAnimationVertex(*spriteNumber, textureNumber, maxSqare.ToXMFLOAT2(), currentNum.ToXMFLOAT2());
	dx12->SpriteSetCmdList(*spriteNumber, textureNumber, false);
}

void Library::DrawSpriteAnimation2
(
	const Vector2& position,
	const Vector2& currentStartNum,
	const Vector2& currentEndNum,
	const sprite& spriteNumber,
	const texture& textureNumber
)
{
	//dx12->spriteMap({ position.x,position.y }, { 0,0 }, *spriteNumber, *textureNumber);
	dx12->SetSpriteAnimationVertex2
	(
		*spriteNumber,
		textureNumber,
		position.x,
		position.y,
		currentEndNum.x - currentStartNum.x,
		currentEndNum.y - currentStartNum.y,
		currentStartNum.x,
		currentStartNum.y,
		currentEndNum.x,
		currentEndNum.y
	);

	dx12->SpriteSetCmdList(*spriteNumber, textureNumber, false);
}

void Library::DrawSprite3D
(
	const Vector3& position,
	const Vector2& size,
	const sprite& spriteNumber,
	const texture& textureNumber
)
{
	dx12->Sprite3DDataMap
	(
		position.ToXMFLOAT3(),
		size.ToXMFLOAT2(),
		*spriteNumber, 
		textureNumber
	);
	dx12->SpriteSetCmdList(*spriteNumber, textureNumber,true);
}

void Library::DrawSprite3DAnimation
(
	const Vector3& position,
	const Vector2& size,
	const Vector2& leftUpPos,
	const Vector2& rightDownPos,
	const sprite& spriteNumber,
	const texture& textureNumber
)
{
	dx12->Sprite3DDataMap
	(
		position.ToXMFLOAT3(),
		size.ToXMFLOAT2(),
		*spriteNumber,
		textureNumber
	);
	dx12->SetSprite3DAnimation
	(
		leftUpPos.ToXMFLOAT2(),
		rightDownPos.ToXMFLOAT2(),
		*spriteNumber,
		textureNumber
	);
	dx12->SpriteSetCmdList(*spriteNumber, textureNumber,true);
}

void Library::DrawPointTexture
(
	Vector3 pos, 
	point point, 
	texture texture, 
	int num
)
{
	dx12->PointSetCmdList({ pos.x,pos.y,pos.z }, *point, texture, num);
}

#pragma region 形状
void Library::DrawSpriteBox
(
	const Vector2& position,
	const Vector2& size,
	const Color& color,
	const sprite& spriteHandle
)
{
	SetSpriteAddColor(color, spriteHandle);
	dx12->SpriteDataMap(position.ToXMFLOAT2(), size.ToXMFLOAT2(), *spriteHandle, 0);
	dx12->SpriteSetCmdList(*spriteHandle, 0,false);
}

void Library::DrawSprite3DBox
(
	const Vector3& position,
	const Vector2& size,
	const Color& color,
	const sprite& spriteHandle
)
{
	SetSpriteAddColor(color, spriteHandle);
	dx12->Sprite3DDataMap(position.ToXMFLOAT3(), size.ToXMFLOAT2(), *spriteHandle, 0);
	dx12->SpriteSetCmdList(*spriteHandle, 0,true);
}

#pragma endregion


#pragma endregion

#pragma region 削除
void Library::DeleteModelData(const ModelData& modelData)
{
	dx12->DeletePolygonData(modelData);
	dx12->DeleteHeapData(modelData);
}


void Library::DeleteSprite(sprite sprite)
{
	dx12->DeleteSprite(*sprite);
}
#pragma endregion

#pragma region 見た目


#pragma region スムースシェーディング
void Library::SetSmoothingFlag(bool flag)
{
	dx12->SetSmoothingFlag(flag);
}
#pragma endregion

void Library::SetMulColor(Color color,const ModelData& modelData, int number)
{
	
	dx12->SetMulColor(color, modelData.key, number);
}
void Library::SetAddColor(Color color,const ModelData& modelData, int number)
{

	dx12->SetAddColor(color, modelData.key, number);
}
void Library::SetSubColor(Color color,const ModelData& modelData, int number)
{
	
	dx12->SetSubColor(color, modelData.key, number);
}

void Library::SetIsPlane(bool flag)
{
	dx12->SetIsPlane(flag);
}

void Library::SetIsBillboard(bool x, bool y, bool z)
{
	dx12->SetIsBillboard(x, y, z);
}

void Library::SetSpriteMulColor(Color color, sprite spriteNum)
{
	dx12->SetSpriteMulColor(color, *spriteNum);
}
void Library::SetSpriteAddColor(Color color, sprite spriteNum)
{
	dx12->SetSpriteAddColor(color, *spriteNum);
}
void Library::SetSpriteSubColor(Color color, sprite spriteNum)
{
	dx12->SetSpriteSubColor(color, *spriteNum);
}


void Library::SetPointScale(Vector2 scale, point pointNum, int num)
{
	dx12->SetPointScale({ scale.x,scale.y }, *pointNum, num);
}
#pragma endregion

#pragma region カメラ
void Library::SetCameraMatrixPoint(Vector3 position, Vector3 target, Vector3 up)
{
	dx12->SetCameraDataMatrixPoint(position, target, up);
}

void Library::SetCamera(Vector3 position, Vector3 target, Vector3 up)
{
	dx12->SetCameraData(position, target, up);


}

void Library::SetCameraVelocity(Vector3 eyeVelocity, Vector3 targetVelocity)
{
	dx12->SetCameraVelocity({ eyeVelocity.x,eyeVelocity.y,eyeVelocity.z }, { targetVelocity.x,targetVelocity.y,targetVelocity.z });
}

void Library::SetCameraAngle(Vector3 eyeAngle, Vector3 targetAngle, Vector3 upAngle)
{
	dx12->SetCameraAngre({ eyeAngle.x,eyeAngle.y,eyeAngle.z }, { targetAngle.x ,targetAngle.y,targetAngle.z }, { upAngle.x,upAngle.y,upAngle.z });
}

void Library::SetCameraNearAndFar(float nearNum, float farNum)
{
	dx12->SetNearAndFar(nearNum, farNum);
}

#pragma endregion

#pragma region ライト

void Library::SetLightVector(Vector3 vector)
{
	Vector3 v = Vector3Normalize(vector);
	dx12->SetLightVector({ v.x,v.y,v.z });
}
//

void Library::SetLightColor(Color lightColor)
{
	dx12->SetLightColor(lightColor);
}
#pragma endregion

#pragma region 操作
void Library::SetPosition(Vector3 position, const ModelData& modelData , int number)
{
	
	dx12->SetObjectPosition({ position.x,position.y,position.z }, modelData.key, number);
}


void Library::SetAngle(Vector3 angle, const ModelData& modelData,  int number)
{

	dx12->SetObjectAngle({ angle.x,angle.y,angle.z }, modelData.key, number);
}

void Library::SetScale(Vector3 scale, const ModelData& modelData , int number)
{
	dx12->SetObjectScale({ scale.x,scale.y,scale.z }, modelData.key, number);
}

void Library::SetPushPorigonNumber(float ex, const ModelData& modelData, int number)
{
	dx12->SetObjectPushNum(ex, modelData.key, number);
}


void Library::SetSpritePosition(Vector2 position, sprite sptiteNumber)
{
	dx12->SetSpritePosition({ position.x,position.y }, *sptiteNumber);
}
void Library::SetSpriteScale(Vector2 scale, sprite sptiteNumber)
{
	dx12->SetSpriteScale({ scale.x,scale.y }, *sptiteNumber);
}
void Library::SetSpriteAngle(float angle, sprite spriteNumber)
{
	dx12->SetSpriteAngle({0,0,angle }, *spriteNumber);
}
void Library::SetSpriteAngle3D(const Vector3& angle, const sprite& spriteNumber)
{
	dx12->SetSpriteAngle(angle.ToXMFLOAT3(), *spriteNumber);
}

void ChangeSpriteSize(Vector2 size, int *spriteData)
{

}



void Library::SetPointMulColor(Color color, point pointNum, int num)
{
	dx12->SetPointMulColor(color, *pointNum, num);
}



#pragma region ポストエフェクト

void Library::SetRenderTargetPosition(const Vector3& pos, const int& rtNum) 
{
	dx12->SetRenderTargerPosition({ pos .x,pos .y,pos .z}, rtNum);
}

void Library::SetRenderTargetAngle(const Vector3& angle, const int& rtNum)
{
	dx12->SetRenderTargetAngle({ angle .x,angle .y,angle .z}, rtNum);
}

void Library::SetRenderTargetScale(const Vector3& scale, const int& rtNum)
{
	dx12->SetRenderTargetScale({ scale.x,scale.y,scale.z }, rtNum);
}

void Library::SetPostEffectCameraFlag(const bool& flag, const int& rtNum)
{
	dx12->SetPostEffectCameraFlag(true, rtNum);
}
#pragma endregion

#pragma region アニメーション

//void setOBJModelPoint
//(
//	const Vector3& position, 
//	const UINT& boneNum,
//	const ModelData& modelData
//)
//{
//
//}

void Library::SetObjBoneMoveVector
(
	const Vector3& vector, 
	const UINT& boneNum,
	const ModelData& modelData,
	const UINT& objectNum
)
{
	if (modelData.type != VertexType::VERTEX_TYPE_OBJ_ANIMATION)
		return;

	dx12->SetObjBoneMoveVector(vector.ToXMFLOAT3(), boneNum, modelData.key, objectNum);

}

void Library::SetObjBoneScale
(
	const Vector3& scale,
	const UINT& boneNum,
	const ModelData& modelData,
	const UINT& objectNum
)
{
	if (modelData.type != VertexType::VERTEX_TYPE_OBJ_ANIMATION)
		return;
	dx12->SetObjBoneScale(scale.ToXMFLOAT3(), boneNum, modelData.key, objectNum);
}

void Library::SetObjBoneAngle
(
	const Vector3& angle, 
	const UINT& boneNum,
	const ModelData& modelData,
	const UINT& objectNum
)
{
	if (modelData.type != VertexType::VERTEX_TYPE_OBJ_ANIMATION)
		return;
	dx12->SetObjBoneAngle(angle.ToXMFLOAT3(), boneNum, modelData.key, objectNum);
}

void Library::SetParentObjBone
(
	const UINT& boneNum,
	const UINT& parentBoneNum,
	const ModelData& modelData
)
{
	if (modelData.type != VertexType::VERTEX_TYPE_OBJ_ANIMATION)
		return;

	dx12->SetParentObjBone
	(
		boneNum, 
		parentBoneNum,
		modelData.key
	);
}

void Library::SetParentObjBoneScaleImpact
(
	const UINT& boneNum,
	const Vector3& scaleImpact,
	const ModelData& modelData
)
{
	if (modelData.type != VertexType::VERTEX_TYPE_OBJ_ANIMATION)
		return;
	dx12->SetParentObjBoneScaleImpact
	(
		boneNum,
		scaleImpact.ToXMFLOAT3(),
		modelData.key
	);
}

void Library::SetParentObjBoneAngleImpact
(
	const UINT& boneNum,
	const Vector3& angleImpact,
	const ModelData& modelData
)
{
	if (modelData.type != VertexType::VERTEX_TYPE_OBJ_ANIMATION)
		return;
	dx12->SetParentObjBoneAngleImpact
	(
		boneNum,
		angleImpact.ToXMFLOAT3(),
		modelData.key
	);
}

void Library::SetParentObjBoneMoveVectorImpact
(
	const UINT& boneNum,
	const Vector3& moveVectorImpact,
	const ModelData& modelData
)
{
	if (modelData.type != VertexType::VERTEX_TYPE_OBJ_ANIMATION)
		return;
	dx12->SetParentObjBoneMoveVectorImpact
	(
		boneNum,
		moveVectorImpact.ToXMFLOAT3(),
		modelData.key
	);
}

#pragma endregion


#pragma endregion

#pragma region アニメーション

void Library::SetPointBoardAnimation
(
	const Vector2& leftUpPos,
	const Vector2& rightDownPos,
	const point& p,
	const int& num
)
{

}

//
//void Library::changeAnimation(vertex createNum, int maxWidth, int maxHeight, int animationNumX, int animationNumY)
//{
//	if (!createNum)return;
//	dx12->setAnimation(*createNum, maxWidth, maxHeight, animationNumX, animationNumY);
//}
//
//void Library::changeAnimation2(vertex vertexNum, heap dataNum, int startAreaX, int startAreaY, int endAreaX, int endAreaY)
//{
//	if (!vertexNum || !dataNum)return;
//	dx12->setAnimation2(*vertexNum, *dataNum, startAreaX, startAreaY, endAreaX, endAreaY);
//}

//void Library::spriteChangeAnimation(int* spriteNum, int maxWidth, int maxHeight, int animationNumX, int animationNumY)
//{
//	dx12->setSpriteAnimationVertex(*spriteNum, maxWidth, maxHeight, animationNumX, animationNumY);
//}
//
//void Library::spriteChangeAnimation2(int* spriteNum, int maxWidth, int maxHeight, int width, int height, int startAreaX, int startAreaY, int endAreaX, int endAreaY)
//{
//	dx12->setSpriteAnimationVertex2(*spriteNum,maxWidth,maxHeight,width,height, startAreaX, startAreaY, endAreaX, endAreaY);
//}

#pragma endregion

#pragma region 情報取得取得
std::vector<std::vector<Vector3>> Library::GetModelVerticesPosition(const ModelData& modelData)
{
	std::vector<std::vector<Vector3>>vector3VertexPos;
	std::vector<std::vector<DirectX::XMFLOAT3>> xmFloat3VertexPos;
	xmFloat3VertexPos = dx12->GetModelVerticesPosition(modelData.key);
	vector3VertexPos.resize(xmFloat3VertexPos.size());

	size_t num = xmFloat3VertexPos.size();
	size_t xmSize = 0;
	for (size_t i = 0; i < num; i++)
	{
		xmSize = xmFloat3VertexPos[i].size();
		vector3VertexPos[i].resize(xmSize);
		for(size_t j = 0; j < xmSize;j++)
		{
			vector3VertexPos[i][j] = xmFloat3VertexPos[i][j];
		}
	}

	return vector3VertexPos;


}

bool Library::OverrideWriteVertexPosition(std::vector<std::vector<Vector3>>vertPos, const ModelData& modelData)
{
	

	std::vector<std::vector<DirectX::XMFLOAT3>> kariXM;
	kariXM.resize(vertPos.size());
	size_t num = vertPos.size();
	for (size_t i = 0; i < num; i++)
	{
		kariXM[i].resize(vertPos[i].size());
	}

	num = kariXM.size();
	size_t num2 = 0;
	for (size_t i = 0; i < num; i++)
	{
		num2 = kariXM[i].size();
		for (size_t j = 0; j < num2; j++)
		{
			kariXM[i][j] = vertPos[i][j].ToXMFLOAT3();
			
		}
	}
	return dx12->OverrideWriteVertexPosition(kariXM, modelData.key);
}

#pragma region アニメーション


std::vector<Vector3> Library::GetBonePosition(const ModelData& modelData)
{
	std::vector<DirectX::XMFLOAT3>getVector = dx12->GetBonePosition(modelData.key);
	std::vector<Vector3>returnVector(getVector.size());

	int count = 0;
	for (auto& retV : returnVector)
	{
		retV = getVector[count];
		count++;
	}
	return returnVector;
}
#pragma endregion

#pragma region 当たり判定
void Library::GetModelBoxCollisionData
(
	const ModelData& modelData,
	Vector3* pointPosition,
	Vector3* minPosition,
	Vector3* maxPosition,
	Vector3* boxSize
)
{
	std::vector<std::vector<Vector3>> modelVertices;
	modelVertices = GetModelVerticesPosition(modelData);

	Vector3 min = 999999.0f;
	Vector3 max = -999999.0f;

	for(auto& v1 : modelVertices)
	{
		for(auto& v2 : v1)
		{
			if (min.x > v2.x)
				min.x = v2.x;
			if (min.y > v2.y)
				min.y = v2.y;
			if (min.z > v2.z)
				min.z = v2.z;

			if (max.x < v2.x)
				max.x = v2.x;
			if (max.y < v2.y)
				max.y = v2.y;
			if (max.z < v2.z)
				max.z = v2.z;
		}
	}

	if (minPosition)
		*minPosition = min;
	if (maxPosition)
		*maxPosition = max;
	if (pointPosition) 
		*pointPosition = (min + max) / 2;
	if (boxSize)
		*boxSize = max - min;
}
#pragma endregion


#pragma endregion

#pragma region 行列による変換
Vector3 Library::RotateVectorToCameraPosition(Vector3 eyeVelocity, bool flag)
{
	DirectX::XMFLOAT3 ret = dx12->MatchEyeVelocityToCamera({ eyeVelocity.x,eyeVelocity.y,eyeVelocity.z }, flag);


	return { ret.x,ret.y,ret.z };
}

Vector3 Library::GetRotateCameraPosition()
{
	DirectX::XMFLOAT3 pos = dx12->GetRotateCameraPosition();
	return { pos.x,pos.y,pos.z };
}

Vector3 Library::GetRotateCameraTarget()
{
	DirectX::XMFLOAT3 pos = dx12->GetRotateCameraTarget();
	return { pos.x,pos.y,pos.z };
}

#pragma endregion

#pragma region 文字表示
void Library::DrawsSpriteFontString(Vector2 position, Vector2 size, std::string text, font* spriteTextureNum)
{
	dx12->DrawSpriteFontString({ position.x,position.y }, { size.x,size.y }, text, *spriteTextureNum);
}
#pragma endregion


#pragma region 親子構造
void Library::SetParent(const ModelData& modelData, int number, const ModelData& parentmodelData, int parentNum)
{

	dx12->SetParent(modelData.key, number, parentmodelData.key, parentNum);
}

#pragma endregion

#pragma region 計算
void Library::CalculationNormal
(
	Vector3 pos1, Vector3 pos2, Vector3 pos3,
	Vector3& normal1, Vector3& normal2, Vector3& normal3
)
{
	DirectX::XMFLOAT3 normal[3];

	dx12->CalculationNormal
	(
		{ pos1.x,pos1.y,pos1.z },
		{ pos2.x,pos2.y,pos2.z },
		{ pos3.x,pos3.y,pos3.z },
		normal[0],
		normal[1],
		normal[2]
	);

	normal1.x = normal[0].x;
	normal1.y = normal[0].y;
	normal1.z = normal[0].z;

	normal2.x = normal[1].x;
	normal2.y = normal[1].y;
	normal2.z = normal[1].z;

	normal3.x = normal[2].x;
	normal3.y = normal[2].y;
	normal3.z = normal[2].z;

}

#pragma endregion

#pragma region サウンド
void Library::PlaySound(const char* path)
{
	audio.get()->PlayWave(path);
}

void Library::LoadSound(const char* path, std::string name, bool loop)
{
	audio.get()->LoadSound(path, name,loop);
}
void Library::PlayLoadSound(std::string name)
{
	audio.get()->PlayLoadSound(name);
}
void Library::StopLoadSound(std::string name, bool resetFlag)
{
	audio.get()->StopLoadSound(name, resetFlag);
}

#pragma endregion

