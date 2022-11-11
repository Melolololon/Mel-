#pragma once
#include<string>
#include<unordered_map>
#include<vector>

#include"Vector.h"

// structでもいい?
// 設定した値を書き出せるように、ファイルはウィンドウごとでいい?
// テストプレイ中は変数の値が変わっても影響受けないように
// テストプレイ中は変数の値変えられるけど、エディットに戻ったらSET_VALUEに戻すようにする
// 読み込んだ時にソースコードと値が一致しなかったら読み込んだファイルの値を優先(Unityと同じにする)

// コンストラクタでデータがあったら読み込むようにする

namespace MelLib 
{

	class GuiOption
	{
	private:
		std::string inOutPath;

		GuiOption(){}
		~GuiOption() {}
	public:
		GuiOption(GuiOption& o) = delete;
		GuiOption& operator=(GuiOption& o) = delete;
		static GuiOption* GetInstance();

		/// <summary>
		/// Guiで設定した値の情報の入出力先を設定します。
		/// </summary>
		/// <param name="path"></param>
		void SetGuiDataPath(const std::string& path) { inOutPath = path; }
		std::string GetGuiDataPath() { return inOutPath; }

	};

	// UnityのSerializeFieldみたいなやつ
	class GuiInt
	{
	private:
		int value = 0;
		int valueMin = 0;
		int valueMax = 0;
		int setValue = 0;
		std::string windowName;
		std::string lavel;
		
	
		bool typingInput = false;
	public:
		GuiInt() {}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="value"></param>
		/// <param name="windowName"></param>
		/// <param name="lavel"></param>
		/// <param name="minNumber"></param>
		/// <param name="maxNumber"></param>GuiInt(int value, const std::string& windowName, const std::string& lavel,int minNumber,int maxNumber);
		GuiInt(const int value, const std::string& windowName, const std::string& lavel, int minNumber, int maxNumber);
		~GuiInt();

		void SetData(int value, const std::string& windowName, const std::string& lavel, int minNumber, int maxNumber);
		
		void operator=(const int num) { this->value = num; }
		GuiInt& operator=(const GuiInt& value)
		{
			*this = value;
			return *this;
		}

		GuiInt& operator+=(const GuiInt& value)
		{
			this->value += value.GetValue();
			return *this;
		}

		void operator++() { value++; }
		void operator--() { value--; }

		
		int GetValue()const { return value; }
		int GetMaxValue() const { return valueMax; }
		int GetMinValue()const { return valueMin; }
		int& GetRefValue() { return value; }

		bool GetTypingInputValueFlag()const { return typingInput; }

		/// <summary>
		/// 読み込まれた値をセットします。読み込んだ値をセットしたい場合や
		/// ライブラリの初期化処理が始まる前にコンストラクタが呼び出されてしまい、
		/// データをセットできなかった場合にお使いください。
		/// </summary>
		void SetLoadData();

		void SetValue(int value) { this->value = value; }

		void SetTypingInputFlag(const bool flag) { typingInput = flag; }

	};

	class GuiFloat
	{
	private:
		float value = 0.0f;
		float valueMin = 0.0f;
		float valueMax = 0.0f;
		float setValue = 0.0f;
		std::string windowName;
		std::string lavel;

		bool typingInput = false;
	public:
		GuiFloat(){}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="value"></param>
		/// <param name="windowName"></param>
		/// <param name="lavel"></param>
		/// <param name="minNumber"></param>
		/// <param name="maxNumber"></param>
		GuiFloat(float value, const std::string& windowName, const std::string& lavel, float minNumber, float maxNumber);
		~GuiFloat();

		void SetData(float value, const std::string& windowName, const std::string& lavel, float minNumber, float maxNumber);

		GuiFloat& operator=(const float num) 
		{
			this->value = num; 
			return *this; 
		}

		GuiFloat& operator+=(const GuiFloat& value)
		{
			this->value += value.GetValue();
			return *this;
		}

		void operator++() { value++; }
		void operator--() { value--; }

		float GetValue()const { return value; }
		float GetMaxValue() const { return valueMax; }
		float GetMinValue()const { return valueMin; }

		float& GetRefValue() { return value; }
		bool GetTypingInputValueFlag()const { return typingInput; }
		/// <summary>
		/// 読み込まれた値をセットします。読み込んだ値をセットしたい場合や
		/// ライブラリの初期化処理が始まる前にコンストラクタが呼び出されてしまい、
		/// データをセットできなかった場合にお使いください。
		/// </summary>
		void SetLoadData();
		void SetValue(float value) { this->value = value; }
		void SetTypingInputFlag(const bool flag) { typingInput = flag; }
	};

	

	class GuiVector3
	{
	private:
		MelLib::Vector3 value = 0.0f;
		float valueMin = 0.0f;
		float valueMax = 0.0f;
		MelLib::Vector3 setValue = 0.0f;
		std::string windowName;
		std::string lavel;

		bool typingInput = false;
	public:
		GuiVector3(){}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="value"></param>
		/// <param name="windowName"></param>
		/// <param name="lavel"></param>
		/// <param name="minNumber"></param>
		/// <param name="maxNumber"></param>
		GuiVector3(const MelLib::Vector3& value, const std::string& windowName, const std::string& lavel, float minNumber, float maxNumber);
		~GuiVector3();

		void SetData(const MelLib::Vector3& value, const std::string& windowName, const std::string& lavel, float minNumber, float maxNumber);

		GuiVector3& operator=(const GuiVector3& value)
		{
			this->value = value.GetValue();
			return *this;
		}

		GuiVector3& operator=(const Vector3& value)
		{
			this->value = value;
			return *this;
		}

	
		GuiVector3& operator+=(const GuiVector3& value)
		{
			this->value += value.GetValue();
			return *this;
		}

		

	/*	void operator++() { value++; }
		void operator--() { value--; }*/

		MelLib::Vector3 GetValue()const { return value; }
		MelLib::Vector3& GetRefValue() { return value; }
		float GetMaxValue() const { return valueMax; }
		float GetMinValue()const { return valueMin; }
		bool GetTypingInputValueFlag()const { return typingInput; }
		
		/// <summary>
		/// 読み込まれた値をセットします。読み込んだ値をセットしたい場合や
		/// ライブラリの初期化処理が始まる前にコンストラクタが呼び出されてしまい、
		/// データをセットできなかった場合にお使いください。
		/// </summary>
		void SetLoadData();

		void SetValue(const Vector3& value) { this->value = value; }
		void SetTypingInputFlag(const bool flag) { typingInput = flag; }
	};

	/*Vector3 operator+(const GuiVector3& value, const GuiVector3& value2) { return value + value2; }
	Vector3 operator-(const GuiVector3& value, const GuiVector3& value2) { return value - value2; }
	Vector3 operator*(const GuiVector3& value, const GuiVector3& value2) { return value * value2; }
	Vector3 operator/(const GuiVector3& value, const GuiVector3& value2) { return value / value2; }
	Vector3 operator+(const Vector3& value, const GuiVector3& value2) { return value + value2; }
	Vector3 operator-(const Vector3& value, const GuiVector3& value2) { return value - value2; }
	Vector3 operator*(const Vector3& value, const GuiVector3& value2) { return value * value2; }
	Vector3 operator/(const Vector3& value, const GuiVector3& value2) { return value / value2; }*/

	class GuiBool
	{
	private:
		bool value = false;
		bool setValue = false;
		std::string windowName;
		std::string lavel;

	public:
		GuiBool(){}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="value"></param>
		/// <param name="windowName"></param>
		/// <param name="lavel"></param>
		/// <param name="minNumber"></param>
		/// <param name="maxNumber"></param>
		GuiBool(bool value, const std::string& windowName, const std::string& lavel);
		~GuiBool();

		void SetData(bool value, const std::string& windowName, const std::string& lavel);

		GuiBool& operator=(const GuiBool& value)
		{
			this->value = value.GetValue();
			return *this;
		}

		GuiBool& operator=(const bool& value)
		{
			this->value = value;
			return *this;
		}

		

		bool GetValue()const { return value; }
		bool& GetRefValue() { return value; }

		/// <summary>
		/// 読み込まれた値をセットします。読み込んだ値をセットしたい場合や
		/// ライブラリの初期化処理が始まる前にコンストラクタが呼び出されてしまい、
		/// データをセットできなかった場合にお使いください。
		/// </summary>
		void SetLoadData();
		void SetValue(bool value) { this->value = value; }
	};
}