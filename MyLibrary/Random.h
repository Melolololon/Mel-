#pragma once
#include"Vector.h"
#include<minwindef.h>
class Random
{
private:
	Random(){}
	~Random(){}

	

public:

	static void Initialize();
	
	/// <summary>
	/// �V�[�h�l���Z�b�g���܂��B
	/// </summary>
	/// <param name="seed"></param>
	static void SetSeed(UINT seed);

	/// <summary>
	/// �V�[�h�l�Ɏ��Ԃ��g�p����悤�ɂ��܂��B
	/// </summary>
	static void SetSeedTime();

	/// <summary>
	///	�������擾���܂��B�擾�ł���l��rand�֐��Ɠ����ł��B
	/// </summary>
	/// <param name="num"></param>
	/// <returns></returns>
	static int GetRandomNumber(const int num);
	
	/// <summary>
	/// �w�肵���͈͓��̐��̂ǂꂩ���擾���܂��B
	/// </summary>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <returns></returns>
	static int GetRandomNumberRangeSelect(const int start, const int end);

	/// <summary>
	/// �����̗������擾���܂��B
	/// </summary>
	/// <param name="num"></param>
	/// <param name="digits">����(1����4)</param>
	/// <returns></returns>
	static float GetRandomFloatNumber(const float num,const int digits);

	/// <summary>
	/// �w�肵���͈͓��̐��̂ǂꂩ���擾���܂��B
	/// </summary>
	/// <param name="num"></param>
	/// <param name="digits">����(1����4)</param>
	/// <returns></returns>
	static float GetRandomFloatNumberRangeSelect(const float start, const float end, const int digits);

};

