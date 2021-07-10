#pragma once

#include<Windows.h>
#include<Xinput.h>
#pragma comment(lib,"Xinput9_1_0.lib")


#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include"Vector.h"
#include<DirectXMath.h>

#include<unordered_map>

//����PadButton�ɕς���
enum class XInputButton
{
	UP_BUTTON = 0x0001,//�\���L�[��
	DOWN_BUTTON = 0x0002,//�\���L�[��
	LEFT_BUTTON = 0x0004,//�\���L�[��
	RIGHT_BUTTON = 0x0008,//�\���L�[�E
	START_BUTTON = 0x00010,
	BACK_BUTTON = 0x00020,
	A_BUTTON = 0x1000,//PS�́~ Switch��B
	B_BUTTON = 0x2000,//PS�́� Switch��A
	X_BUTTON = 0x4000,//PS�́� Switch��Y
	Y_BUTTON = 0x8000,//PS�́� Switch��X
	R_STICK_BUTTON = 0x0040,//PS��R3
	L_STICK_BUTTON = 0x0080,//PS��L3
	RB_BUTTON = 0x0200,//PS��R1 Switch��R
	LB_BUTTON = 0x0100,//PS��L1 Switch��L
};

enum class MouseButton
{
	MOUSE_BUTTON_LEFT,
	MOUSE_BUTTON_RIGHT,
	MOUSE_BUTTON_CENTER,
};

class Input
{
private:

	//�L�[�����������ɓ��͕�����Ԃ����߂̔z��
	static std::vector<std::unordered_map<UCHAR, char>>returnChars;

	Input() {}
	~Input() {}

#pragma region DirectInput


	static HINSTANCE hInstance;
	static HRESULT dIResult;//���ʎ擾
	static LPDIRECTINPUT8 dInput8;//dxInput������ϐ�
	static LPDIRECTINPUTDEVICE8 keyDev;//�f�o�C�X������ϐ�

	static IDirectInput8* dInputDevice;
	static IDirectInputDevice8* devKeyBoard;
	static IDirectInputDevice8* devMouse;


	//���݂̃L�[���
	static BYTE keysState[256];
	//1�t���[���O�̃L�[���
	static BYTE preKeysState[256];

	//���݉�����Ă���L�[
	static std::vector<BYTE> currentPressKeys;

	//�}�E�X
	static DIMOUSESTATE mouseState;
	static DIMOUSESTATE mousePrevious;


	//�擾�����E�B���h�E�n���h��
	static HWND mHwnd;

#pragma endregion

#pragma region XInput


	static XINPUT_STATE padState[4];
	static XINPUT_STATE padPrevious[4];

	static bool padConnected[4];

	//���̍ō��l
	static const short MAX_AXIS_VALUE = 32767;
	//�U���̍ō��l
	static const WORD MAX_VIBRATION_VALUE = 65535;
#pragma endregion

	//�s��
	static DirectX::XMMATRIX viewMat;
	static DirectX::XMMATRIX projectionMat;

	//�J����
	static float nearNum;
	static float farNum;


	//�E�B���h�E�T�C�Y
	static int winWidth;
	static int winHeight;

	/// <summary>
	/// �p�b�h�ԍ���3�𒴂��Ă��Ȃ����ƁA�ڑ�����Ă��邩���m�F���܂��B
	/// </summary>
	/// <param name="padNum"></param>
	/// <returns></returns>
	static bool PadCheck(const UCHAR padNum);


public:
	static void Initialize(HWND hwnd,const int windowWidth,const int windowHeight);
	static void Update();
	static void Finitialize();

	static bool GetPadConnectedFlag(const UCHAR padNum);


#pragma region �L�[�{�[�h

	//�����Ă��邩
	static bool KeyState(const BYTE keyDef);
	//�������u�Ԃ�
	static bool KeyTrigger(const BYTE keyDef);
	//�������u�Ԃ�
	static bool KeyRelease(const BYTE keyDef);

	/// <summary>
	/// �L�[�̕�����Ԃ��܂�
	/// </summary>
	/// <returns></returns>
	static std::vector<char> GetPressKeyChars();

	/// <summary>
	/// ���݉�����Ă���L�[���擾���܂��B
	/// </summary>
	/// <returns></returns>
	static std::vector<BYTE> GetStateKeys() { return currentPressKeys; }

	/// <summary>
	/// �A���[�L�[�����x�������Ă��邩���p�x�ŕԂ��܂��B	
	/// </summary>
	/// <returns></returns>
	static float ArrowKeyAngle();
#pragma endregion

#pragma region �}�E�X


#pragma region �J�[�\��

	
	static void SetViewMatrix(const DirectX::XMMATRIX& viewMatrix) { viewMat = viewMatrix; }
	static void SetProjectionMatrix(const DirectX::XMMATRIX& projectionMatrix) { projectionMat = projectionMatrix; }
	static void SetNear(const float nearNumber) { nearNum = nearNumber; }
	static void SetFar(const float farNumber) { farNum = farNumber; }

	/// <summary>
	/// �N���C�A���g���W�ɕϊ����A�ŏ��ɐݒ肵���E�B���h�E�T�C�Y�ɉ����ĕ␳�����}�E�X�̍��W���擾���܂�
	/// </summary>
	/// <returns></returns>
	static Vector2 GetMousePosition();

	/// <summary>
	/// �E�B���h�E�̒��S����Ƃ����J�[�\���̃x�N�g�����擾���܂��B
	/// </summary>
	/// <returns>�x�N�g��</returns>
	static Vector2 GetCenterToMouseVector();

	/// <summary>
	/// �E�B���h�E�̒��S����Ƃ����J�[�\���̊p�x���擾���܂��B
	/// </summary>
	/// <returns>�p�x</returns>
	static float GetMouseAngle();
#pragma endregion


#pragma region �{�^��


	/// <summary>
	/// �}�E�X�J�[�\������o�Ă�������̍ŋߓ_�ƍŉ��_���擾���܂�
	/// </summary>
	/// <param name="nearPoint"></param>
	/// <param name="farPoint"></param>
	static void GetMouse3DLine(Vector3& nearPoint, Vector3& farPoint);

	static bool MouseButtonState(const MouseButton mouseButton);

	static bool MouseButtonTrigger(const MouseButton mouseButton);

	static bool MouseButtonRelease(const MouseButton mouseButton);


#pragma endregion

#pragma endregion

#pragma region �Q�[���p�b�h


#pragma region �{�^��
	static bool ButtonState(const XInputButton button, const UCHAR padNum);
	static bool ButtonTrigger(const XInputButton button, const UCHAR padNum);
	static bool ButtonRelease(const XInputButton button, const UCHAR padNum);

	/// <summary>
	/// �\���{�^���������p�x���擾���܂��B
	/// </summary>
	/// <returns></returns>
	static float DirectionalButtonAngle(const UCHAR padNum);
#pragma endregion

#pragma region �A�i���O�X�e�B�b�N
	static bool LeftStickLeft(const float lXPar, const UCHAR padNum);
	static bool LeftStickRight(const float lXPar, const UCHAR padNum);
	static bool LeftStickUp(const float lYPar, const UCHAR padNum);
	static bool LeftStickDown(const float lYPar, const UCHAR padNum);
	static bool RightStickLeft(const float lXPar, const UCHAR padNum);
	static bool RightStickRight(const float lXPar, const UCHAR padNum);
	static bool RightStickUp(const float lYPar, const UCHAR padNum);
	static bool RightStickDown(const float lYPar, const UCHAR padNum);

	/// <summary>
	/// ���X�e�B�b�N�����x�����p�x�ŕԂ��܂��B�X�e�B�b�N���X���Ă��Ȃ��ꍇ�A-1��Ԃ��܂��B
	/// </summary>
	/// <returns></returns>
	static float LeftStickAngle(const UCHAR padNum);

	/// <summary>
	/// �E�X�e�B�b�N�����x�����p�x�ŕԂ��܂��B�X�e�B�b�N���X���Ă��Ȃ��ꍇ�A-1��Ԃ��܂��B
	/// </summary>
	/// <returns></returns>
	static float RightStickAngle(const UCHAR padNum);
#pragma endregion

#pragma region �U��
	/// <summary>
	/// �Q�[���p�b�h��U�������܂�
	/// </summary>
	/// <param name="leftPar">�����̐U����(0%�`100%)</param>
	/// <param name="rightPar">�E���̐U����(0%�`100%)</param>
	/// <param name="padNum">�p�b�h�ԍ�(1P��2P�̂���)</param>
	static void PadVibration(const float leftPar, const float rightPar, const UCHAR padNum);

#pragma endregion
#pragma endregion

};

