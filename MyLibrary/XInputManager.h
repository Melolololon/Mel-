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
		XINPUT_A_BUTTON = 0x1000,//PSの× SwitchのB
		XINPUT_B_BUTTON = 0x2000,//PSの○ SwitchのA
		XINPUT_X_BUTTON = 0x4000,//PSの□ SwitchのY
		XINPUT_Y_BUTTON = 0x8000,//PSの△ SwitchのX
		XINPUT_R_STICK_BUTTON = 0x0040,//PSのR3
		XINPUT_L_STICK_BUTTON = 0x0080,//PSのL3
		XINPUT_RB_BUTTON = 0x0200,//PSのR1 SwitchのR
		XINPUT_LB_BUTTON = 0x0100,//PSのL1 SwitchのL
	};
private:
	XInputManager(){}
	~XInputManager(){}

	static XINPUT_STATE padState[4];
	static XINPUT_STATE padPrevious[4];

	static bool padConnected[4];

	/// <summary>
	/// パッド番号が3を超えていないかと、接続されているかを確認します。
	/// </summary>
	/// <param name="padNum"></param>
	/// <returns></returns>
	static bool padCheck(const UCHAR& padNum);

	//軸の最高値
    static const short MAX_AXIS_VALUE;
	//振動の最高値
	static const WORD MAX_VIBRATION_VALUE;

public:
	static void Update();

	static bool GetPadConnectedFlag(const UCHAR& padNum);

#pragma region ボタン
	static bool ButtonState(const XInputButton& button,const UCHAR& padNum);
	static bool ButtonTrigger(const XInputButton& button,const UCHAR& padNum);
	static bool ButtonRelease(const XInputButton& button,const UCHAR& padNum);

	/// <summary>
	/// 十字ボタンが示す角度を取得します。
	/// </summary>
	/// <returns></returns>
	static float DirectionalButtonAngle(const UCHAR& padNum);
#pragma endregion

#pragma region アナログスティック
	static bool LeftStickLeft(const float& lXPar , const UCHAR& padNum);
	static bool LeftStickRight(const float&  lXPar, const UCHAR& padNum);
	static bool LeftStickUp(const float&  lYPar, const UCHAR& padNum);
	static bool LeftStickDown(const float&  lYPar, const UCHAR& padNum);
	static bool RightStickLeft(const float&  lXPar, const UCHAR& padNum);
	static bool RightStickRight(const float&  lXPar, const UCHAR& padNum);
	static bool RightStickUp(const float&  lYPar, const UCHAR& padNum);
	static bool RightStickDown(const float&  lYPar, const UCHAR& padNum);

	/// <summary>
	/// 左スティックが何度かを角度で返します。スティックが傾いていない場合、-1を返します。
	/// </summary>
	/// <returns></returns>
	static float LeftStickAngle(const UCHAR& padNum);

	/// <summary>
	/// 右スティックが何度かを角度で返します。スティックが傾いていない場合、-1を返します。
	/// </summary>
	/// <returns></returns>
	static float RightStickAngle(const UCHAR& padNum);
#pragma endregion

#pragma region 振動
	/// <summary>
	/// ゲームパッドを振動させます
	/// </summary>
	/// <param name="leftPar">左側の振動量(0%～100%)</param>
	/// <param name="rightPar">右側の振動量(0%～100%)</param>
	/// <param name="padNum">パッド番号(1Pや2Pのこと)</param>
	static void PadVibration(const float& leftPar,const float& rightPar,const UCHAR& padNum);

#pragma endregion

};

