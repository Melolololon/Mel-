#pragma once
#include"TimerManager.h"
class TimerManager;

#include<vector>

//演算子オーバーロードする?
//マネージャーでのUpdateとストップフラグ関数やめて、
//関数呼び出してる間ストップしない(Updateする)って仕様にする?

//フレームタイマー(プログラム依存)
class FreamTimer
{
private:
	//タイマーのポインタの配列(Update用)
	std::vector<FreamTimer*>pTimers;

	//現在の時間
	int time = 0;
	//リセットしたときに現在の時間を上書きする時間
	int resetTime = 0;
	//最大カウント時間
	int maxTime = INT_MAX;
	//カウントするかどうか
	bool isStop = true;
	//タイムがリセットされるフレームかどうか
	bool timeResetFream = false;
	//時間を加算するか減算するか
	bool isDecrement = false;

	void Update();
public:
	friend TimerManager;

	FreamTimer();
	/// <summary>
	/// 
	/// </summary>
	/// <param name="maxTime">最大計測時間</param>
	FreamTimer(const int maxTime);
	~FreamTimer();


	/// <summary>
	/// 時間をリセットタイムでリセットします。
	/// </summary>
	void ResetTime() { time = resetTime; }

	/// <summary>
	/// 時間を0でリセットします。
	/// </summary>
	void ResetTimeZero() { time = 0; }

#pragma region ゲット


	/// <summary>
	/// タイマーが停止しているかを返します。
	/// </summary>
	/// <returns></returns>
	bool GetStopFlag() { return isStop; }

	/// <summary>
	/// 最大計測時間に達したときにリセットするための時間を返します。
	/// </summary>
	/// <returns></returns>
	int GetTime() { return time; }

	/// <summary>
	/// 最小計測時間を取得します。
	/// </summary>
	/// <returns></returns>
	int GetResetTime() { return resetTime; }

	/// <summary>
	/// 最大計測時間を返します。
	/// </summary>
	/// <returns></returns>
	int GetMaxTime() { return maxTime; }

	/// <summary>
	/// 最大計測時間と現在の時間が同じになった瞬間にtrueを返します。
	/// </summary>
	/// <returns></returns>
	bool GetSameAsMaximumFlag() { return timeResetFream; }

	/// <summary>
	/// 時間がセットした数値の倍数だった場合、trueを返します。
	/// </summary>
	/// <param name="num">数値</param>
	/// <returns></returns>
	bool GetMultipleTimeFlag(const int num){ return time % num == 0; }

	/// <summary>
	/// セットした時間ごとにフラグをtrueとfalseに切り替えて返します。falseからスタートします。
	/// </summary>
	/// <param name="num">数値</param>
	/// <returns></returns>
	int GetSetByNumberFlag(const int num) { return time % (num * 2) >= num; }
#pragma endregion

#pragma region セット

	/// <summary>
	/// タイマーを停止するかどうかのフラグをセットします、
	/// </summary>
	/// <param name="flag"></param>
	void SetStopFlag(const bool flag) { isStop = flag; }

	/// <summary>
	/// 時間を加算していくか減算していくかを指定します。
	/// </summary>
	/// <param name="flag"></param>
	void SetDecrementFlag(const bool flag) { isDecrement = flag; }

	/// <summary>
	/// 最大計測時間に達したときに現在の時間をリセットするための時間を設定します。
	/// </summary>
	/// <param name="num"></param>
	void SetResetTime(const int num) { resetTime = num; }

	/// <summary>
	/// 最大計測時間を設定します。
	/// </summary>
	/// <param name="num"></param>
	void SetMaxTime(const int num) { maxTime = num; }

	/// <summary>
	/// タイマーの現在の時間を設定します。
	/// </summary>
	/// <param name="num"></param>
	void SetNowTime(const int num) { time = num; }
#pragma endregion

};

