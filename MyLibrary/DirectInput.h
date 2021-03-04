#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include<Windows.h>
#include<vector>
#include"Vector.h"

#include<DirectXMath.h>


#pragma region �p�b�h�̃{�^��
//����O���[�o���萔�ɂ���?
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
#pragma region �\���L�[
//enum�ɕς���
const int UpButton = 0;
const int RightButton = 1;
const int DownButton = 2;
const int LeftButton = 3;
#pragma endregion

#pragma region �}�E�X�{�^��

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
	//�񋓗p�R�[���o�b�N
	static BOOL CALLBACK diEnumDevicesCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
	static BOOL CALLBACK diEnumDevicesObjectCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

	static HINSTANCE hInstance;
	static HRESULT dIResult;//���ʎ擾
	static LPDIRECTINPUT8 dInput8;//dxInput������ϐ�
	static LPDIRECTINPUTDEVICE8 keyDev;//�f�o�C�X������ϐ�

	static IDirectInput8* dInputDevice;
	static IDirectInputDevice8* devKeyBoard;
	static IDirectInputDevice8* devMouse;
	static IDirectInputDevice8* devPad;

	//static IDirectInputDevice8* padSetUp;
	//static DIDEVICEOBJECTDATA* lpdidod;
	static std::vector<DIDEVICEINSTANCE>getEnum;
	static DIDEVCAPS padCaps;


	//�p�b�h
	static DIJOYSTATE padState;//�p�b�h���
	static DIJOYSTATE padPrevious;//�p�b�h���(1���[�v�O)


	//���݂̃L�[���
	static BYTE key[256];
	//1�t���[���O�̃L�[���
	static BYTE preKey[256];

	//�}�E�X
	static DIMOUSESTATE mouseState;
	static DIMOUSESTATE mousePrevious;


	//�擾�����E�B���h�E�n���h��
	static HWND mHwnd;

	//�p�b�h��F���������ǂ���
	static bool setPad;

	//�E�B���h�E�T�C�Y
	static int winWidth;
	static int winHeight;

	//�s��
	static DirectX::XMMATRIX viewMat;
	static DirectX::XMMATRIX projectionMat;

	//�J����
	static float nearNum;
	static float farNum;

public:


	//����
	static void initialize(HWND hwnd,int windowWidth,int windowHeight);
	//���擾
	static void update();

#pragma region �L�[�{�[�h

	//�����Ă��邩
	static bool keyState(int keyDef);
	//�������u�Ԃ�
	static bool keyTrigger(int keyDef);
	//�������u�Ԃ�
	static bool keyRelease(int keyDef);
#pragma endregion

#pragma region �p�b�h
#pragma region �ʏ�{�^��

	//�����Ă��邩
	static bool buttonState(int button);
	//�������u�Ԃ�
	static bool buttonTrigger(int button);
	//�������u�Ԃ�
	static bool buttonRelease(int button);
#pragma endregion

#pragma region �\���L�[

	static bool directionalButtonState(int button);
	static bool directionalButtonTrigger(int button);
#pragma endregion

#pragma endregion

#pragma region �X�e�B�b�N
	/// <summary>
	/// 
	/// </summary>
	/// <param name="lNum">��(32767���ō��l)</param>
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


	//DInput�̊J��
	static void release();

#pragma region �}�E�X

	static void setMatrixAndNearFar(DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix,float nearNumber ,float farNumber);

	/// <summary>
	/// �N���C�A���g���W�ɕϊ����A�ŏ��ɐݒ肵���E�B���h�E�T�C�Y�ɉ����ĕ␳�����}�E�X�̍��W���擾���܂�
	/// </summary>
	/// <returns></returns>
	static Vector2 getMousePosition();


	/// <summary>
	/// �}�E�X�J�[�\������o�Ă�������̍ŋߓ_�ƍŉ��_���擾���܂�
	/// </summary>
	/// <param name="nearPoint"></param>
	/// <param name="farPoint"></param>
	static void getMouse3DLine(Vector3& nearPoint,Vector3& farPoint);

	static bool mouseButtonState(MouseButton mouseButton);

	static bool mouseButtonTrigger(MouseButton mouseButton);
	
	static bool mouseButtonRelease(MouseButton mouseButton);
#pragma endregion



};



