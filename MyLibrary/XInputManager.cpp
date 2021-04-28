#include "XInputManager.h"
#include"LibMath.h"
#include<Windows.h>

XINPUT_STATE XInputManager::padState[4];
XINPUT_STATE XInputManager::padPrevious[4];
bool XInputManager::padConnected[4];

const short XInputManager::MAX_AXIS_VALUE = 32767;
const WORD XInputManager::MAX_VIBRATION_VALUE = 65535;

void XInputManager::Update()
{
	for(int i = 0; i < XUSER_MAX_COUNT;i++)
	{
		padPrevious[i] = padState[i];
		if (XInputGetState(i, &padState[i]) != ERROR_SUCCESS)padConnected[i] = false;
		else padConnected[i] = true;
	}
}



bool XInputManager::GetPadConnectedFlag(const UCHAR& padNum)
{
	return padConnected[padNum - 1];
}

bool XInputManager::padCheck(const UCHAR& padNum)
{
	if (padNum > 3)
	{
		OutputDebugString(L"パッド番号が4を超えています\n");
		return false;
	}
	if (!padConnected[padNum - 1])
	{
		std::wstring padNumWStr = std::to_wstring(padNum);
		padNumWStr += L"Pコントローラーが接続されていません\n";
		OutputDebugString(padNumWStr.c_str());
		return false;
	}
		return true;
}

#pragma region ボタン


bool XInputManager::ButtonState(const XInputButton& button, const UCHAR& padNum)
{
	if (!padCheck(padNum))return false;

	int num = padState[padNum - 1].Gamepad.wButtons & button;
	if (num == button)
		return true;

	return false;
}

bool XInputManager::ButtonTrigger(const XInputButton& button, const UCHAR& padNum)
{
	if (!padCheck(padNum))return false;

	int num = padPrevious[padNum - 1].Gamepad.wButtons & button;
	if (num != button &&
		ButtonState(button, padNum))
		return true;

	return false;
}

bool XInputManager::ButtonRelease(const XInputButton& button, const UCHAR& padNum)
{
	if (!padCheck(padNum))return false;

	int num = padPrevious[padNum - 1].Gamepad.wButtons & button;
	if (num == button &&
		!ButtonState(button, padNum))
		return true;

	return false;
}

float XInputManager::DirectionalButtonAngle(const UCHAR& padNum)
{
	if (!padCheck(padNum))return -1.0f;

	bool right = XInputManager::ButtonState(XINPUT_RIGHT_BUTTON, padNum);
	bool left = XInputManager::ButtonState(XINPUT_LEFT_BUTTON, padNum);
	bool up = XInputManager::ButtonState(XINPUT_UP_BUTTON, padNum);
	bool down = XInputManager::ButtonState(XINPUT_DOWN_BUTTON, padNum);

	if (right && up)return 45.0f;
	if (up && left)return 135.0f;
	if (left && down)return 225.0f;
	if (down && right)return 315.0f;

	if (right)return 0.0f;
	if (up)return 90.0f;
	if (left)return 180.0f;
	if (down)return 270.0f;

	return -1.0f;
}

#pragma endregion


#pragma region スティック

bool XInputManager::LeftStickLeft(const float& lXPar, const UCHAR& padNum)
{
	if (!padCheck(padNum))return false;
	if (-lXPar / 100.0f * MAX_AXIS_VALUE >= padState[padNum - 1].Gamepad.sThumbLX)
		return true;
	
	return false;
}

bool XInputManager::LeftStickRight(const float&  lXPar, const UCHAR& padNum)
{
	if (!padCheck(padNum))return false;

	if (lXPar / 100.0f * MAX_AXIS_VALUE <= padState[padNum - 1].Gamepad.sThumbLX)
		return true;
	
	return false;
}

bool XInputManager::LeftStickUp(const float&  lYPar, const UCHAR& padNum)
{
	if (!padCheck(padNum))return false;

	if (lYPar / 100.0f * MAX_AXIS_VALUE <= padState[padNum - 1].Gamepad.sThumbLY)
		return true;
	
	return false;
}

bool XInputManager::LeftStickDown(const float&  lYPar, const UCHAR& padNum)
{
	if (!padCheck(padNum))return false;

	if (-lYPar / 100.0f * MAX_AXIS_VALUE >= padState[padNum - 1].Gamepad.sThumbLY)
		return true;
	
	return false;
}

bool XInputManager::RightStickLeft(const float&  lXPar, const UCHAR& padNum)
{

	if (!padCheck(padNum))return false;

	if (-lXPar / 100.0f * MAX_AXIS_VALUE >=padState[padNum - 1].Gamepad.sThumbRX)
		return true;
	
	return false;
}

bool XInputManager::RightStickRight(const float&  lXPar, const UCHAR& padNum)
{

	if (!padCheck(padNum))return false;
	if (lXPar / 100.0f * MAX_AXIS_VALUE <= padState[padNum - 1].Gamepad.sThumbRX)
		return true;
	
	return false;
}

bool XInputManager::RightStickUp(const float&  lYPar, const UCHAR& padNum)
{

	if (!padCheck(padNum))return false;

	if (lYPar / 100.0f * MAX_AXIS_VALUE <=padState[padNum - 1].Gamepad.sThumbRY)
		return true;
	
	return false;
}

bool XInputManager::RightStickDown(const float&  lYPar, const UCHAR& padNum)
{
	if (!padCheck(padNum))return false;

	if (-lYPar / 100.0f * MAX_AXIS_VALUE >= padState[padNum - 1].Gamepad.sThumbRY)
		return true;
	
	return false;
}


float XInputManager::LeftStickAngle( const UCHAR& padNum)
{
	if (!padCheck(padNum))return -1.0f;

	float x = static_cast<float>(padState[padNum - 1].Gamepad.sThumbLX);
	float y = static_cast<float>(padState[padNum - 1].Gamepad.sThumbLY);
	
	//未入力はリターン
	if (x == 0 && y == 0)
		return -1.0f;


	return LibMath::Vecto2ToAngle(Vector2Normalize({ x,y }), true);
}

float XInputManager::RightStickAngle( const UCHAR& padNum)
{
	if (!padCheck(padNum))return -1.0f;

	float x = static_cast<float>(padState[padNum - 1].Gamepad.sThumbRX);
	float y = static_cast<float>(padState[padNum - 1].Gamepad.sThumbRY);
	

	if (x == 0 && y == 0 )
		return -1.0f;


	return LibMath::Vecto2ToAngle(Vector2Normalize({ x,y }),true);
}

#pragma endregion


#pragma region 振動

void XInputManager::PadVibration(const float& leftPar, const float& rightPar, const UCHAR& padNum)
{
	if (leftPar > 100 || rightPar > 100)
	{
		OutputDebugString(L"leftParまたはrightParが0%～100%の範囲を超えています\n");
		return;
	}
	if (!padCheck(padNum))return;

	XINPUT_VIBRATION v;
	v.wLeftMotorSpeed = static_cast<WORD>(leftPar / 100.0f) * MAX_VIBRATION_VALUE;
	v.wRightMotorSpeed = static_cast<WORD>(rightPar / 100.0f) * MAX_VIBRATION_VALUE;
	XInputSetState(padNum - 1, &v);
}

#pragma endregion

