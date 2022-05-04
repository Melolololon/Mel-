#pragma once
#include<string>
#include<unordered_map>
#include<vector>
// structでもいい?
// 設定した値を書き出せるように、ファイルはウィンドウごとでいい?
// テストプレイ中(実行中)は変数の値が変わっても影響受けないように

namespace MelLib 
{
	// UnityのSerializeFieldみたいなやつ
	class GuiInt
	{
	private:
		int value = 0;
		const int MIN_VALUE = 0;
		const int MAX_VALUE = 0;
		const int SET_VALUE = 0;
		const std::string WINDOW_NAME;
		const std::string LAVEL;
		
	public:
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="number"></param>
		/// <param name="windowName"></param>
		/// <param name="lavel"></param>
		/// <param name="minNumber"></param>
		/// <param name="maxNumber"></param>
		GuiInt(int number, const std::string& windowName, const std::string& lavel,int minNumber,int maxNumber);
		~GuiInt();
		void operator=(int num) { this->value = num; }

		int GetValue()const { return value; }
		int GetMaxValue() const { return MAX_VALUE; }
		int GetMinValue()const { return MIN_VALUE; }

	};

	class GuiFloat
	{
	private:
		float value = 0.0f;
		const float MIN_VALUE = 0.0f;
		const float MAX_VALUE = 0.0f;
		const float SET_VALUE = 0.0f;
		const std::string WINDOW_NAME;
		const std::string LAVEL;

	public:

		/// <summary>
		/// 
		/// </summary>
		/// <param name="number"></param>
		/// <param name="windowName"></param>
		/// <param name="lavel"></param>
		/// <param name="minNumber"></param>
		/// <param name="maxNumber"></param>
		GuiFloat(float number, const std::string& windowName, const std::string& lavel, float minNumber, float maxNumber);
		~GuiFloat();
		void operator=(float num) { this->value = num; }

		float GetValue()const { return value; }
		float GetMaxValue() const { return MAX_VALUE; }
		float GetMinValue()const { return MIN_VALUE; }

	};
}