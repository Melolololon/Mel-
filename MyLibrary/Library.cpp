#include "Library.h"
#include"LibMath.h"
#include"XInputManager.h"
#pragma comment(lib,"winmm.lib")//timeGetTimeと
#include"ObjectManager.h"

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

void Library::initialize(int windowWidth, int windowHeight, const Color& screenColor, const wchar_t* windowName)
{
	dx12 = DirectX12::getInstance();
	createPolygon = CreatePolygon::getInstance();
	createPolygon->initialize(windowWidth, windowHeight);

	srand((unsigned int)time(NULL));
	count = 0;
	fps = 60;
	startProsessTime = timeGetTime();
	nowTime = timeGetTime();

#pragma region ウィンドウ処理

	//OutputDebugStringA("Hello,DirectX!!\n");

	w.cbSize = sizeof(WNDCLASSEX);
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

	ShowWindow(hwnd, SW_SHOW);



#pragma endregion

#pragma region DC

	//DCを取得
	hdc = GetDC(hwnd);
	//リフレッシュレートを取得
	refReat = GetDeviceCaps(hdc, VREFRESH);
#pragma endregion

#pragma region Input
	DirectInput::initialize(hwnd, windowWidth, windowHeight);
#pragma endregion


	audio = std::unique_ptr<Audio>(new Audio());

	dx12->setScreenColor(screenColor);
	dx12->initialize(hwnd, windowWidth, windowHeight);
	

	createPipelineCounter = dx12->getStartPipelineCreateNum();
	//スプライトフォント300を作るのでカウントを増やしておく
	createSpriteConter = 300;

	//べた塗用テクスチャを0にするために1にする(DrawBoxなどに使う)
	loadTextureCounter = 1;

	//modelDatas.reserve(99999);
}

void Library::roopStartProcess()
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

	dx12->preparationToDraw();

#pragma region Input

	DirectInput::update();
	XInputManager::update();
#pragma endregion

}

void Library::roopEndProcess()
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

	dx12->draw();

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

}

void Library::endFlagTrue()
{
	isDestroy = false;
	isEnd = true;
}

bool Library::getIsEnd()
{
	return isEnd;
}

void Library::end()
{
	dx12->end();

	DirectInput::release();

	if (!isDestroy)
	{
		DestroyWindow(hwnd);
	}

	UnregisterClass(w.lpszClassName, w.hInstance);
}

int Library::getRandomNumber(int number)
{
	return rand() % number;
}

float Library::getRandomNumberFloat(int number)
{
	return static_cast<float>((rand() % number));
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
HWND Library::getHWND()
{
	return hwnd;
}
#pragma endregion

Vector2 Library::getTextureSize(texture textureHandle)
{
	DirectX::XMFLOAT2 size = dx12->getTextureSize(textureHandle);
	return { size.x,size.y };
}

#pragma region パイプライン設定
void Library::setDespTestFlag(bool flag)
{
	//dx12->setDespTestFlag(flag);
}
#pragma endregion

#pragma region パイプライン作成
pipeline Library::createUserPipelineState(
	PipelineData pipelineData,
	ShaderData vShaderData,
	ShaderData gSyaderData,
	ShaderData pShaderData,
	bool useUserInputLayout
)
{
	bool result = dx12->createUserPipelineState
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

void Library::setPipeline(pipeline num)
{
	dx12->setPipelineNumber(num);
}


//void Library::setConstMapData(void** dataP, unsigned int dataSize)
//{
//	dx12->setConstMapData(dataP, dataSize);
//}

void Library::getMatrix(float matrix[4][4], const ModelData& modelData, int number)
{
	dx12->getMatrix(matrix, modelData.key, number);
}

void Library::getCameraMatrix(float matrix[4][4])
{
	dx12->getCameraMatrix(matrix);
}


void Library::setInputLayout(const std::vector<InputLayoutData>& inputLayoutData)
{
	dx12->setInputLayout(inputLayoutData);
}

void Library::deleteInputLayout()
{
	dx12->deleteInputLayout();
}



#pragma region ポストエフェクト
void Library::setPostEffectPipeline(const pipeline& num)
{
	dx12->setPostEffectPipeline(num);
}

pipeline Library::createUserPostEffectPipelineState
(
	const ShaderData& pShaderData
)
{
	createPostEffectPuiperineCounter++;
	dx12->createUserPostEffectPipelineState(pShaderData);
	return createPostEffectPuiperineCounter;
}
#pragma endregion


#pragma endregion


#pragma region 設定

void Library::setFramesPerSecond60(bool flag)
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
void  Library::loadOBJVertex
(
	const char* path,
	bool loadUV,
	bool loadNormal,
	std::string* materialFireName,
	ModelData& modelData
)
{
	if (!checkSetKeyName(modelData.key))return;


	modelData.type = dx12->loadOBJVertex
	(
		path,
		loadUV,
		loadNormal,
		materialFireName,
		modelData.key
	);
	


}
#pragma endregion


void Library::createPoint(int createNum, point* p)
{
	*p = new int(createPointCount);
	dx12->createPoint(createNum, *p);
	createPointCount++;
}

void Library::createBoard(const Vector2& size, ModelData& modelData)
{
	if (!checkSetKeyName(modelData.key))return;

	Vector2 boardSize = { size.x,size.y};

	boardSize.y *= -1;
	float width = boardSize.x;
	float height = boardSize.y;
	width /= 2;
	height /= 2;

	dx12->createPolygonData
	(
		createPolygon->setBoardPolygonVertex
		(
			{ 0 - width,boardSize.y - height,0 },//左下
			{ 0 - width,0 - height, 0 },//左上
			{ boardSize.x - width,boardSize.y - height,0 },//右下
			{ boardSize.x - width,0 - height,0 }//右上
		),
		createPolygon->setBoardPolygonIndex(),
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



void Library::createTriangularPyramid
(
	const float& r,
	const int& vertexNumber,
	const Vector3& centerPosition,
	const float& upVertex,
	ModelData& modelData
)
{
	if (!checkSetKeyName(modelData.key))return;
	
	dx12->createPolygonData
	(
		createPolygon->getTriangularPyramidVertex
		(
			r,
			vertexNumber,
			centerPosition.toXMFLOAT3(),
			upVertex
		),
		createPolygon->setTriangularPyramidIndex(vertexNumber),
		modelData.key
	);

	
	modelData.type = VERTEX_TYPE_NORMAL;
}


void Library::create3DBox(const Vector3& size,  ModelData& modelData)
{
	if (!checkSetKeyName(modelData.key))return;
	
	dx12->createPolygonData
	(
		createPolygon->getVertexMany3DBox({ size.x,size.y,size.z }),
		createPolygon->getVertexMany3DBoxIndex(),
		modelData.key
	);

	
	modelData.type = VERTEX_TYPE_NORMAL;
}

#pragma region ユーザー

void Library::createUserObject
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
		vertices[i].pos = vertexPos[i].toXMFLOAT3();
		vertices[i].uv = vertexUV[i].toXMFLOAT2();
	}

	dx12->createPolygonData
	(
		vertices,
		indices,
		modelData.key

	);
	
	
	modelData.type = VERTEX_TYPE_NORMAL;
}

void Library::createUserObject2
(
	void** vertexData, 
	UINT vertexDataSize,
	UINT vertexSumDataSize,
	std::vector<USHORT>&index,
	ModelData& modelData
)
{
	if (!checkSetKeyName(modelData.key))return;

	dx12->createUserPolygon
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

void Library::loadOBJMaterial
(
	std::string materialDirectoryPath, 
	std::string materialFileName, 
	int objectNum, 
	 ModelData& modelData
)
{
	if (!checkCreateVertexBuffer(modelData.type))return;
	

	dx12->loadOBJMaterial(materialDirectoryPath, materialFileName, false, modelData.key, objectNum, modelData.type);
	
}

void Library::loadObjMaterialUseUserData
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
	
	dx12->loadOBJMaterial(materialDirectoryPath, materialFileName, true, modelData.key, objectNum, modelData.type);
	
}

void Library::createHeapData
(
	const wchar_t* texturePath, 
	int objectNum,
	ModelData& modelData
)
{
	


	if (!checkCreateVertexBuffer(modelData.type))return;

	dx12->createHeapData(false, modelData.key, objectNum, texturePath, nullptr);
	
	
}

void Library::createHeapData2
(
	Color color, 
	int objectNum,
	ModelData& modelData
)
{

	if (!checkCreateVertexBuffer(modelData.type))return;

	dx12->createHeapData(false, modelData.key, objectNum, L"", &color);

}

void Library::createUserHeapData
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


	dx12->createHeapData(true, modelData.key, objectNum, texturePath, nullptr);

}

void Library::createUserHeapData2
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


	dx12->createHeapData(true, modelData.key, objectNum, L"", &color);

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

font Library::loadSpriteFont
(
	const wchar_t *const texturePath,
	const Vector2& lineNum
)
{
	loadFontTextureCounter++;
	dx12->loadSpriteFont(texturePath,lineNum.toXMFLOAT2());

	return loadFontTextureCounter - 1;

}

//テクスチャの番号を返す
texture Library::loadTexture(const wchar_t* texturePath)
{
	dx12->loadTexture(texturePath, { 0,0,0,0 });

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
void Library::createSprite(sprite* sprite)
{
	*sprite = new int(createSpriteConter);
	dx12->createSprite(*sprite, false);

	createSpriteConter++;
	//return createSpriteConter - 1;
}
#pragma endregion


#pragma endregion

#pragma region 描画

void Library::drawGraphic
(
	const ModelData& modelData,
	int number
)
{
	/*std::tuple<ModelData, int> mData = std::make_tuple(modelData, number);
	modelDatas.push_back(mData);*/

	dx12->map(modelData, number);
	dx12->setCmdList(modelData.key, number);
}

void Library::drawSprite
(
	const Vector2& position,
	const sprite& spriteNumber,
	const texture& textureNumber
)
{
	//dx12->spriteSetObjectPosition({ position.x,position.y }, *spriteNumber);
	dx12->spriteMap(position.toXMFLOAT2(), { 0,0 }, *spriteNumber, textureNumber);
	dx12->spriteSetCmdList(*spriteNumber, textureNumber);
}

void Library::drawSpriteAnimation
(
	const Vector2& position,
	const Vector2& maxSqare,
	const Vector2& currentNum,
	const sprite& spriteNumber,
	const texture& textureNumber
)
{
	//この順番じゃないとuvがちゃんとセットされない
	dx12->spriteMap({ position.x,position.y }, { 0,0 }, *spriteNumber, textureNumber);
	dx12->setSpriteAnimationVertex(*spriteNumber, textureNumber, maxSqare.toXMFLOAT2(), currentNum.toXMFLOAT2());
	dx12->spriteSetCmdList(*spriteNumber, textureNumber);
}

void Library::drawSpriteAnimation2
(
	const Vector2& position,
	const Vector2& currentStartNum,
	const Vector2& currentEndNum,
	const sprite& spriteNumber,
	const texture& textureNumber
)
{
	//dx12->spriteMap({ position.x,position.y }, { 0,0 }, *spriteNumber, *textureNumber);
	dx12->setSpriteAnimationVertex2
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

	dx12->spriteSetCmdList(*spriteNumber, textureNumber);
}

void Library::drawPointTexture(Vector3 pos, point point, texture texture, int num)
{
	dx12->pointSetCmdList({ pos.x,pos.y,pos.z }, *point, texture, num);
}

#pragma region べた塗
void Library::drawBox
(
	const Vector2& position,
	const Vector2& size,
	const Color& color,
	const sprite& spriteHandle
)
{
	setSpriteAddColor(color, spriteHandle);
	dx12->spriteMap(position.toXMFLOAT2(), size.toXMFLOAT2(), *spriteHandle, 0);
	dx12->spriteSetCmdList(*spriteHandle, 0);
}
#pragma endregion


#pragma endregion

#pragma region 削除
void Library::deleteObject3DData(const ModelData& modelData)
{
	dx12->deletePolygonData(modelData.key);
	dx12->deleteHeapData(modelData.key);
}


void Library::deleteSprite(sprite sprite)
{
	dx12->deleteSprite(*sprite);
}
#pragma endregion

#pragma region 見た目


#pragma region スムースシェーディング
void Library::setSmoothingFlag(bool flag)
{
	dx12->setSmoothingFlag(flag);
}
#pragma endregion

void Library::setMulColor(Color color,const ModelData& modelData, int number)
{
	
	dx12->setMulColor(color, modelData.key, number);
}
void Library::setAddColor(Color color,const ModelData& modelData, int number)
{

	dx12->setAddColor(color, modelData.key, number);
}
void Library::setSubColor(Color color,const ModelData& modelData, int number)
{
	
	dx12->setSubColor(color, modelData.key, number);
}

void Library::setIsPlane(bool flag)
{
	dx12->setIsPlane(flag);
}

void Library::setIsBillboard(bool x, bool y, bool z)
{
	dx12->setIsBillboard(x, y, z);
}

void Library::setSpriteMulColor(Color color, sprite spriteNum)
{
	dx12->setSpriteMulColor(color, *spriteNum);
}
void Library::setSpriteAddColor(Color color, sprite spriteNum)
{
	dx12->setSpriteAddColor(color, *spriteNum);
}
void Library::setSpriteSubColor(Color color, sprite spriteNum)
{
	dx12->setSpriteSubColor(color, *spriteNum);
}


void Library::setPointScale(Vector2 scale, point pointNum, int num)
{
	dx12->setPointScale({ scale.x,scale.y }, *pointNum, num);
}
#pragma endregion

#pragma region カメラ
void Library::setCameraMatrixPoint(Vector3 position, Vector3 target, Vector3 up)
{
	dx12->setCameraDataMatrixPoint(position, target, up);
}

void Library::setCamera(Vector3 position, Vector3 target, Vector3 up)
{
	dx12->setCameraData(position, target, up);


}

void Library::setCameraVelocity(Vector3 eyeVelocity, Vector3 targetVelocity)
{
	dx12->setCameraVelocity({ eyeVelocity.x,eyeVelocity.y,eyeVelocity.z }, { targetVelocity.x,targetVelocity.y,targetVelocity.z });
}

void Library::setCameraAngle(Vector3 eyeAngle, Vector3 targetAngle, Vector3 upAngle)
{
	dx12->setCameraAngre({ eyeAngle.x,eyeAngle.y,eyeAngle.z }, { targetAngle.x ,targetAngle.y,targetAngle.z }, { upAngle.x,upAngle.y,upAngle.z });
}

void Library::setCameraNearAndFar(float nearNum, float farNum)
{
	dx12->setNearAndFar(nearNum, farNum);
}

#pragma endregion

#pragma region ライト

void Library::setLightVector(Vector3 vector)
{
	Vector3 v = vector3Normalize(vector);
	dx12->setLightVector({ v.x,v.y,v.z });
}
//

void Library::setLightColor(Color lightColor)
{
	dx12->setLightColor(lightColor);
}
#pragma endregion

#pragma region 操作
void Library::setPosition(Vector3 position, const ModelData& modelData , int number)
{
	
	dx12->setObjectPosition({ position.x,position.y,position.z }, modelData.key, number);
}


void Library::setAngle(Vector3 angle, const ModelData& modelData,  int number)
{

	dx12->setObjectAngle({ angle.x,angle.y,angle.z }, modelData.key, number);
}

void Library::setScale(Vector3 scale, const ModelData& modelData , int number)
{
	dx12->setObjectScale({ scale.x,scale.y,scale.z }, modelData.key, number);
}

void Library::setPushPorigonNumber(float ex, const ModelData& modelData, int number)
{
	dx12->setObjectPushNum(ex, modelData.key, number);
}


void Library::setSpritePosition(Vector2 position, sprite sptiteNumber)
{
	dx12->spriteSetObjectPosition({ position.x,position.y }, *sptiteNumber);
}
void Library::setSpriteScale(Vector2 scale, sprite sptiteNumber)
{
	dx12->spriteSetObjectScale({ scale.x,scale.y }, *sptiteNumber);
}
void Library::setSpriteAngle(float angle, sprite spriteNumber)
{
	dx12->spriteSetObjectAngle(angle, *spriteNumber);
}

void changeSpriteSize(Vector2 size, int *spriteData)
{

}



void Library::setPointMulColor(Color color, point pointNum, int num)
{
	dx12->setPointMulColor(color, *pointNum, num);
}



#pragma region ポストエフェクト

void Library::setRenderTargetPosition(const Vector3& pos, const int& rtNum) 
{
	dx12->setRenderTargerPosition({ pos .x,pos .y,pos .z}, rtNum);
}

void Library::setRenderTargetAngle(const Vector3& angle, const int& rtNum)
{
	dx12->setRenderTargetAngle({ angle .x,angle .y,angle .z}, rtNum);
}

void Library::setRenderTargetScale(const Vector3& scale, const int& rtNum)
{
	dx12->setRenderTargetScale({ scale.x,scale.y,scale.z }, rtNum);
}

void Library::setPostEffectCameraFlag(const bool& flag, const int& rtNum)
{
	dx12->setPostEffectCameraFlag(true, rtNum);
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

void Library::setOBJBoneMoveVector
(
	const Vector3& vector, 
	const UINT& boneNum,
	const ModelData& modelData,
	const UINT& objectNum
)
{
	if (modelData.type != VertexType::VERTEX_TYPE_OBJ_ANIMATION)
		return;

	dx12->setOBJBoneMoveVector(vector.toXMFLOAT3(), boneNum, modelData.key, objectNum);

}

void Library::setOBJBoneScale
(
	const Vector3& scale,
	const UINT& boneNum,
	const ModelData& modelData,
	const UINT& objectNum
)
{
	if (modelData.type != VertexType::VERTEX_TYPE_OBJ_ANIMATION)
		return;
	dx12->setOBJBoneScale(scale.toXMFLOAT3(), boneNum, modelData.key, objectNum);
}

void Library::setOBJBoneAngle
(
	const Vector3& angle, 
	const UINT& boneNum,
	const ModelData& modelData,
	const UINT& objectNum
)
{
	if (modelData.type != VertexType::VERTEX_TYPE_OBJ_ANIMATION)
		return;
	dx12->setOBJBoneAngle(angle.toXMFLOAT3(), boneNum, modelData.key, objectNum);
}

void Library::setParentOBJBone
(
	const UINT& boneNum,
	const UINT& parentBoneNum,
	const ModelData& modelData
)
{
	if (modelData.type != VertexType::VERTEX_TYPE_OBJ_ANIMATION)
		return;

	dx12->setParentOBJBone
	(
		boneNum, 
		parentBoneNum,
		modelData.key
	);
}

void Library::setParentOBJBoneScaleImpact
(
	const UINT& boneNum,
	const Vector3& scaleImpact,
	const ModelData& modelData
)
{
	if (modelData.type != VertexType::VERTEX_TYPE_OBJ_ANIMATION)
		return;
	dx12->setParentOBJBoneScaleImpact
	(
		boneNum,
		scaleImpact.toXMFLOAT3(),
		modelData.key
	);
}

void Library::setParentOBJBoneAngleImpact
(
	const UINT& boneNum,
	const Vector3& angleImpact,
	const ModelData& modelData
)
{
	if (modelData.type != VertexType::VERTEX_TYPE_OBJ_ANIMATION)
		return;
	dx12->setParentOBJBoneAngleImpact
	(
		boneNum,
		angleImpact.toXMFLOAT3(),
		modelData.key
	);
}

void Library::setParentOBJBoneMoveVectorImpact
(
	const UINT& boneNum,
	const Vector3& moveVectorImpact,
	const ModelData& modelData
)
{
	if (modelData.type != VertexType::VERTEX_TYPE_OBJ_ANIMATION)
		return;
	dx12->setParentOBJBoneMoveVectorImpact
	(
		boneNum,
		moveVectorImpact.toXMFLOAT3(),
		modelData.key
	);
}

#pragma endregion


#pragma endregion

#pragma region アニメーション
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

#pragma region 頂点座標取得など
std::vector<std::vector<Vector3>> Library::getVertexPosition(const ModelData& modelData)
{
	std::vector<std::vector<Vector3>>vector3VertexPos;
	std::vector<std::vector<DirectX::XMFLOAT3>> xmFloat3VertexPos;
	xmFloat3VertexPos = dx12->getObjectVertexPosition(modelData.key);
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

bool Library::overrideWriteVertexPosition(std::vector<std::vector<Vector3>>vertPos, const ModelData& modelData)
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
			kariXM[i][j] = vertPos[i][j].toXMFLOAT3();
			
		}
	}
	return dx12->overrideWriteVertexPosition(kariXM, modelData.key);
}

#pragma region アニメーション


std::vector<Vector3> Library::getBonePosition(const ModelData& modelData)
{
	std::vector<DirectX::XMFLOAT3>getVector = dx12->getBonePosition(modelData.key);
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


#pragma endregion

#pragma region 行列による変換
Vector3 Library::rotateVectorToCameraPosition(Vector3 eyeVelocity, bool flag)
{
	DirectX::XMFLOAT3 ret = dx12->matchEyeVelocityToCamera({ eyeVelocity.x,eyeVelocity.y,eyeVelocity.z }, flag);


	return { ret.x,ret.y,ret.z };
}

Vector3 Library::getRotateCameraPosition()
{
	DirectX::XMFLOAT3 pos = dx12->getRotateCameraPosition();
	return { pos.x,pos.y,pos.z };
}

Vector3 Library::getRotateCameraTarget()
{
	DirectX::XMFLOAT3 pos = dx12->getRotateCameraTarget();
	return { pos.x,pos.y,pos.z };
}

#pragma endregion

#pragma region 文字表示
void Library::drawsSpriteFontString(Vector2 position, Vector2 size, std::string text, font* spriteTextureNum)
{
	dx12->drawSpriteFontString({ position.x,position.y }, { size.x,size.y }, text, *spriteTextureNum);
}
#pragma endregion

#pragma region 読み込み
bool Library::loadTextIntVector(const char* path, std::vector<std::vector<int>>& vector)
{

	if (!TextLoader::loadText(path))return false;
	TextLoader::getIntVector(vector);
	return true;
}
#pragma endregion

#pragma region 親子構造
void Library::setParent(const ModelData& modelData, int number, const ModelData& parentmodelData, int parentNum)
{

	dx12->setParent(modelData.key, number, parentmodelData.key, parentNum);
}

#pragma endregion

#pragma region 計算
void Library::calculationNormal
(
	Vector3 pos1, Vector3 pos2, Vector3 pos3,
	Vector3& normal1, Vector3& normal2, Vector3& normal3
)
{
	DirectX::XMFLOAT3 normal[3];

	dx12->calculationNormal
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
void Library::playSound(const char* path)
{
	audio.get()->playWave(path);
}

void Library::loadSound(const char* path, std::string name, bool loop)
{
	audio.get()->loadSound(path, name,loop);
}
void Library::playLoadSound(std::string name)
{
	audio.get()->playLoadSound(name);
}
void Library::stopLoadSound(std::string name, bool resetFlag)
{
	audio.get()->stopLoadSound(name, resetFlag);
}

#pragma endregion

