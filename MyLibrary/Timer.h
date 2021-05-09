#pragma once
#include"TimerManager.h"
class TimerManager;

class Timer
{
private:
	unsigned int currentTime;
	unsigned int maxTime;

	
	void Update() { currentTime++; }
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
	unsigned int GetCurrentTime() { return currentTime; }

	/// <summary>
	/// 最大計測時間と現在の時間が同じかを返します。
	/// </summary>
	bool GetSameAsMaximumFlag();

};

