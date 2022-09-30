#pragma once
#include<string>
#include<map>
#include<unordered_map>
#include<vector>
#include<typeinfo>

#include"GuiValue.h"

// 起きたらやること 2022/09/28
// 登録、読み込みした順番にWindowで表示されるようにする。valueDatasを使って確認。
// GetGUIValueのキャスト処理をcastIntDataなどの関数に移動させてそれを呼び出すようにする。
// guidがあったらAddValueDataを呼び出さないまたは行わないようにする。
// 呼び出さなくすると現状AddValueDataに書かれているAddCreateWindowNameが呼び出されないため、
// AddCreateWindowNameの処理をguidがあった場合のみGetGUIDataでも行うようにする。

// valueDatasか各データのmapどちらかだけでいい
// valueDatasと各データの中身は別物だから消すな

namespace MelLib
{
	// .guid(GUI Data) 仕様
	// ファイル名はウィンドウ名
	// 
	// 内部(ラベル分存在)
	// ラベル(HPなどのパラメーター名)
	// 区切り-1
	// 名前復元用乱数
	// 値の型
	// 値
	// 終了判別値(-1がまだ続く。-2が末尾)


	class GuiValueManager
	{
	private:

		GuiValueManager();
		~GuiValueManager() {}

		// createWindowNamesに渡された名前がなかった場合追加
		void AddCreateWindowName(const std::string& windowName);


		/// <summary>
		/// 
		/// </summary>
		/// <param name="windowName">ウィンドウ名</param>
		/// <param name="lavel">ラベル</param>
		/// <param name="data">char*に変換したデータのポインタの参照</param>
		/// <param name="dataSize">データサイズ</param>
		/// <param name="refFlag">changeFlag(戻す用)</param>
		void Save(const std::string& windowName, const std::string& lavel, const char*& data, const type_info& type,size_t dataSize,bool& refFlag);

		void Load();

	private:

		static const std::string DATA_FORMAT;
		static const std::unordered_map<std::string, char>DATA_FORMAT_STR;



		// std::map < Window名, std::map<ラベル名, データ(.guidの中身全部)>>
		// 読み込んだデータ
		std::unordered_map < std::string, std::unordered_map<std::string, std::string>>valueDatas;
		// 追加した順序
		std::unordered_map < std::string, std::vector<std::string>>addOrders;
		//[オブジェクト名][ラベル名]
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