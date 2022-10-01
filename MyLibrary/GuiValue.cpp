#include "GuiValue.h"

#include"GuiValueManager.h"

MelLib::GuiInt::GuiInt(const int value,const std::string& windowName, const std::string& lavel, int minNumber, int maxNumber)
	
{
	SetData(value, windowName, lavel, minNumber, maxNumber);
}

MelLib::GuiInt::~GuiInt() 
{
	GuiValueManager::GetInstance()->EraseGuiValue(typeid(int), windowName, lavel);
}

void MelLib::GuiInt::SetData(int value, const std::string& windowName, const std::string& lavel, int minNumber, int maxNumber)
{
	bool result = GuiValueManager::GetInstance()->GetGuiData(this, this->value, windowName, lavel);
	if (!result) GuiValueManager::GetInstance()->AddGuiValue(this, windowName, lavel);

	this->value = value;
	valueMin = minNumber;
	valueMax = maxNumber;
	this->windowName = windowName;
	this->lavel = lavel;
}

void MelLib::GuiInt::SetLoadData()
{
	GuiValueManager::GetInstance()->GetGuiData(this, value, windowName, lavel);
}

MelLib::GuiFloat::GuiFloat(const float value, const std::string& windowName, const std::string& lavel, float minNumber, float maxNumber)
{
	SetData(value, windowName, lavel, minNumber, maxNumber);
}

MelLib::GuiFloat::~GuiFloat()
{
	GuiValueManager::GetInstance()->EraseGuiValue(typeid(float), windowName, lavel);
}

void MelLib::GuiFloat::SetData(float value, const std::string& windowName, const std::string& lavel, float minNumber, float maxNumber)
{
	bool result = GuiValueManager::GetInstance()->GetGuiData(this, this->value, windowName, lavel);
	if (!result) GuiValueManager::GetInstance()->AddGuiValue(this, windowName, lavel);

	this->value = value;
	valueMin = minNumber;
	valueMax = maxNumber;
	this->windowName = windowName;
	this->lavel = lavel;
}

void MelLib::GuiFloat::SetLoadData()
{
	GuiValueManager::GetInstance()->GetGuiData(this, this->value, windowName, lavel);
}

MelLib::GuiVector3::GuiVector3(const MelLib::Vector3& value, const std::string& windowName, const std::string& lavel, float minNumber, float maxNumber)
{
	SetData(value, windowName, lavel, minNumber, maxNumber);
}

MelLib::GuiVector3::~GuiVector3()
{
	GuiValueManager::GetInstance()->EraseGuiValue(typeid(MelLib::Vector3), windowName, lavel);
}

void MelLib::GuiVector3::SetData(const MelLib::Vector3& value, const std::string& windowName, const std::string& lavel, float minNumber, float maxNumber)
{
	bool result = GuiValueManager::GetInstance()->GetGuiData(this, this->value, windowName, lavel);
	if (!result) GuiValueManager::GetInstance()->AddGuiValue(this, windowName, lavel);

	this->value = value;
	valueMin = minNumber;
	valueMax = maxNumber;
	this->windowName = windowName;
	this->lavel = lavel;
}

void MelLib::GuiVector3::SetLoadData()
{
	GuiValueManager::GetInstance()->GetGuiData(this, this->value, windowName, lavel);
}

MelLib::GuiBool::GuiBool(const bool value, const std::string& windowName, const std::string& lavel)
{
	SetData(value, windowName, lavel);
}

MelLib::GuiBool::~GuiBool()
{
	GuiValueManager::GetInstance()->EraseGuiValue(typeid(bool), windowName, lavel);
}

void MelLib::GuiBool::SetData(bool value, const std::string& windowName, const std::string& lavel)
{
	bool result = GuiValueManager::GetInstance()->GetGuiData(this, this->value, windowName, lavel);
	if (!result) GuiValueManager::GetInstance()->AddGuiValue(this, windowName, lavel);

	this->value = value;
	this->windowName = windowName;
	this->lavel = lavel;
}

void MelLib::GuiBool::SetLoadData()
{
	GuiValueManager::GetInstance()->GetGuiData(this, value, windowName, lavel);
}

MelLib::GuiOption* MelLib::GuiOption::GetInstance()
{
	static GuiOption o;
	return &o;
}
