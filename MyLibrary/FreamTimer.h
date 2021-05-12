#pragma once
#include"TimerManager.h"
class TimerManager;

class FreamTimer
{
private:
	unsigned int time = 0;
	unsigned int maxTime = 0;
	bool timeResetFream = false;

	void Update();
public:
	friend TimerManager;

	FreamTimer();
	/// <summary>
	/// 
	/// </summary>
	/// <param name="maxTime">最大計測時間</param>
	FreamTimer(const unsigned int maxTime);
	~FreamTimer();



	/// <summary>
	/// 現在の時間を返します。
	/// </summary>
	/// <returns></returns>
	unsigned int GetTime() { return time; }

	/// <summary>
	/// 最大計測時間を返します。
	/// </summary>
	/// <returns></returns>
	unsigned int GetMaxTime() { return maxTime; }
	
	/// <summary>
	/// 最大計測時間と現在の時間が同じ場合、trueを返します。
	/// </summary>
	bool GetSameAsMaximumFlag();

	/// <summary>
	/// 時間がセットした数値の倍数だった場合、trueを返します。
	/// </summary>
	/// <param name="num"></param>
	/// <returns></returns>
	bool GetMultipleTimeFlag(const unsigned int num);


	void SetMaxTime(const unsigned int num) { maxTime = num; }
};

