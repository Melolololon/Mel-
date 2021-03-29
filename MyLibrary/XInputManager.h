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
		XINPUT_A_BUTTON = 0x1000,//PS�́~ Switch��B
		XINPUT_B_BUTTON = 0x2000,//PS�́� Switch��A
		XINPUT_X_BUTTON = 0x4000,//PS�́� Switch��Y
		XINPUT_Y_BUTTON = 0x8000,//PS�́� Switch��X
		XINPUT_R_STICK_BUTTON = 0x0040,//PS��R3
		XINPUT_L_STICK_BUTTON = 0x0080,//PS��L3
		XINPUT_RB_BUTTON = 0x0100,//PS��R1 Switch��R
		XINPUT_LB_BUTTON = 0x0200,//PS��L1 Switch��L
	};
private:
	static XINPUT_STATE padState[4];
	static XINPUT_STATE padPrevious[4];

	static bool padConnected[4];

	/// <summary>
	/// �p�b�h�ԍ���3�𒴂��Ă��Ȃ����ƁA�ڑ�����Ă��邩���m�F���܂��B
	/// </summary>
	/// <param name="padNum"></param>
	/// <returns></returns>
	static bool padCheck(const UCHAR& padNum);
public:
	static void update();

	static bool getPadConnectedFlag(const UCHAR& padNum);

#pragma region �{�^��
	static bool buttonState(const XInputButton& button,const UCHAR& padNum);
	static bool buttonTrigger(const XInputButton& button,const UCHAR& padNum);
	static bool buttonRelease(const XInputButton& button,const UCHAR& padNum);

#pragma endregion

#pragma region �A�i���O�X�e�B�b�N
	static bool leftStickLeft(const short& lXNum , const UCHAR& padNum);
	static bool leftStickRight(const short&  lXNum, const UCHAR& padNum);
	static bool leftStickUp(const short&  lYNum, const UCHAR& padNum);
	static bool leftStickDown(const short&  lYNum, const UCHAR& padNum);
	static bool rightStickLeft(const short&  lXNum, const UCHAR& padNum);
	static bool rightStickRight(const short&  lXNum, const UCHAR& padNum);
	static bool rightStickUp(const short&  lYNum, const UCHAR& padNum);
	static bool rightStickDown(const short&  lYNum, const UCHAR& padNum);

	/// <summary>
	/// ���X�e�B�b�N�����x�����p�x�ŕԂ��܂��B�X�e�B�b�N���X���Ă��Ȃ��ꍇ�A-1��Ԃ��܂��B
	/// </summary>
	/// <returns></returns>
	static float leftStickAngle(const UCHAR& padNum);

	/// <summary>
	/// �E�X�e�B�b�N�����x�����p�x�ŕԂ��܂��B�X�e�B�b�N���X���Ă��Ȃ��ꍇ�A-1��Ԃ��܂��B
	/// </summary>
	/// <returns></returns>
	static float rightStickAngle(const UCHAR& padNum);
#pragma endregion

#pragma region �U��
	/// <summary>
	/// �Q�[���p�b�h��U�������܂�
	/// </summary>
	/// <param name="leftPar">�����̐U����(0%�`100%)</param>
	/// <param name="rightPar">�E���̐U����(0%�`100%)</param>
	/// <param name="padNum">�p�b�h�ԍ�(1P��2P�̂���)</param>
	static void padVibration(const float& leftPar,const float& rightPar,const UCHAR& padNum);

#pragma endregion

};

