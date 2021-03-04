#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include<Windows.h>
#include<vector>
#include"Vector.h"

#include<DirectXMath.h>


#pragma region パッドのボタン
//これグローバル定数にする?
const int XButton = 0;
const int YButton = 1;
const int AButton = 2;
const int BButton = 3;
const int LBButton = 4;
const int RBButton = 5;
const int LTButton = 6;
const int RTButton = 7;
const int StartButton = 11;
#pragma endregion
#pragma region 十字キー
//enumに変える
const int UpButton = 0;
const int RightButton = 1;
const int DownButton = 2;
const int LeftButton = 3;
#pragma endregion

#pragma region マウスボタン

enum MouseButton
{
	MOUSE_BUTTON_LEFT,
	MOUSE_BUTTON_RIGHT,
	MOUSE_BUTTON_CENTER,
	//MOUSE_BUTTON_SIDE_DOWN,

};
#pragma endregion


class DirectInput
{
private:
	//列挙用コールバック
	static BOOL CALLBACK diEnumDevicesCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
	static BOOL CALLBACK diEnumDevicesObjectCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

	static HINSTANCE hInstance;
	static HRESULT dIResult;//結果取得
	static LPDIRECTINPUT8 dInput8;//dxInputを入れる変数
	static LPDIRECTINPUTDEVICE8 keyDev;//デバイスを入れる変数

	static IDirectInput8* dInputDevice;
	static IDirectInputDevice8* devKeyBoard;
	static IDirectInputDevice8* devMouse;
	static IDirectInputDevice8* devPad;

	//static IDirectInputDevice8* padSetUp;
	//static DIDEVICEOBJECTDATA* lpdidod;
	static std::vector<DIDEVICEINSTANCE>getEnum;
	static DIDEVCAPS padCaps;


	//パッド
	static DIJOYSTATE padState;//パッド情報
	static DIJOYSTATE padPrevious;//パッド情報(1ループ前)


	//現在のキー情報
	static BYTE key[256];
	//1フレーム前のキー情報
	static BYTE preKey[256];

	//マウス
	static DIMOUSESTATE mouseState;
	static DIMOUSESTATE mousePrevious;


	//取得したウィンドウハンドル
	static HWND mHwnd;

	//パッドを認識したかどうか
	static bool setPad;

	//ウィンドウサイズ
	static int winWidth;
	static int winHeight;

	//行列
	static DirectX::XMMATRIX viewMat;
	static DirectX::XMMATRIX projectionMat;

	//カメラ
	static float nearNum;
	static float farNum;

public:


	//生成
	static void initialize(HWND hwnd,int windowWidth,int windowHeight);
	//情報取得
	static void update();

#pragma region キーボード

	//押しているか
	static bool keyState(int keyDef);
	//押した瞬間か
	static bool keyTrigger(int keyDef);
	//離した瞬間か
	static bool keyRelease(int keyDef);
#pragma endregion

#pragma region パッド
#pragma region 通常ボタン

	//押しているか
	static bool buttonState(int button);
	//押した瞬間か
	static bool buttonTrigger(int button);
	//離した瞬間か
	static bool buttonRelease(int button);
#pragma endregion

#pragma region 十字キー

	static bool directionalButtonState(int button);
	static bool directionalButtonTrigger(int button);
#pragma endregion

#pragma endregion

#pragma region スティック
	/// <summary>
	/// 
	/// </summary>
	/// <param name="lNum">軸(32767が最高値)</param>
	/// <returns></returns>
	static bool leftStickLeft(int lXNum);
	static bool leftStickRight(int lXNum);
	static bool leftStickUp(int lYNum);
	static bool leftStickDown(int lYNum);
	static bool rightStickLeft(int lXNum);
	static bool rightStickRight(int lXNum);
	static bool rightStickUp(int lYNum);
	static bool rightStickDown(int lYNum);

	
#pragma endregion


	//DInputの開放
	static void release();

#pragma region マウス

	static void setMatrixAndNearFar(DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix,float nearNumber ,float farNumber);

	/// <summary>
	/// クライアント座標に変換し、最初に設定したウィンドウサイズに応じて補正したマウスの座標を取得します
	/// </summary>
	/// <returns></returns>
	static Vector2 getMousePosition();


	/// <summary>
	/// マウスカーソルから出ている線分の最近点と最遠点を取得します
	/// </summary>
	/// <param name="nearPoint"></param>
	/// <param name="farPoint"></param>
	static void getMouse3DLine(Vector3& nearPoint,Vector3& farPoint);

	static bool mouseButtonState(MouseButton mouseButton);

	static bool mouseButtonTrigger(MouseButton mouseButton);
	
	static bool mouseButtonRelease(MouseButton mouseButton);
#pragma endregion



};



