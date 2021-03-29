#pragma once
#include<Windows.h>
#include<Xinput.h>
#pragma comment(lib,"Xinput9_1_0.lib")
class XInputManager
{
public :
	enum XInputButton 
	{
		
		XINPUT_UP_BUTTON = 0x0001,
		XINPUT_DOWN_BUTTON = 0x0002,
		XINPUT_LEFT_BUTTON = 0x0004,
		XINPUT_RIGHT_BUTTON = 0x0008,
		XINPUT_START_BUTTON = 0x00010,
		XINPUT_BACK_BUTTON = 0x00020,
		XINPUT_A_BUTTON = 0x1000,
		XINPUT_B_BUTTON = 0x2000,
		XINPUT_X_BUTTON = 0x4000,
		XINPUT_Y_BUTTON = 0x8000,
		XINPUT_R_STICK_BUTTON = 0x0040,
		XINPUT_L_STICK_BUTTON = 0x0080,
		XINPUT_RB_BUTTON = 0x0100,
		XINPUT_LB_BUTTON = 0x0200,
	};
private:
	static XINPUT_STATE padState[4];
	static XINPUT_STATE padPrevious[4];

	static bool padConnected[4];
public:
	static void update();

	static bool getPadConnectedFlag(const UCHAR& padNum);

#pragma region ボタン
	static bool buttonState(const XInputButton& button,const UCHAR& padNum);
	static bool buttonTrigger(const XInputButton& button,const UCHAR& padNum);
	static bool buttonRelease(const XInputButton& button,const UCHAR& padNum);

#pragma endregion

#pragma region アナログスティック
	static bool leftStickLeft(const short& lXNum , const UCHAR& padNum);
	static bool leftStickRight(const short&  lXNum, const UCHAR& padNum);
	static bool leftStickUp(const short&  lYNum, const UCHAR& padNum);
	static bool leftStickDown(const short&  lYNum, const UCHAR& padNum);
	static bool rightStickLeft(const short&  lXNum, const UCHAR& padNum);
	static bool rightStickRight(const short&  lXNum, const UCHAR& padNum);
	static bool rightStickUp(const short&  lYNum, const UCHAR& padNum);
	static bool rightStickDown(const short&  lYNum, const UCHAR& padNum);

	/// <summary>
	/// 左スティックが何度かを角度で返します。スティックが傾いていない場合、-1を返します。
	/// </summary>
	/// <returns></returns>
	static float leftStickAngle(const UCHAR& padNum);

	/// <summary>
	/// 右スティックが何度かを角度で返します。スティックが傾いていない場合、-1を返します。
	/// </summary>
	/// <returns></returns>
	static float rightStickAngle(const UCHAR& padNum);
#pragma endregion


};

