#pragma once
#include<string>
#include<unordered_map>
#include<vector>
#include<typeinfo>

#include"GuiValue.h"

namespace MelLib
{
	class GuiValueManager
	{
	private:
		GuiValueManager() {}
		~GuiValueManager() {}

		// createWindowNames�ɓn���ꂽ���O���Ȃ������ꍇ�ǉ�
		void AddCreateWindowName(const std::string& windowName);
	private:
		// bool�^�����

		std::unordered_map<std::string, std::unordered_map<std::string, GuiInt*>>intValues;
		std::unordered_map<std::string, std::unordered_map<std::string, GuiFloat*>>floatValues;

		std::vector<std::string>createWindowNames;


	public:
		GuiValueManager(GuiValueManager& m) = delete;
		GuiValueManager& operator=(GuiValueManager& m) = delete;
		static GuiValueManager* GetInstance();

		void AddGuiValue(GuiInt* pGuiValue, const std::string& windowName,const std::string& lavel);
		void AddGuiValue(GuiFloat* pGuiValue, const std::string& windowName,const std::string& lavel);

		void EraseGuiValue(const type_info& type,const std::string& windowName, const std::string& lavel);

		void Update();

	};

}