#pragma once

#include<Windows.h>
#include<Xinput.h>
#pragma comment(lib,"Xinput9_1_0.lib")


#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#include<DirectXMath.h>
#include<unordered_map>

#include"Vector.h"
#include"Camera.h"

namespace MelLib 
{
	enum class GamePadButton
	{
		UP = 0x0001,//十字キー上
		DOWN = 0x0002,//十字キー下
		LEFT = 0x0004,//十字キー左
		RIGHT = 0x0008,//十字キー右
		START = 0x00010,
		BACK = 0x00020,
		A = 0x1000,//PSの× SwitchのB
		B = 0x2000,//PSの○ SwitchのA
		X = 0x4000,//PSの□ SwitchのY
		Y = 0x8000,//PSの△ SwitchのX
		R_STICK = 0x0040,//PSのR3
		L_STICK = 0x0080,//PSのL3
		RB = 0x0200,//PSのR1 SwitchのR
		LB = 0x0100,//PSのL1 SwitchのL
	};

	enum class MouseButton
	{
		LEFT,
		RIGHT,
		CENTER,
	};

	//GetPressKeyChars使わずに、WinAPIのエディットボックスで文字取得するようにする?
	//エディットボックスを透明にできるか試す
	//そもそも子ウィンドウ扱いだからゲーム画面に表示できない?できる?
	//できなかった
	//もしかして隠れてても入力はできる?ウィンドウタイプで設定すればいける

	//入力クラス
	class Input
	{
	private:
		Input() {}
		~Input() {}

#pragma region DirectInput


		static HINSTANCE hInstance;
		static HRESULT dIResult;//結果取得
		static LPDIRECTINPUT8 dInput8;//dxInputを入れる変数
		static LPDIRECTINPUTDEVICE8 keyDev;//デバイスを入れる変数

		static IDirectInput8* dInputDevice;
		static IDirectInputDevice8* devKeyBoard;
		static IDirectInputDevice8* devMouse;

		//現在のキー情報
		static BYTE keysState[256];

		//1フレーム前のキー情報
		static BYTE preKeysState[256];

		//現在押されているキー
		static std::vector<BYTE> currentPressKeys;

		//GetPressKeyChars()から文字列を取得したかどうか
		static bool callGetPressKeyChars;
		//前フレームでGetPressKeyChars()から文字列を取得したかどうか
		static bool callPreGetPressKeyChars;


		static std::unordered_map<std::string, HWND>hwnds;
		//キーを押した時に入力文字を返すための配列
		static std::vector<std::unordered_map<UCHAR, char>>returnChars;

		//パッドでの入力に使えるかもと思い、用意したもの
		static const std::vector<wchar_t>hiragana;
		static const std::vector<wchar_t>katakana;


		//マウス
		static DIMOUSESTATE mouseState;
		static DIMOUSESTATE mousePrevious;

		//取得したウィンドウハンドル
		static HWND mHwnd;

		static LRESULT ParamChildWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#pragma endregion

#pragma region XInput


		static XINPUT_STATE padState[4];
		static XINPUT_STATE padPrevious[4];

		static bool padConnected[4];

		//軸の最高値
		static const short MAX_AXIS_VALUE = 32767;
		//振動の最高値
		static const WORD MAX_VIBRATION_VALUE = 65535;
#pragma endregion

		//行列
		static DirectX::XMMATRIX viewMat;
		static DirectX::XMMATRIX projectionMat;

		//カメラ
		static float nearNum;
		static float farNum;


		//ウィンドウサイズ
		static int winWidth;
		static int winHeight;

		/// <summary>
		/// パッド番号が3を超えていないかと、接続されているかを確認します。
		/// </summary>
		/// <param name="padNum"></param>
		/// <returns></returns>
		static bool PadCheck(const UCHAR padNum);


	public:
		static void Initialize(HWND hwnd, const int windowWidth, const int windowHeight);
		static void Update();
		static void Finalize();

		static bool GetPadConnectedFlag(const UCHAR padNum);


#pragma region キーボード

		//押しているか
		static bool KeyState(const BYTE keyDef);
		//押した瞬間か
		static bool KeyTrigger(const BYTE keyDef);
		//離した瞬間か
		static bool KeyRelease(const BYTE keyDef);

#pragma region 文字取得
		//GetPressKeyCharsとかは気軽に文字取得できるし、
		//タイピングゲームみたいに枠用意する必要ないときや、必要ないときに使えるから残しといていい

		/// <summary>
		/// 現在押されているキーの文字を返します。
		/// </summary>
		/// <returns></returns>
		static std::string GetPressKeyChars();

		/// <summary>
		/// 押された瞬間にキーの文字を返します。
		/// </summary>
		/// <returns></returns>
		static std::string GetTriggerKeyChars();


		static void CreateStringInputWindow
		(
			const std::wstring& initStr,
			const Vector2& position,
			const Vector2& size,
			HWND parentHWND,
			const std::string& name
		);

		static void DeleteStringInputWindow(const std::string& name);

		static std::wstring GetInputString(const std::string& name);

#pragma endregion

		/// <summary>
		/// 現在押されているキーを取得します。
		/// </summary>
		/// <returns></returns>
		static const std::vector<BYTE>& GetStateKeys() { return currentPressKeys; }

		/// <summary>
		/// アローキーが何度を示しているかを角度で返します。	
		/// </summary>
		/// <returns></returns>
		static float ArrowKeyAngle();
#pragma endregion

#pragma region マウス


#pragma region カーソル


		static void SetViewMatrix(const DirectX::XMMATRIX& viewMatrix) { viewMat = viewMatrix; }
		static void SetProjectionMatrix(const DirectX::XMMATRIX& projectionMatrix) { projectionMat = projectionMatrix; }
		static void SetNear(const float nearNumber) { nearNum = nearNumber; }
		static void SetFar(const float farNumber) { farNum = farNumber; }

		/// <summary>
		/// クライアント座標に変換し、最初に設定したウィンドウサイズに応じて補正したマウスの座標を取得します
		/// </summary>
		/// <returns></returns>
		static Vector2 GetMousePosition();

		/// <summary>
		/// ウィンドウの中心を基準としたカーソルのベクトルを取得します。
		/// </summary>
		/// <returns>ベクトル</returns>
		static Vector2 GetCenterToMouseVector();

		/// <summary>
		/// ウィンドウの中心を基準としたカーソルの角度を取得します。
		/// </summary>
		/// <returns>角度</returns>
		static float GetMouseAngle();
#pragma endregion


#pragma region ボタン


		/// <summary>
		/// マウスカーソルから出ている線分の最近点と最遠点を取得します
		/// </summary>
		/// <param name="nearPoint"></param>
		/// <param name="farPoint"></param>
		static void GetMouse3DLine(Vector3& nearPoint, Vector3& farPoint);

		static bool MouseButtonState(const MouseButton mouseButton);

		static bool MouseButtonTrigger(const MouseButton mouseButton);

		static bool MouseButtonRelease(const MouseButton mouseButton);


#pragma endregion

#pragma endregion

#pragma region ゲームパッド


#pragma region ボタン
		static bool ButtonState(const UCHAR padNum,const GamePadButton button);
		static bool ButtonTrigger(const UCHAR padNum, const GamePadButton button);
		static bool ButtonRelease(const UCHAR padNum, const GamePadButton button);

		/// <summary>
		/// 十字ボタンが示す角度を取得します。
		/// </summary>
		/// <returns></returns>
		static float DirectionalButtonAngle(const UCHAR padNum);
#pragma endregion

#pragma region アナログスティック

#pragma region 左

		static bool LeftStickLeft(const UCHAR padNum, const float lXPar);
		static bool LeftStickRight(const UCHAR padNum, const float lXPar);
		static bool LeftStickUp(const UCHAR padNum, const float lYPar);
		static bool LeftStickDown(const UCHAR padNum, const float lYPar);

		/// <summary>
		/// 左スティックの角度を返します。スティックが傾いていない場合、-1を返します。
		/// </summary>
		/// <returns></returns>
		static float LeftStickAngle(const UCHAR padNum);


		static Vector2 LeftStickVector2(const UCHAR padNum, const bool dimention3D);
		static Vector3 LeftStickVector3(const UCHAR padNum, Camera* pCamera = nullptr,const bool rotX = false, const bool rotY = false);
#pragma endregion

#pragma region 右
		static bool RightStickLeft(const UCHAR padNum, const float lXPar);
		static bool RightStickRight(const UCHAR padNum, const float lXPar);
		static bool RightStickUp(const UCHAR padNum, const float lYPar);
		static bool RightStickDown(const UCHAR padNum, const float lYPar);


		/// <summary>
		/// 右スティックが何度かを角度で返します。スティックが傾いていない場合、-1を返します。
		/// </summary>
		/// <returns></returns>
		static float RightStickAngle(const UCHAR padNum);


		static Vector2 RightStickVector2(const UCHAR padNum,const bool dimention3D);
		static Vector3 RightStickVector3(const UCHAR padNum, Camera* pCamera = nullptr, const bool rotX = false, const bool rotY = false);
#pragma endregion


	

	

#pragma endregion

#pragma region 振動
		/// <summary>
		/// ゲームパッドを振動させます
		/// </summary>
		/// <param name="leftPar">左側の振動量(0%～100%)</param>
		/// <param name="rightPar">右側の振動量(0%～100%)</param>
		/// <param name="padNum">パッド番号(1Pや2Pのこと)</param>
		static void PadVibration(const float leftPar, const float rightPar, const UCHAR padNum);

#pragma endregion
#pragma endregion

	};
}