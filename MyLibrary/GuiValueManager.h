#pragma once
#include<string>
#include<map>
#include<unordered_map>
#include<vector>
#include<typeinfo>

#include"GuiValue.h"

// �N�������邱�� 2022/09/28
// �o�^�A�ǂݍ��݂������Ԃ�Window�ŕ\�������悤�ɂ���BvalueDatas���g���Ċm�F�B
// GetGUIValue�̃L���X�g������castIntData�Ȃǂ̊֐��Ɉړ������Ă�����Ăяo���悤�ɂ���B
// guid����������AddValueData���Ăяo���Ȃ��܂��͍s��Ȃ��悤�ɂ���B
// �Ăяo���Ȃ�����ƌ���AddValueData�ɏ�����Ă���AddCreateWindowName���Ăяo����Ȃ����߁A
// AddCreateWindowName�̏�����guid���������ꍇ�̂�GetGUIData�ł��s���悤�ɂ���B

// valueDatas���e�f�[�^��map�ǂ��炩�����ł���
// valueDatas�Ɗe�f�[�^�̒��g�͕ʕ������������

// valueData����Ȃ�
// �e�f�[�^����l���o���ď������߂΂���

namespace MelLib
{
	// .guid(GUI Data) �d�l
	// �t�@�C�����̓E�B���h�E��
	// 
	// ����(���x��������)
	// ���x��(HP�Ȃǂ̃p�����[�^�[��)
	// ��؂�-1
	// ���O�����p����
	// �l�̌^
	// �l
	// �I�����ʒl(-1���܂������B-2������)


	class GuiValueManager
	{
	private:

		GuiValueManager();
		~GuiValueManager() {}

		// createWindowNames�ɓn���ꂽ���O���Ȃ������ꍇ�ǉ�
		void AddCreateWindowName(const std::string& windowName);


		/// <summary>
		/// 
		/// </summary>
		/// <param name="windowName">�E�B���h�E��</param>
		/// <param name="lavel">���x��</param>
		/// <param name="data">char*�ɕϊ������f�[�^�̃|�C���^�̎Q��</param>
		/// <param name="dataSize">�f�[�^�T�C�Y</param>
		/// <param name="refFlag">changeFlag(�߂��p)</param>
		void Save(const std::string& windowName, const std::string& lavel, const char*& data, const type_info& type,size_t dataSize,bool& refFlag);
		void Export();

		void Load();
		void AllSetLoadData();
	private:

		static const std::string DATA_FORMAT;
		static const std::unordered_map<std::string, char>DATA_FORMAT_STR;



		// std::map < Window��, std::map<���x����, �f�[�^(.guid�̒��g�S��)>>
		// �f�[�^
		std::unordered_map < std::string, std::unordered_map<std::string, std::string>>valueDatas;
		// �ǉ���������
		std::unordered_map < std::string, std::vector<std::string>>addOrders;
		//[�I�u�W�F�N�g��][���x����]
		std::unordered_map<std::string, std::unordered_map<std::string, GuiInt*>>intValues;
		std::unordered_map<std::string, std::unordered_map<std::string, GuiFloat*>>floatValues;
		std::unordered_map<std::string, std::unordered_map<std::string, GuiVector3*>>vector3Values;
		std::unordered_map<std::string, std::unordered_map<std::string, GuiBool*>>boolValues;

		std::vector<std::string>createWindowNames;

		

		int* castIntData(const std::string& data);
		float* castFloatData(const std::string& data);
		Vector3* castVector3Data(const std::string& data);
		bool* castBoolData(const std::string& data);



	public:
		GuiValueManager(GuiValueManager& m) = delete;
		GuiValueManager& operator=(GuiValueManager& m) = delete;
		static GuiValueManager* GetInstance();

		void AddGuiValue(GuiInt* pGuiValue, const std::string& windowName,const std::string& lavel);
		void AddGuiValue(GuiFloat* pGuiValue, const std::string& windowName,const std::string& lavel);
		void AddGuiValue(GuiVector3* pGuiValue, const std::string& windowName,const std::string& lavel);
		void AddGuiValue(GuiBool* pGuiValue, const std::string& windowName,const std::string& lavel);

		void EraseGuiValue(const type_info& type,const std::string& windowName, const std::string& lavel);

		void Initialize();
		void Update();

		
		bool GetGuiData(GuiInt* pGuiValue, int& refInt, const std::string& windowName, const std::string& lavel);
		bool GetGuiData(GuiFloat* pGuiValue, float& refFloat,const std::string& windowName, const std::string& lavel);
		bool GetGuiData(GuiBool* pGuiValue, bool& refFlag, const std::string& windowName, const std::string& lavel);
		bool GetGuiData(GuiVector3* pGuiValue, Vector3& refVectior3,const std::string& windowName, const std::string& lavel);
	};

}