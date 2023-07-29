#pragma once
#include<string>
#include<map>
#include<unordered_map>
#include<vector>
#include<typeinfo>
#include"GameObject.h"

#include"GuiValue.h"


// valueDatasか各データのmapどちらかだけでいい
// valueDatasと各データの中身は別物だから消すな

// valueDataいらない
// 各データから値取り出して書き込めばいい

namespace MelLib
{
	// .guid(GUI Data) 仕様
	// ファイル名はウィンドウ名
	// 
	// 内部(ラベル分存在)
	// ラベル(HPなどのパラメーター名)
	// 区切り(char型の-1。文字列ではない)
	// 名前復元用乱数(2023_04_19一旦廃止)
	// 値の型
	// 値データ
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
		void Save(const std::string& windowName, const std::string& lavel, const char*& data, const type_info& type,size_t dataSize);
		

		void Load();
		void AllSetLoadData();
	private:

		static const std::string DATA_FORMAT;
		static const std::unordered_map<std::string, char>DATA_FORMAT_STR;



		// std::map < Window名, std::map<ラベル名, データ(.guidの中身全部)>>
		// データ
		std::unordered_map < std::string, std::unordered_map<std::string, std::string>>valueDatas;
		// 追加した順序
		std::unordered_map < std::string, std::vector<std::string>>addOrders;
		//[オブジェクト名][ラベル名]
		std::unordered_map<std::string, std::unordered_map<std::string, GuiInt*>>intValues;
		std::unordered_map<std::string, std::unordered_map<std::string, GuiFloat*>>floatValues;
		std::unordered_map<std::string, std::unordered_map<std::string, GuiVector3*>>vector3Values;
		std::unordered_map<std::string, std::unordered_map<std::string, GuiBool*>>boolValues;

		// 生成したウィンドウと描画フラグを確認するためのフラグ
		std::unordered_map < std::string, bool >drawWindowFlag;
		std::unordered_map < std::string, bool >typingInputFlag;

		int* castIntData(const std::string& data);
		float* castFloatData(const std::string& data);
		Vector3* castVector3Data(const std::string& data);
		bool* castBoolData(const std::string& data);

		std::unordered_map <std::string, MelLib::Vector2>windowPositions;
		std::unordered_map <std::string, MelLib::Vector2>windowSizes;
		std::unordered_map <std::string, bool>windowPositionFixedFlags;
		std::unordered_map <std::string, bool>windowSizeFixedFlags;

	public:
		GuiValueManager(GuiValueManager& m) = delete;
		GuiValueManager& operator=(GuiValueManager& m) = delete;
		static GuiValueManager* GetInstance();

		void AddGuiValue(GuiInt* pGuiValue, const std::string& windowName,const std::string& lavel);
		void AddGuiValue(GuiFloat* pGuiValue, const std::string& windowName,const std::string& lavel);
		void AddGuiValue(GuiVector3* pGuiValue, const std::string& windowName,const std::string& lavel);
		void AddGuiValue(GuiBool* pGuiValue, const std::string& windowName,const std::string& lavel);

		void EraseGuiValue(const type_info& type,const std::string& windowName, const std::string& lavel);
		void DeleteWindow(const std::string& windowName);

		void Initialize();
		void Update();

		/// <summary>
		/// GUIのデータを出力します。
		/// </summary>
		/// <returns></returns>
		void Export();

		/// <summary>
		/// Guiの値objectからobject2にコピーします。
		/// </summary>
		/// <param name="object"></param>
		//void CopyGuiValue(const GameObject& object,const GameObject& object2);
		
		/// <summary>
		/// ウィンドウ1の値をウィンドウ2のラベルが一致する値にコピーします。
		/// </summary>
		/// <param name="windowName1"></param>
		/// <param name="windowName2"></param>
		void CopyGuiValue(const std::string& windowName1, const std::string& windowName2);

		/// <summary>
		/// ウィンドウ名を変更します。
		/// </summary>
		/// <param name="windowName"></param>
		void ChangeWindowName(const std::string& windowName,const std::string& newWindowName);

		/// <summary>
		/// 保存されているGUIのデータを読み込みます。
		/// </summary>
		/// <param name="windowName"></param>
		void LoadGUIFileData(const std::string& windowName);

		void SetTypingInputFlag(const std::string& windowName,bool flag);
		void ChangeTypingInputFlag(const std::string& windowName);
		bool GetTypingInputFlag(const std::string& windowName)const;

		bool GetGuiData(GuiInt* pGuiValue, int& refInt, const std::string& windowName, const std::string& lavel);
		bool GetGuiData(GuiFloat* pGuiValue, float& refFloat,const std::string& windowName, const std::string& lavel);
		bool GetGuiData(GuiBool* pGuiValue, bool& refFlag, const std::string& windowName, const std::string& lavel);
		bool GetGuiData(GuiVector3* pGuiValue, Vector3& refVectior3,const std::string& windowName, const std::string& lavel);

		void SetDrawWindowFlag(const std::string& windowName, bool flag) { drawWindowFlag[windowName] = flag; }
		void ChangeDrawWindowFlag(const std::string& windowName) { drawWindowFlag[windowName] = !drawWindowFlag[windowName]; }

		void SetDrawWindowFlagAll(bool flag);

		void SetPosition(const MelLib::Vector2& pos, const std::string& windowName) { windowPositions[windowName] = pos; }
		void SetFixedPosition(const bool flag, const std::string& windowName) { windowPositionFixedFlags[windowName] = flag; }
		void SetSize(const MelLib::Vector2& size, const std::string& windowName) { windowSizes[windowName] = size; }
		void SetFixedSize(const bool flag, const std::string& windowName) { windowSizeFixedFlags[windowName] = flag; }
	};

}