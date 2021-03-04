#include "XInputManager.h"

XINPUT_STATE XInputManager::padState[4];
XINPUT_STATE XInputManager::padPrevious[4];
bool XInputManager::padSet[4];


void XInputManager::update()
{
	for(int i = 0; i <XUSER_MAX_COUNT;i++)
	{
		padPrevious[i] = padState[i];
		//padSet[i] = XInputGetState(i, &padState[i]);
		auto a = XInputGetState(i, &padState[i]);
		if (a == 1167)padSet[i] = false;
		else padSet[i] = true;
	}
}



bool XInputManager::getPadConnectFlag(const int& padNum)
{
	return padSet[padNum - 1];
}

#pragma region ボタン


bool XInputManager::buttonState(const XInputButton& button, const int& padNum)
{
	if (!padSet[padNum - 1])return false;

	int num = padState[padNum - 1].Gamepad.wButtons & button;
	if (num == button)
		return true;

	return false;
}

bool XInputManager::buttonTrigger(const XInputButton& button, const int& padNum)
{
	if (!padSet[padNum - 1])return false;

	int num = padPrevious[padNum - 1].Gamepad.wButtons & button;
	if (num != button &&
		buttonState(button, padNum))
		return true;
	return false;
}

bool XInputManager::buttonRelease(const XInputButton& button, const int& padNum)
{
	if (!padSet[padNum - 1])return false;

	int num = padPrevious[padNum - 1].Gamepad.wButtons & button;
	if (num == button &&
		!buttonState(button, padNum))
		return true;
	return false;
}


#pragma endregion


#pragma region スティック

bool XInputManager::leftStickLeft(const int& lXNum, const int& padNum)
{
	if (!padSet[padNum - 1])return false;
	if (-lXNum >= padState[padNum - 1].Gamepad.sThumbLX)
	{
		return true;
	}
	return false;
}

bool XInputManager::leftStickRight(const int&  lXNum, const int& padNum)
{
	if (!padSet[padNum - 1])return false;

	if (lXNum <= padState[padNum - 1].Gamepad.sThumbLX)
	{
		return true;
	}
	return false;
}

bool XInputManager::leftStickUp(const int&  lYNum, const int& padNum)
{
	if (!padSet[padNum - 1])return false;

	if (lYNum <= padState[padNum - 1].Gamepad.sThumbLY)
	{
		return true;
	}
	return false;
}

bool XInputManager::leftStickDown(const int&  lYNum, const int& padNum)
{
	if (!padSet[padNum - 1])return false;

	if (-lYNum >= padState[padNum - 1].Gamepad.sThumbLY)
	{
		return true;
	}
	return false;
}

bool XInputManager::rightStickLeft(const int&  lXNum, const int& padNum)
{
	if (!padSet[padNum - 1])return false;
	if (-lXNum >=padState[padNum - 1].Gamepad.sThumbRX)
	{
		return true;
	}
	return false;
}

bool XInputManager::rightStickRight(const int&  lXNum, const int& padNum)
{
	if (!padSet[padNum - 1])return false;

	if (lXNum <= padState[padNum - 1].Gamepad.sThumbRX)
	{
		return true;
	}
	return false;
}

bool XInputManager::rightStickUp(const int&  lYNum, const int& padNum)
{
	if (!padSet[padNum - 1])return false;

	if (lYNum <=padState[padNum - 1].Gamepad.sThumbRY)
	{
		return true;
	}
	return false;
}

bool XInputManager::rightStickDown(const int&  lYNum, const int& padNum)
{
	if (-lYNum >= padState[padNum - 1].Gamepad.sThumbRY)
	{
		return true;
	}
	return false;
}

#pragma endregion

