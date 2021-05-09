#pragma once
#include"TimerManager.h"
class TimerManager;

class Timer
{
private:
	unsigned int time;
	unsigned int maxTime;

	
	void Update() { time++; }
public:
	friend TimerManager;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="maxTime">最大計測時間(フレーム)</param>
	Timer(unsigned int maxTime);
	~Timer();

	/// <summary>
	/// 現在の時間をフレームで返します。
	/// </summary>
	/// <returns></returns>
	unsigned int GetTime() { return time; }

	/// <summary>
	/// 最大計測時間と現在の時間が同じかを返します。
	/// </summary>
	bool GetSameAsMaximumFlag();

	/// <summary>
	/// 時間がセットした数値の倍数だった場合、trueを返します。
	/// </summary>
	/// <param name="num"></param>
	/// <returns></returns>
	bool GetMultipleTime(const unsigned int num);

};

