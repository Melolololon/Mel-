#include "GuiValueManager.h"
#include"ImguiManager.h"


void MelLib::GuiValueManager::AddCreateWindowName(const std::string& windowName)
{
	for (const auto& name : createWindowNames)
	{
		if (name == windowName)return;
	}
	createWindowNames.push_back(windowName);
}

MelLib::GuiValueManager* MelLib::GuiValueManager::GetInstance()
{
	static GuiValueManager m;
	return &m;
}

void MelLib::GuiValueManager::AddGuiValue(GuiInt* pGuiValue, const std::string& windowName, const std::string& lavel)
{
	intValues[windowName].try_emplace(lavel, pGuiValue);
	AddCreateWindowName(windowName);
	
}

void MelLib::GuiValueManager::AddGuiValue(GuiFloat* pGuiValue, const std::string& windowName, const std::string& lavel)
{
	floatValues[windowName].try_emplace(lavel, pGuiValue);
	AddCreateWindowName(windowName);
}

void MelLib::GuiValueManager::AddGuiValue(GuiVector3* pGuiValue, const std::string& windowName, const std::string& lavel)
{
	vector3Values[windowName].try_emplace(lavel, pGuiValue);
	AddCreateWindowName(windowName);
}

void MelLib::GuiValueManager::AddGuiValue(GuiBool* pGuiValue, const std::string& windowName, const std::string& lavel)
{
	boolValues[windowName].try_emplace(lavel, pGuiValue);
	AddCreateWindowName(windowName);
}

void MelLib::GuiValueManager::EraseGuiValue(const type_info& type, const std::string& windowName, const std::string& lavel)
{
	if (type == typeid(int)) 
	{
		intValues[windowName].erase(lavel);
		if (intValues[windowName].size() == 0)intValues.erase(windowName);
	}
	else if (type == typeid(float))
	{
		floatValues[windowName].erase(lavel);
		if (floatValues[windowName].size() == 0)floatValues.erase(windowName);
	}
	else if (type == typeid(MelLib::Vector3))
	{
		vector3Values[windowName].erase(lavel);
		if (vector3Values[windowName].size() == 0)vector3Values.erase(windowName);
	}
	else if (type == typeid(bool))
	{
		boolValues[windowName].erase(lavel);
		if (boolValues[windowName].size() == 0)boolValues.erase(windowName);
	}
}

void MelLib::GuiValueManager::Update()
{
	// 三種類の配列見て、Window名が一緒だったら1つのウィンドウにまとめる
	for (const auto& name : createWindowNames)
	{
		ImguiManager::GetInstance()->BeginDrawWindow(name);

		// int
		const std::unordered_map<std::string, GuiInt*>& refInts = intValues[name];
		for (auto& value : refInts) 
		{
			const std::string& LAVEL = value.first;
			GuiInt& guiInt = *value.second;
			int num = guiInt.GetValue();
			ImguiManager::GetInstance()->DrawSliderInt(LAVEL, num, guiInt.GetMinValue(), guiInt.GetMaxValue());
			guiInt = num;
		}

		// float 
		const std::unordered_map<std::string, GuiFloat*>& refFloats = floatValues[name];
		for (auto& value : refFloats)
		{
			const std::string& LAVEL = value.first;
			GuiFloat& guiFloat = *value.second;
			float num = guiFloat.GetValue();
			ImguiManager::GetInstance()->DrawSliderFloat(LAVEL, num, guiFloat.GetMinValue(), guiFloat.GetMaxValue());
			guiFloat = num;
		}

		// Vector3
		const std::unordered_map<std::string, GuiVector3*>& refVector3 = vector3Values[name];
		for (auto& value : refVector3)
		{
			const std::string& LAVEL = value.first;
			GuiVector3& guiVector3 = *value.second;
			MelLib::Vector3 num = guiVector3.GetValue();
			ImguiManager::GetInstance()->DrawSliderVector3(LAVEL, num, guiVector3.GetMinValue(), guiVector3.GetMaxValue());
			guiVector3 = num;
		}

		// bool
		const std::unordered_map<std::string, GuiBool*>& refBool = boolValues[name];
		for (auto& value : refBool)
		{
			const std::string& LAVEL = value.first;
			GuiBool& guiBool = *value.second;
			bool flag = guiBool.GetValue();
			ImguiManager::GetInstance()->DrawCheckBox(LAVEL, flag);
			guiBool = flag;
		}

		ImguiManager::GetInstance()->EndDrawWindow();
	}
}
