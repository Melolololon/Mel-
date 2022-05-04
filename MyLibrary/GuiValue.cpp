#include "GuiValue.h"

#include"GuiValueManager.h"

MelLib::GuiInt::GuiInt(const int value,const std::string& windowName, const std::string& lavel, int minNumber, int maxNumber)
	:value(value)
	, MIN_VALUE(minNumber)
	, MAX_VALUE(maxNumber)
	, SET_VALUE(value)
	, WINDOW_NAME(windowName)
	, LAVEL(lavel)
{
	GuiValueManager::GetInstance()->AddGuiValue(this, windowName, lavel);
}

MelLib::GuiInt::~GuiInt() 
{
	GuiValueManager::GetInstance()->EraseGuiValue(typeid(int), WINDOW_NAME, LAVEL);
}

MelLib::GuiFloat::GuiFloat(float number, const std::string& windowName, const std::string& lavel, float minNumber, float maxNumber)
	:value(value)
	, MIN_VALUE(minNumber)
	, MAX_VALUE(maxNumber)
	, SET_VALUE(value)
	, WINDOW_NAME(windowName)
	, LAVEL(lavel)
{
	GuiValueManager::GetInstance()->AddGuiValue(this, windowName, lavel);
}

MelLib::GuiFloat::~GuiFloat()
{
	GuiValueManager::GetInstance()->EraseGuiValue(typeid(float), WINDOW_NAME, LAVEL);
}
