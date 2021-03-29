#include "XInputManager.h"
#include"LibMath.h"
#include<Windows.h>

XINPUT_STATE XInputManager::padState[4];
XINPUT_STATE XInputManager::padPrevious[4];
bool XInputManager::padConnected[4];


void XInputManager::update()
{
	for(int i = 0; i < XUSER_MAX_COUNT;i++)
	{
		padPrevious[i] = padState[i];
		if (XInputGetState(i, &padState[i]) != ERROR_SUCCESS)padConnected[i] = false;
		else padConnected[i] = true;
	}
}



bool XInputManager::getPadConnectedFlag(const UCHAR& padNum)
{
	return padConnected[padNum - 1];
}

bool XInputManager::padCheck(const UCHAR& padNum)
{
	if (padNum > 3)
	{
		OutputDebugString(L"パッド番号が3を超えています\n");
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


bool XInputManager::buttonState(const XInputButton& button, const UCHAR& padNum)
{
	if (!padCheck(padNum))return false;

	int num = padState[padNum - 1].Gamepad.wButtons & button;
	if (num == button)
		return true;

	return false;
}

bool XInputManager::buttonTrigger(const XInputButton& button, const UCHAR& padNum)
{
	if (!padCheck(padNum))return false;

	int num = padPrevious[padNum - 1].Gamepad.wButtons & button;
	if (num != button &&
		buttonState(button, padNum))
		return true;
	return false;
}

bool XInputManager::buttonRelease(const XInputButton& button, const UCHAR& padNum)
{
	if (!padCheck(padNum))return false;

	int num = padPrevious[padNum - 1].Gamepad.wButtons & button;
	if (num == button &&
		!buttonState(button, padNum))
		return true;
	return false;
}


#pragma endregion


#pragma region スティック

bool XInputManager::leftStickLeft(const short& lXNum, const UCHAR& padNum)
{
	if (!padCheck(padNum))return false;
	if (-lXNum >= padState[padNum - 1].Gamepad.sThumbLX)
	{
		return true;
	}
	return false;
}

bool XInputManager::leftStickRight(const short&  lXNum, const UCHAR& padNum)
{
	if (!padCheck(padNum))return false;

	if (lXNum <= padState[padNum - 1].Gamepad.sThumbLX)
	{
		return true;
	}
	return false;
}

bool XInputManager::leftStickUp(const short&  lYNum, const UCHAR& padNum)
{
	if (!padCheck(padNum))return false;

	if (lYNum <= padState[padNum - 1].Gamepad.sThumbLY)
	{
		return true;
	}
	return false;
}

bool XInputManager::leftStickDown(const short&  lYNum, const UCHAR& padNum)
{
	if (!padCheck(padNum))return false;

	if (-lYNum >= padState[padNum - 1].Gamepad.sThumbLY)
	{
		return true;
	}
	return false;
}

bool XInputManager::rightStickLeft(const short&  lXNum, const UCHAR& padNum)
{

	if (!padCheck(padNum))return false;

	if (-lXNum >=padState[padNum - 1].Gamepad.sThumbRX)
	{
		return true;
	}
	return false;
}

bool XInputManager::rightStickRight(const short&  lXNum, const UCHAR& padNum)
{

	if (!padCheck(padNum))return false;


	if (lXNum <= padState[padNum - 1].Gamepad.sThumbRX)
	{
		return true;
	}
	return false;
}

bool XInputManager::rightStickUp(const short&  lYNum, const UCHAR& padNum)
{

	if (!padCheck(padNum))return false;


	if (lYNum <=padState[padNum - 1].Gamepad.sThumbRY)
	{
		return true;
	}
	return false;
}

bool XInputManager::rightStickDown(const short&  lYNum, const UCHAR& padNum)
{
	if (!padCheck(padNum))return false;

	if (-lYNum >= padState[padNum - 1].Gamepad.sThumbRY)
	{
		return true;
	}
	return false;
}


float XInputManager::leftStickAngle(const UCHAR& padNum)
{
	if (!padCheck(padNum))return -1.0f;

	float x = static_cast<float>(padState[padNum - 1].Gamepad.sThumbLX);
	float y = static_cast<float>(padState[padNum - 1].Gamepad.sThumbLY);
	const short MAX_AXIS_NUMBER = 32767;
	x /= MAX_AXIS_NUMBER;
	y /= MAX_AXIS_NUMBER;
	
	if (x == 0 && y == 0)return -1.0f;
	
	return LibMath::vecto2ToAngle(vector2Normalize({ x,y }),true);
}

float XInputManager::rightStickAngle(const UCHAR& padNum)
{
	if (!padCheck(padNum))return -1.0f;

	float x = static_cast<float>(padState[padNum - 1].Gamepad.sThumbRX);
	float y = static_cast<float>(padState[padNum - 1].Gamepad.sThumbRY);
	const short MAX_AXIS_NUMBER = 32767;
	x /= MAX_AXIS_NUMBER;
	y /= MAX_AXIS_NUMBER;

	if (x == 0 && y == 0)return -1.0f;

	return LibMath::vecto2ToAngle(vector2Normalize({ x,y }),true);
}

void XInputManager::padVibration(const float& leftPar, const float& rightPar, const UCHAR& padNum)
{
	if (leftPar > 100 || rightPar > 100)
	{
		OutputDebugString(L"%の範囲を超えています\n");
		return;
	}
	if (!padCheck(padNum))return;

	const WORD MAX_VIBRATION_NUMBER = 65535;
	XINPUT_VIBRATION v;
	v.wLeftMotorSpeed = static_cast<WORD>(leftPar / 100.0f) * MAX_VIBRATION_NUMBER;
	v.wRightMotorSpeed = static_cast<WORD>(rightPar / 100.0f) * MAX_VIBRATION_NUMBER;
	XInputSetState(padNum - 1, &v);
}

#pragma endregion

