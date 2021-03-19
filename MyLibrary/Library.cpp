#include "Library.h"
#include"LibMath.h"
#include"XInputManager.h"
#pragma comment(lib,"winmm.lib")//timeGetTime��
#include"ObjectManager.h"

std::unique_ptr<Audio> Library::audio;
DirectX12* Library::directx12;
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
	srand((unsigned int)time(NULL));
	count = 0;
	fps = 60;
	startProsessTime = timeGetTime();
	nowTime = timeGetTime();

#pragma region �E�B���h�E����

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

	//DC���擾
	hdc = GetDC(hwnd);
	//���t���b�V�����[�g���擾
	refReat = GetDeviceCaps(hdc, VREFRESH);
#pragma endregion

	audio = std::unique_ptr<Audio>(new Audio());

	directx12 = new DirectX12(hwnd, windowWidth, windowHeight);
	directx12->setScreenColor(screenColor);
	directx12->initialize();

	createPipelineCounter = directx12->getStartPipelineCreateNum();
	//�X�v���C�g�t�H���g300�����̂ŃJ�E���g�𑝂₵�Ă���
	createSpriteConter = 300;

	//�ׂ��h�p�e�N�X�`����0�ɂ��邽�߂�1�ɂ���(DrawBox�ȂǂɎg��)
	loadTextureCounter = 1;

	//modelDatas.reserve(99999);
}

void Library::roopStartProcess()
{
	startProsessTime = timeGetTime();

#pragma region �E�B���h�E����

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
	//�\�[�g
	directx12->sortModelData(modelDatas);

	//�R�}���h�Z�b�g
	ModelData mData;
	int number;
	for(const auto& m : modelDatas)
	{
		mData = std::get<0>(m);
		number = std::get<1>(m);
		directx12->map(mData, number);
		directx12->setCmdList(mData.key, number);
	}


	directx12->draw();

	if (isSetFPS60)
	{

		//60fps�̎���1�t���[���̏������Ԃ��v��(���̐��l�́A���Ɉˑ����Ȃ�)
		float pureTime = 1000.0f / (float)fps * ((float)count - 1.0f);
		float currTime = 1000.0f / (float)fps * (float)count;
		int taikiTime = (int)currTime - (int)pureTime;//�ҋ@���Ȃ��Ƃ����Ȃ�����
		count++;
		count = count >= fps ? 0 : count;

		//�O���[�v����ǂ̂��炢���Ԃ��o�߂��������v��(pre���AstartProsess�̂ق��Ɉڂ��AGetTickCount64�����)
		//previousTime = currentTime;
		nowTime = timeGetTime();
		DWORD keika = nowTime - startProsessTime;
		//sabun /= 1000;

		//�ҋ@���Ԃ��v��(taikiTime��sabun�̍�����slee�őҋ@)
		int sleepTime = taikiTime - keika;
		sleepTime = sleepTime <= 0 ? 0 : sleepTime;

		//�X���[�v
		timeBeginPeriod(1);
		Sleep(sleepTime);
		timeEndPeriod(1);
	}

	modelDatas.clear();
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

#pragma region �v���C�x�[�g�֐�
bool Library::checkSetKeyName(const std::string& key)
{
	if (key == "")
	{
		OutputDebugString(L"�L�[�̖��O���ݒ肳��Ă��܂���\n");
		return false;
	}
	return true;
}

bool Library::checkCreateVertexBuffer(const VertexType& vertexType)
{
	if (vertexType == VERTEX_TYPE_NONE)
	{
		OutputDebugString(L"�I�u�W�F�N�g�̐����A�܂��̓��f���̓ǂݍ��݂�����Ă��܂���\n");
		return false;
	}
	return true;
}
#pragma endregion


#pragma region WinAPI�֌W
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

#pragma region �p�C�v���C���ݒ�
void Library::setDespTestFlag(bool flag)
{
	//directx12->setDespTestFlag(flag);
}
#pragma endregion

#pragma region �p�C�v���C���쐬
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

void Library::getMatrix(float matrix[4][4], const ModelData& modelData, int number)
{
	directx12->getMatrix(matrix, modelData.key, number);
}

void Library::getCameraMatrix(float matrix[4][4])
{
	directx12->getCameraMatrix(matrix);
}


void Library::setInputLayout(const std::vector<InputLayoutData>& inputLayoutData)
{
	directx12->setInputLayout(inputLayoutData);
}

void Library::deleteInputLayout()
{
	directx12->deleteInputLayout();
}



#pragma region �|�X�g�G�t�F�N�g
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


#pragma region �ݒ�

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

#pragma region ����


#pragma region ���__�C���f�b�N�X�o�b�t�@�쐬

#pragma region ���f���ǂݍ���
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

	PolygonData pData;
	
	/*p.handle = new int(directx12->getCreateNumber().polyNum);
	pData.sikibetuNumP = p.handle;*/
	pData.dimention = dimention3D;

	modelData.type = directx12->loadOBJVertex
	(
		path,
		loadUV,
		loadNormal,
		materialFireName,
		pData,
		modelData.key
	);
	


}
#pragma endregion


void Library::createPoint(int createNum, point* p)
{
	*p = new int(createPointCount);
	directx12->createPoint(createNum, *p);
	createPointCount++;
}

void Library::createBoard(Vector2 size, int dimention,  ModelData& modelData)
{
	if (!checkSetKeyName(modelData.key))return;
	PolygonData pData;

	pData.pos1 = { size.x,size.y,0 };
	pData.katatiNum = 1;


	//pData.sikibetuNum = createPolygonNumber;

	/*p.handle = new int(directx12->getCreateNumber().polyNum);
	pData.sikibetuNumP = p.handle;*/

	pData.dimention = dimention;
	directx12->createPolygonData(pData, modelData.key);
	modelData.type = VERTEX_TYPE_NORMAL;
}

void Library::createCircle(float r, int dimention, ModelData& modelData)
{
	if (!checkSetKeyName(modelData.key))return;
	PolygonData pData;

	pData.fNum1 = r;
	pData.katatiNum = 2;

	/*p.handle = new int(directx12->getCreateNumber().polyNum);
	pData.sikibetuNumP = p.handle;*/
	pData.dimention = dimention;
	directx12->createPolygonData(pData, modelData.key);

	
	modelData.type = VERTEX_TYPE_NORMAL;
}

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
//	/*p.handle = new int(directx12->getCreateNumber().polyNum);
//	pData.sikibetuNumP = p.handle;*/
//	pData.dimention = dimention3D;
//
//	directx12->createPolygonData(pData, p.key);
//}



void Library::createTriangularPyramid
(
	float r,
	int vertexNumber, 
	Vector3 centerPosition, 
	float upVertex, 
	ModelData& modelData
)
{
	if (!checkSetKeyName(modelData.key))return;
	PolygonData pData;

	pData.fNum1 = r;
	pData.fNum2 = upVertex;
	pData.num1 = vertexNumber;
	pData.pos1 = centerPosition;
	pData.katatiNum = 11;

	//pData.sikibetuNum = createPolygonNumber;

	//p.handle = new int(directx12->getCreateNumber().polyNum);
	//pData.sikibetuNumP = p.handle;
	pData.dimention = dimention3D;
	directx12->createPolygonData(pData, modelData.key);

	
	modelData.type = VERTEX_TYPE_NORMAL;
}


void Library::create3DBox(Vector3 size,  ModelData& modelData)
{
	if (!checkSetKeyName(modelData.key))return;
	PolygonData pData;

	pData.pos1 = { size.x,size.y,size.z };
	pData.katatiNum = 100;


	/*p.handle = new int(directx12->getCreateNumber().polyNum);
	pData.sikibetuNumP = p.handle;*/
	pData.dimention = dimention3D;

	directx12->createPolygonData(pData, modelData.key);

	
	modelData.type = VERTEX_TYPE_NORMAL;
}

#pragma region ���[�U�[

void Library::createUserObject
(
	std::vector<Vector3>& vertexPos, 
	std::vector<Vector2>& vertexUV,
	std::vector<unsigned short>& index,
	ModelData& modelData
)
{
	if (!checkSetKeyName(modelData.key))return;
	directx12->addUserVertex(vertexPos, vertexUV, modelData.key);
	directx12->addUserIndex(index, modelData.key);

	PolygonData pData;
	pData.katatiNum = -1;


	/*p.handle = new int(directx12->getCreateNumber().polyNum);
	pData.sikibetuNumP = p.handle;*/
	pData.dimention = dimention3D;

	directx12->createPolygonData(pData, modelData.key);
	
	
	modelData.type = VERTEX_TYPE_NORMAL;
}

void Library::createUserObject2
(
	void** vertexData, 
	unsigned int vertexDataSize, 
	unsigned int vertexSumDataSize, 
	std::vector<unsigned short>&index, 
	ModelData& modelData
)
{
	if (!checkSetKeyName(modelData.key))return;
	PolygonData pData;
	//p.handle = new int(directx12->getCreateNumber().polyNum);
	//pData.sikibetuNumP = p.handle;
	
	
	pData.dimention = dimention3D;
	directx12->createUserPolygon(vertexData, vertexDataSize, vertexSumDataSize, index, pData, modelData.key);
	
	modelData.type = VERTEX_TYPE_USER_VERTEX;
}
#pragma endregion



#pragma endregion

#pragma region �q�[�v�쐬

void Library::loadOBJMaterial
(
	std::string materialDirectoryPath, 
	std::string materialFileName, 
	int objectNum, 
	 ModelData& modelData
)
{
	if (!checkCreateVertexBuffer(modelData.type))return;
	

	directx12->loadOBJMaterial(materialDirectoryPath, materialFileName, false, modelData.key, objectNum, modelData.type);
	
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
	directx12->setConstMapData(dataP, dataSize);
	
	directx12->loadOBJMaterial(materialDirectoryPath, materialFileName, true, modelData.key, objectNum, modelData.type);
	
}

void Library::createHeapData
(
	const wchar_t* texturePath, 
	int objectNum,
	ModelData& modelData
)
{
	


	if (!checkCreateVertexBuffer(modelData.type))return;

	directx12->createHeapData(false, modelData.key, objectNum, texturePath, nullptr);
	
	
}

void Library::createHeapData2
(
	Color color, 
	int objectNum,
	ModelData& modelData
)
{

	if (!checkCreateVertexBuffer(modelData.type))return;

	directx12->createHeapData(false, modelData.key, objectNum, L"", &color);

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
	directx12->setConstMapData(dataP, dataSize);


	directx12->createHeapData(true, modelData.key, objectNum, texturePath, nullptr);

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
	directx12->setConstMapData(dataP, dataSize);


	directx12->createHeapData(true, modelData.key, objectNum, L"", &color);

}


#pragma endregion

#pragma region �����쐬
//void Library::loadOBJ(const char* path, std::string materialDirectoryPath, bool loadUV, bool loadNormal, int objectNum, vertex* vertP, heap* heapP)
//{
//	//���_�ǂݍ��ݎ��Ƀt�@�C�����擾���āAheap�쐬���Ƀ}�e���A����ǂݍ��ނ悤�ɂ���
//	//�}�e���A�����́Aload������󂯎���悤�ɂ���
//	//�����悤�ȏ���(�q�[�v�쐬�Ȃ�)�͊֐�������
//	PolyData pData;
//	*vertP = new int(directx12->getCreateNumber().polyNum);
//	pData.sikibetuNumP = *vertP;
//	pData.dimention = dimention3D;
//
//	HeapData hData;
//	hData.objectNum = objectNum;
//	*heapP = new int(directx12->getCreateNumber().despNum);
//	hData.sikibetuNumP = *heapP
//		;
//	directx12->loadOBJ
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


#pragma region �X�v���C�g

font Library::loadSpriteFont(const wchar_t* texturePath, Vector2 lineNum, Vector2 fontSize)
{
	loadFontTextureCounter++;
	directx12->loadSpriteFont(texturePath, { lineNum.x,lineNum.y }, { fontSize.x,fontSize.y });

	return loadFontTextureCounter - 1;

}

//�e�N�X�`���̔ԍ���Ԃ�
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

//�X�v���C�g�̎��ʔԍ���Ԃ�
void Library::createSprite(sprite* sprite)
{
	*sprite = new int(createSpriteConter);
	directx12->createSprite(*sprite, false);

	createSpriteConter++;
	//return createSpriteConter - 1;
}
#pragma endregion


#pragma endregion

#pragma region �`��

void Library::drawGraphic
(
	const ModelData& modelData,
	int number
)
{
	std::tuple<ModelData, int> mData = std::make_tuple(modelData, number);
	modelDatas.push_back(mData);

	/*directx12->map(modelData, number);
	directx12->setCmdList(modelData.key, number);*/
}

void Library::drawSprite(Vector2 position, sprite spriteNumber, texture* textureNumber)
{
	//directx12->spriteSetObjectPosition({ position.x,position.y }, *spriteNumber);
	directx12->spriteMap({ position.x,position.y }, { 0,0 }, *spriteNumber, *textureNumber);
	directx12->spriteSetCmdList(*spriteNumber, *textureNumber);
}

void Library::drawSpriteAnimation(Vector2 position, Vector2 maxSqare, Vector2 currentNum, sprite spriteNumber, texture* textureNumber)
{
	//���̏��Ԃ���Ȃ���uv�������ƃZ�b�g����Ȃ�
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

#pragma region �ׂ��h
void Library::drawBox(const Vector2 position, const Vector2& size, const Color& color, sprite spriteHandle)
{
	setSpriteAddColor(color, spriteHandle);
	directx12->spriteMap({ position.x,position.y }, { size.x,size.y }, *spriteHandle, 0);
	directx12->spriteSetCmdList(*spriteHandle, 0);
}
#pragma endregion


#pragma endregion

#pragma region �폜
void Library::deleteObject3DData(const ModelData& modelData)
{
	directx12->deletePolygonData(modelData.key);
	directx12->deleteHeapData(modelData.key);
}


void Library::deleteSprite(sprite sprite)
{
	directx12->deleteSprite(*sprite);
}
#pragma endregion

#pragma region ������


#pragma region �X���[�X�V�F�[�f�B���O
void Library::setSmoothingFlag(bool flag)
{
	directx12->setSmoothingFlag(flag);
}
#pragma endregion

void Library::setMulColor(Color color,const ModelData& modelData, int number)
{
	
	directx12->setMulColor(color, modelData.key, number);
}
void Library::setAddColor(Color color,const ModelData& modelData, int number)
{

	directx12->setAddColor(color, modelData.key, number);
}
void Library::setSubColor(Color color,const ModelData& modelData, int number)
{
	
	directx12->setSubColor(color, modelData.key, number);
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

#pragma region �J����
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

#pragma region ���C�g

void Library::setLightVector(Vector3 vector)
{
	Vector3 v = vector3Normalize(vector);
	directx12->setLightVector({ v.x,v.y,v.z });
}
//

void Library::setLightColor(Color lightColor)
{
	directx12->setLightColor(lightColor);
}
#pragma endregion

#pragma region ����
void Library::setPosition(Vector3 position, const ModelData& modelData , int number)
{
	
	directx12->setObjectPosition({ position.x,position.y,position.z }, modelData.key, number);
}


void Library::setAngle(Vector3 angle, const ModelData& modelData,  int number)
{

	directx12->setObjectAngle({ angle.x,angle.y,angle.z }, modelData.key, number);
}

void Library::setScale(Vector3 scale, const ModelData& modelData , int number)
{
	directx12->setObjectScale({ scale.x,scale.y,scale.z }, modelData.key, number);
}

void Library::setPushPorigonNumber(float ex, const ModelData& modelData, int number)
{
	directx12->setObjectPushNum(ex, modelData.key, number);
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



#pragma region �|�X�g�G�t�F�N�g

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

#pragma region �A�j���[�V����

std::vector<Vector3> Library::getBonePosition(const ModelData& modelData)
{
	std::vector<DirectX::XMFLOAT3>getVector = directx12->getBonePosition(modelData.key);
	std::vector<Vector3>returnVector(getVector.size());

	int count = 0;
	for(auto& retV : returnVector)
	{
		retV = getVector[count];
		count++;
	}
	return returnVector;
}

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

	directx12->setOBJBoneMoveVector(vector.toXMFLOAT3(), boneNum, modelData.key, objectNum);

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
	directx12->setOBJBoneScale(scale.toXMFLOAT3(), boneNum, modelData.key, objectNum);
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
	directx12->setOBJBoneAngle(angle.toXMFLOAT3(), boneNum, modelData.key, objectNum);
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

	directx12->setParentOBJBone
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
	directx12->setParentOBJBoneScaleImpact
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
	directx12->setParentOBJBoneAngleImpact
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
	directx12->setParentOBJBoneMoveVectorImpact
	(
		boneNum,
		moveVectorImpact.toXMFLOAT3(),
		modelData.key
	);
}

#pragma endregion


#pragma endregion

#pragma region �A�j���[�V����
//
//void Library::changeAnimation(vertex createNum, int maxWidth, int maxHeight, int animationNumX, int animationNumY)
//{
//	if (!createNum)return;
//	directx12->setAnimation(*createNum, maxWidth, maxHeight, animationNumX, animationNumY);
//}
//
//void Library::changeAnimation2(vertex vertexNum, heap dataNum, int startAreaX, int startAreaY, int endAreaX, int endAreaY)
//{
//	if (!vertexNum || !dataNum)return;
//	directx12->setAnimation2(*vertexNum, *dataNum, startAreaX, startAreaY, endAreaX, endAreaY);
//}

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

#pragma region ���_���W�擾�Ȃ�
std::vector<std::vector<Vector3>> Library::getVertexPosition(const ModelData& modelData)
{
	std::vector<std::vector<Vector3>>vector3VertexPos;
	std::vector<std::vector<DirectX::XMFLOAT3>> xmFloat3VertexPos;
	xmFloat3VertexPos = directx12->getObjectVertexPosition(modelData.key);
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
	return directx12->overrideWriteVertexPosition(kariXM, modelData.key);
}
#pragma endregion

#pragma region �s��ɂ��ϊ�
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

#pragma region �����\��
void Library::drawsSpriteFontString(Vector2 position, Vector2 size, std::string text, font* spriteTextureNum)
{
	directx12->drawSpriteFontString({ position.x,position.y }, { size.x,size.y }, text, *spriteTextureNum);
}
#pragma endregion

#pragma region �ǂݍ���
bool Library::loadTextIntVector(const char* path, std::vector<std::vector<int>>& vector)
{

	if (!TextLoader::loadText(path))return false;
	TextLoader::getIntVector(vector);
	return true;
}
#pragma endregion

#pragma region �e�q�\��
void Library::setParent(const ModelData& modelData, int number, const ModelData& parentmodelData, int parentNum)
{

	directx12->setParent(modelData.key, number, parentmodelData.key, parentNum);
}

#pragma endregion

#pragma region �v�Z
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

#pragma region �T�E���h
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

