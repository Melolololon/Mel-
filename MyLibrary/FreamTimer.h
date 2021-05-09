#pragma once
#include"TimerManager.h"
class TimerManager;

class FreamTimer
{
private:
	unsigned int time;
	unsigned int maxTime;


	void Update() { time++; }
public:
	friend TimerManager;

	FreamTimer();
	/// <summary>
	/// 
	/// </summary>
	/// <param name="maxTime">最大計測時間(フレーム)</param>
	FreamTimer(const unsigned int maxTime);
	~FreamTimer();

	/// <summary>
	/// 現在の時間をフレームで返します。
	/// </summary>
	/// <returns></returns>
	unsigned int GetTime() { return time; }

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

};

