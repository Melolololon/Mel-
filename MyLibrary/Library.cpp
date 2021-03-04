#include "Library.h"
#include"LibMath.h"
#include"XInputManager.h"
#pragma comment(lib,"winmm.lib")//timeGetTimeと
#include"ObjectManager.h"

std::unique_ptr<Audio> Library::audio;
DirectX12* Library::directx12;
WNDCLASSEX Library::w;
MSG  Library::msg;
HWND  Library::hwnd;
bool Library::isEnd;

bool Library::isDestroy;
int Library::count;

int Library::createPointCount;

bool Library::isSetFPS60;
unsigned int Library::fps;

int Library::loadFontTextureCounter;
int Library::loadTextureCounter;
int Library::createSpriteConter;

int Library::createPipelineCounter;
int Library::createPostEffectPuiperineCounter;

HDC Library::hdc;
int Library::refReat;
DWORD Library::startProsessTime;
DWORD Library::nowTime;

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

	audio = std::unique_ptr<Audio>(new Audio());

	directx12 = new DirectX12(hwnd, windowWidth, windowHeight);
	directx12->setScreenColor(screenColor);
	directx12->initialize();

	createPipelineCounter = directx12->getStartPipelineCreateNum();
	//スプライトフォント300を作るのでカウントを増やしておく
	createSpriteConter = 300;

	//べた塗用テクスチャを0にするため(DrawBoxなどに使う)
	loadTextureCounter = 1;

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

	directx12->preparationToDraw();

	XInputManager::update();
}

void Library::roopEndProcess()
{
	directx12->draw();

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
	delete directx12;

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
	return (float)(rand() % number);
}

#pragma region WinAPI関係
HWND Library::getHWND()
{
	return hwnd;
}
#pragma endregion

Vector2 Library::getTextureSize(texture textureHandle)
{
	DirectX::XMFLOAT2 size = directx12->getTextureSize(textureHandle);
	return { size.x,size.y };
}

#pragma region パイプライン設定
void Library::setDespTestFlag(bool flag)
{
	//directx12->setDespTestFlag(flag);
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
	bool result = directx12->createUserPipelineState
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
	directx12->setPipelineNumber(num);
}


//void Library::setConstMapData(void** dataP, unsigned int dataSize)
//{
//	directx12->setConstMapData(dataP, dataSize);
//}

void Library::getMatrix(float matrix[4][4], int* heapNum, int number)
{
	directx12->getMatrix(matrix, *heapNum, number);
}

void Library::getCameraMatrix(float matrix[4][4])
{
	directx12->getCameraMatrix(matrix);
}


void Library::setInputLayout(const char* semantics, int num)
{
	directx12->setInputLayout(semantics, num);
}

void Library::deleteInputLayout()
{
	directx12->deleteInputLayout();
}



#pragma region ポストエフェクト
void Library::setPostEffectPipeline(const pipeline& num)
{
	directx12->setPostEffectPipeline(num);
}

pipeline Library::createUserPostEffectPipelineState
(
	const ShaderData& pShaderData
)
{
	createPostEffectPuiperineCounter++;
	directx12->createUserPostEffectPipelineState(pShaderData);
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
//	directx12->setScreenColor(color);
//}

#pragma endregion

#pragma region 生成


#pragma region 頂点_インデックスバッファ作成

#pragma region モデル読み込み
void  Library::loadOBJVertex(const char* path, bool loadUV, bool loadNormal, std::string* materialFireName, vertex* p)
{
	PolyData pData;

	*p = new int(directx12->getCreateNumber().polyNum);
	pData.sikibetuNumP = *p;
	pData.dimention = dimention3D;

	directx12->loadOBJVertex(path, loadUV, loadNormal, materialFireName, pData);

}
#pragma endregion


void Library::createPoint(int createNum, point* p)
{
	*p = new int(createPointCount);
	directx12->createPoint(createNum, *p);
	createPointCount++;
}

void Library::createBoard(Vector2 size, int dimention, vertex* p)
{
	PolyData pData;

	pData.pos1 = { size.x,size.y,0 };
	pData.katatiNum = 1;


	//pData.sikibetuNum = createPolygonNumber;

	*p = new int(directx12->getCreateNumber().polyNum);
	pData.sikibetuNumP = *p;

	pData.dimention = dimention;
	directx12->createPolygonData(pData);

}

void Library::createCircle(float r, int dimention, vertex* p)
{
	PolyData pData;

	pData.fNum1 = r;
	pData.katatiNum = 2;

	*p = new int(directx12->getCreateNumber().polyNum);
	pData.sikibetuNumP = *p;
	pData.dimention = dimention;
	directx12->createPolygonData(pData);

}


void Library::create3DBox(Vector3 size, vertex* p)
{
	PolyData pData;

	pData.pos1 = { size.x,size.y,size.z };
	pData.katatiNum = 10;

	//pData.sikibetuNum = createPolygonNumber;

	*p = new int(directx12->getCreateNumber().polyNum);
	pData.sikibetuNumP = *p;
	pData.dimention = dimention3D;

	directx12->createPolygonData(pData);
}



void Library::createTriangularPyramid(float r, int vertexNumber, Vector3 centerPosition, float upVertex, vertex* p)
{
	PolyData pData;

	pData.fNum1 = r;
	pData.fNum2 = upVertex;
	pData.num1 = vertexNumber;
	pData.pos1 = centerPosition;
	pData.katatiNum = 11;

	//pData.sikibetuNum = createPolygonNumber;

	*p = new int(directx12->getCreateNumber().polyNum);
	pData.sikibetuNumP = *p;
	pData.dimention = dimention3D;
	directx12->createPolygonData(pData);

}


void Library::createManyVertex3DBox(Vector3 size, vertex* p)
{
	PolyData pData;

	pData.pos1 = { size.x,size.y,size.z };
	pData.katatiNum = 100;


	*p = new int(directx12->getCreateNumber().polyNum);
	pData.sikibetuNumP = *p;
	pData.dimention = dimention3D;

	directx12->createPolygonData(pData);


}

#pragma region ユーザー

void Library::createUserObject(std::vector<Vector3>& vertexPos, std::vector<Vector2>& vertexUV, std::vector<unsigned short>& index, vertex* p)
{
	directx12->addUserVertex(vertexPos, vertexUV);
	directx12->addUserIndex(index);

	PolyData pData;
	pData.katatiNum = -1;

	*p = new int(directx12->getCreateNumber().polyNum);
	pData.sikibetuNumP = *p;
	pData.dimention = dimention3D;

	directx12->createPolygonData(pData);
}

void Library::createUserObject2(void** vertexData, unsigned int vertexDataSize, unsigned int vertexSumDataSize, std::vector<unsigned short>&index, vertex* p)
{
	PolyData pData;
	*p = new int(directx12->getCreateNumber().polyNum);
	pData.sikibetuNumP = *p;
	pData.dimention = dimention3D;


	//クリエイトする!!
	//クリエイトする関数作ってー
	directx12->createUserPolygon(vertexData, vertexDataSize, vertexSumDataSize, index, pData);
}
#pragma endregion



#pragma endregion

#pragma region ヒープ作成

void Library::loadOBJMaterial(std::string materialDirectoryPath, std::string materialFileName, int objectNum, heap* heapP)
{
	HeapData hData;
	hData.objectNum = objectNum;
	*heapP = new int(directx12->getCreateNumber().despNum);
	hData.sikibetuNumP = *heapP;

	directx12->loadOBJMaterial(materialDirectoryPath, materialFileName, hData, false);
}

void Library::loadObjMaterialUseUserData
(
	std::string materialDirectoryPath,
	std::string materialFileName,
	int objectNum,
	void** dataP,
	unsigned int dataSize,
	heap* heapP
)
{
	HeapData hData;
	hData.objectNum = objectNum;
	directx12->setConstMapData(dataP, dataSize);
	*heapP = new int(directx12->getCreateNumber().despNum);
	hData.sikibetuNumP = *heapP;

	directx12->loadOBJMaterial(materialDirectoryPath, materialFileName, hData, true);
}

void Library::createHeapData(const wchar_t* texturePath, int objectNum, heap* p)
{
	//テクスチャかべた塗かの識別方法も考える
	HeapData dData;
	dData.path = texturePath;
	dData.objectNum = objectNum;

	*p = new int(directx12->getCreateNumber().despNum);
	dData.sikibetuNumP = *p;
	directx12->createHeapData(dData, false);


}

void Library::createHeapData2(Color color, int objectNum, heap* p)
{
	HeapData dData;
	dData.path = L"";

	dData.color = color;
	dData.objectNum = objectNum;


	*p = new int(directx12->getCreateNumber().despNum);
	dData.sikibetuNumP = *p;
	directx12->createHeapData(dData, false);


}

void Library::createUserHeapData(const wchar_t* texturePath, int objectNum, void** dataP, unsigned int dataSize, heap* p)
{
	HeapData dData;
	dData.path = texturePath;

	dData.objectNum = objectNum;

	directx12->setConstMapData(dataP, dataSize);

	*p = new int(directx12->getCreateNumber().despNum);
	dData.sikibetuNumP = *p;
	directx12->createHeapData(dData, true);
}

#pragma endregion

#pragma region 同時作成
void Library::loadOBJ(const char* path, std::string materialDirectoryPath, bool loadUV, bool loadNormal, int objectNum, vertex* vertP, heap* heapP)
{
	//頂点読み込み時にファイル名取得して、heap作成時にマテリアルを読み込むようにする
	//マテリアル名は、loadしたら受け取れるようにする
	//同じような処理(ヒープ作成など)は関数化する
	PolyData pData;
	*vertP = new int(directx12->getCreateNumber().polyNum);
	pData.sikibetuNumP = *vertP;
	pData.dimention = dimention3D;

	HeapData hData;
	hData.objectNum = objectNum;
	*heapP = new int(directx12->getCreateNumber().despNum);
	hData.sikibetuNumP = *heapP
		;
	directx12->loadOBJ
	(
		path,
		materialDirectoryPath,
		loadUV,
		loadNormal,
		pData,
		hData
	);
}
#pragma endregion


#pragma region スプライト

font Library::loadSpriteFont(const wchar_t* texturePath, Vector2 lineNum, Vector2 fontSize)
{
	loadFontTextureCounter++;
	directx12->loadSpriteFont(texturePath, { lineNum.x,lineNum.y }, { fontSize.x,fontSize.y });

	return loadFontTextureCounter - 1;

}

//テクスチャの番号を返す
texture Library::loadTexture(const wchar_t* texturePath)
{
	directx12->loadTexture(texturePath, { 0,0,0,0 });

	loadTextureCounter++;
	return loadTextureCounter - 1;
}
//
//int Library::createSpriteTexture(Color color)
//{
//	directx12->loadTexture(L"", color);
//
//	loadTextureCounter++;
//	return loadTextureCounter - 1;
//}

//スプライトの識別番号を返す
void Library::createSprite(sprite* sprite)
{
	*sprite = new int(createSpriteConter);
	directx12->createSprite(*sprite, false);

	createSpriteConter++;
	//return createSpriteConter - 1;
}
#pragma endregion


#pragma endregion

#pragma region 描画

void Library::drawGraphic(vertex polygonVertexNumber, heap polygonDataNumber, int number)
{
	if (!polygonVertexNumber || !polygonDataNumber)return;
	directx12->map(*polygonVertexNumber, *polygonDataNumber, number);
	directx12->setCmdList(*polygonVertexNumber, *polygonDataNumber, number);
}

void Library::drawSprite(Vector2 position, sprite spriteNumber, texture* textureNumber)
{
	//directx12->spriteSetObjectPosition({ position.x,position.y }, *spriteNumber);
	directx12->spriteMap({ position.x,position.y }, { 0,0 }, *spriteNumber, *textureNumber);
	directx12->spriteSetCmdList(*spriteNumber, *textureNumber);
}

void Library::drawSpriteAnimation(Vector2 position, Vector2 maxSqare, Vector2 currentNum, sprite spriteNumber, texture* textureNumber)
{
	//この順番じゃないとuvがちゃんとセットされない
	directx12->spriteMap({ position.x,position.y }, { 0,0 }, *spriteNumber, *textureNumber);
	directx12->setSpriteAnimationVertex(*spriteNumber, *textureNumber, maxSqare.x, maxSqare.y, currentNum.x, currentNum.y);
	directx12->spriteSetCmdList(*spriteNumber, *textureNumber);
}

void Library::drawSpriteAnimation2(Vector2 position, Vector2 currentStartNum, Vector2 currentEndNum, sprite spriteNumber, texture* textureNumber)
{
	//directx12->spriteMap({ position.x,position.y }, { 0,0 }, *spriteNumber, *textureNumber);
	directx12->setSpriteAnimationVertex2
	(
		*spriteNumber,
		*textureNumber,
		position.x,
		position.y,
		currentEndNum.x - currentStartNum.x,
		currentEndNum.y - currentStartNum.y,
		currentStartNum.x,
		currentStartNum.y,
		currentEndNum.x,
		currentEndNum.y
	);

	directx12->spriteSetCmdList(*spriteNumber, *textureNumber);
}

void Library::drawPointAndTexture(Vector3 pos, point point, texture texture, int num)
{
	directx12->pointSetCmdList({ pos.x,pos.y,pos.z }, *point, texture, num);
}

#pragma region べた塗
void Library::drawBox(const Vector2 position, const Vector2& size, const Color& color, sprite spriteHandle)
{
	setSpriteAddColor(color, spriteHandle);
	directx12->spriteMap({ position.x,position.y }, { size.x,size.y }, *spriteHandle, 0);
	directx12->spriteSetCmdList(*spriteHandle, 0);
}
#pragma endregion


#pragma endregion

#pragma region 削除
void Library::deleteVertexData(vertex polygonVertexNumber)
{
	if (!polygonVertexNumber)return;
	directx12->deletePolygonData(*polygonVertexNumber);
}

void Library::deleteHeapData(heap polygonDataNumber)
{
	if (!polygonDataNumber)return;
	directx12->deleteHeapData(*polygonDataNumber);
}


void Library::deleteSprite(sprite sprite)
{
	directx12->deleteSprite(*sprite);
}
#pragma endregion

#pragma region 見た目


#pragma region スムースシェーディング
void Library::setSmoothingFlag(bool flag)
{
	directx12->setSmoothingFlag(flag);
}
#pragma endregion

void Library::setMulColor(Color color, heap polygonDataNum, int number)
{
	if (!polygonDataNum)return;
	directx12->setMulColor(color, *polygonDataNum, number);
}
void Library::setAddColor(Color color, heap polygonDataNum, int number)
{
	if (!polygonDataNum)return;
	directx12->setAddColor(color, *polygonDataNum, number);
}
void Library::setSubColor(Color color, heap polygonDataNum, int number)
{
	if (!polygonDataNum)return;
	directx12->setSubColor(color, *polygonDataNum, number);
}

void Library::setIsPlane(bool flag)
{
	directx12->setIsPlane(flag);
}

void Library::setIsBillboard(bool x, bool y, bool z)
{
	directx12->setIsBillboard(x, y, z);
}

void Library::setSpriteMulColor(Color color, sprite spriteNum)
{
	directx12->setSpriteMulColor(color, *spriteNum);
}
void Library::setSpriteAddColor(Color color, sprite spriteNum)
{
	directx12->setSpriteAddColor(color, *spriteNum);
}
void Library::setSpriteSubColor(Color color, sprite spriteNum)
{
	directx12->setSpriteSubColor(color, *spriteNum);
}


void Library::setPointScale(Vector2 scale, point pointNum, int num)
{
	directx12->setPointScale({ scale.x,scale.y }, *pointNum, num);
}
#pragma endregion

#pragma region カメラ
void Library::setCameraMatrixPoint(Vector3 position, Vector3 target, Vector3 up)
{
	directx12->setCameraDataMatrixPoint(position, target, up);
}

void Library::setCamera(Vector3 position, Vector3 target, Vector3 up)
{
	directx12->setCameraData(position, target, up);


}

void Library::setCameraVelocity(Vector3 eyeVelocity, Vector3 targetVelocity)
{
	directx12->setCameraVelocity({ eyeVelocity.x,eyeVelocity.y,eyeVelocity.z }, { targetVelocity.x,targetVelocity.y,targetVelocity.z });
}

void Library::setCameraAngle(Vector3 eyeAngle, Vector3 targetAngle, Vector3 upAngle)
{
	directx12->setCameraAngre({ eyeAngle.x,eyeAngle.y,eyeAngle.z }, { targetAngle.x ,targetAngle.y,targetAngle.z }, { upAngle.x,upAngle.y,upAngle.z });
}

void Library::setCameraNearAndFar(float nearNum, float farNum)
{
	directx12->setNearAndFar(nearNum, farNum);
}

#pragma endregion

#pragma region ライト

void Library::setLightVector(Vector3 vector)
{
	Vector3 v = Vector3::normalize(vector);
	directx12->setLightVector({ v.x,v.y,v.z });
}
//

void Library::setLightColor(Color lightColor)
{
	directx12->setLightColor(lightColor);
}
#pragma endregion

#pragma region 操作
void Library::setPosition(Vector3 position, heap dataNum, int number)
{
	if (!dataNum)return;
	directx12->setObjectPosition({ position.x,position.y,position.z }, *dataNum, number);
}


void Library::setAngle(Vector3 angle, heap dataNum, int number)
{
	if (!dataNum)return;
	directx12->setObjectAngle({ angle.x,angle.y,angle.z }, *dataNum, number);
}

void Library::setScale(Vector3 scale, heap dataNum, int number)
{
	if (!dataNum)return;
	directx12->setObjectScale({ scale.x,scale.y,scale.z }, *dataNum, number);
}

void Library::pushPolygon(float ex, heap polygonDataNumber, int number)
{
	if (!polygonDataNumber)return;
	directx12->setObjectEX(ex, *polygonDataNumber, number);
}


void Library::setSpritePosition(Vector2 position, sprite sptiteNumber)
{
	directx12->spriteSetObjectPosition({ position.x,position.y }, *sptiteNumber);
}
void Library::setSpriteScale(Vector2 scale, sprite sptiteNumber)
{
	directx12->spriteSetObjectScale({ scale.x,scale.y }, *sptiteNumber);
}
void Library::setSpriteAngle(float angle, sprite spriteNumber)
{
	directx12->spriteSetObjectAngle(angle, *spriteNumber);
}

void changeSpriteSize(Vector2 size, int *spriteData)
{

}



void Library::setPointMulColor(Color color, point pointNum, int num)
{
	directx12->setPointMulColor(color, *pointNum, num);
}



#pragma region ポストエフェクト

void Library::setRenderTargetPosition(const Vector3& pos, const int& rtNum) 
{
	directx12->setRenderTargerPosition({ pos .x,pos .y,pos .z}, rtNum);
}

void Library::setRenderTargetAngle(const Vector3& angle, const int& rtNum)
{
	directx12->setRenderTargetAngle({ angle .x,angle .y,angle .z}, rtNum);
}

void Library::setRenderTargetScale(const Vector3& scale, const int& rtNum)
{
	directx12->setRenderTargetScale({ scale.x,scale.y,scale.z }, rtNum);
}

void Library::setPostEffectCameraFlag(const bool& flag, const int& rtNum)
{
	directx12->setPostEffectCameraFlag(true, rtNum);
}
#pragma endregion

#pragma endregion

#pragma region アニメーション

void Library::changeAnimation(vertex createNum, int maxWidth, int maxHeight, int animationNumX, int animationNumY)
{
	if (!createNum)return;
	directx12->setAnimation(*createNum, maxWidth, maxHeight, animationNumX, animationNumY);
}

void Library::changeAnimation2(vertex vertexNum, heap dataNum, int startAreaX, int startAreaY, int endAreaX, int endAreaY)
{
	if (!vertexNum || !dataNum)return;
	directx12->setAnimation2(*vertexNum, *dataNum, startAreaX, startAreaY, endAreaX, endAreaY);
}

//void Library::spriteChangeAnimation(int* spriteNum, int maxWidth, int maxHeight, int animationNumX, int animationNumY)
//{
//	directx12->setSpriteAnimationVertex(*spriteNum, maxWidth, maxHeight, animationNumX, animationNumY);
//}
//
//void Library::spriteChangeAnimation2(int* spriteNum, int maxWidth, int maxHeight, int width, int height, int startAreaX, int startAreaY, int endAreaX, int endAreaY)
//{
//	directx12->setSpriteAnimationVertex2(*spriteNum,maxWidth,maxHeight,width,height, startAreaX, startAreaY, endAreaX, endAreaY);
//}

#pragma endregion

#pragma region 頂点座標取得など
std::vector<std::vector<Vector3>> Library::getVertexPosition(int* vertData)
{
	if (!vertData)
	{
		std::vector<std::vector<Vector3>> ret;
		return ret;
	}

	std::vector<std::vector<Vector3>>kariV;
	std::vector<std::vector<DirectX::XMFLOAT3>> kariXM;
	kariXM = directx12->getObjectVertexPosition(*vertData);
	kariV.resize(kariXM.size());

	int num = kariV.size();
	for (int i = 0; i < num;i++)
	{
		kariV[i].resize(kariXM[i].size());
		
	}

	num = kariXM.size();
	int num2 = 0;
	for (int i = 0; i < num; i++)
	{
		num2 = kariXM[i].size();
		for (int j = 0; j < num2; j++)
		{
			kariV[i][j].x = kariXM[i][j].x;
			kariV[i][j].y = kariXM[i][j].y;
			kariV[i][j].z = kariXM[i][j].z;
		}
	}
	return kariV;
}

bool Library::overrideWriteVertexPosition(std::vector<std::vector<Vector3>>vertPos, int* vertNum)
{
	if (!vertNum)return false;

	std::vector<std::vector<DirectX::XMFLOAT3>> kariXM;
	kariXM.resize(vertPos.size());
	int num = vertPos.size();
	for (int i = 0; i < num; i++)
	{
		kariXM[i].resize(vertPos[i].size());
	}

	num = kariXM.size();
	int num2 = 0;
	for (int i = 0; i < num; i++)
	{
		num2 = kariXM[i].size();
		for (int j = 0; j < num2; j++) 
		{
			kariXM[i][j].x = vertPos[i][j].x;
			kariXM[i][j].y = vertPos[i][j].y;
			kariXM[i][j].z = vertPos[i][j].z;
		}
	}
	return directx12->overrideWriteVertexPosition(kariXM, *vertNum);
}
#pragma endregion

#pragma region 行列による変換
Vector3 Library::rotateVectorToCameraPosition(Vector3 eyeVelocity, bool flag)
{
	DirectX::XMFLOAT3 ret = directx12->matchEyeVelocityToCamera({ eyeVelocity.x,eyeVelocity.y,eyeVelocity.z }, flag);


	return { ret.x,ret.y,ret.z };
}

Vector3 Library::getRotateCameraPosition()
{
	DirectX::XMFLOAT3 pos = directx12->getRotateCameraPosition();
	return { pos.x,pos.y,pos.z };
}

Vector3 Library::getRotateCameraTarget()
{
	DirectX::XMFLOAT3 pos = directx12->getRotateCameraTarget();
	return { pos.x,pos.y,pos.z };
}

#pragma endregion

#pragma region 文字表示
void Library::drawsSpriteFontString(Vector2 position, Vector2 size, std::string text, font* spriteTextureNum)
{
	directx12->drawSpriteFontString({ position.x,position.y }, { size.x,size.y }, text, *spriteTextureNum);
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
void Library::setParent(heap heapNum, int number, heap parentObjHeapNum, int parentNum)
{
	if (!heapNum || !parentObjHeapNum)return;

	directx12->setParent(*heapNum, number, *parentObjHeapNum, parentNum);
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

	directx12->calculationNormal
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

