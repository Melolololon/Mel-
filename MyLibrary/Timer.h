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
	/// <param name="maxTime">�ő�v������(�t���[��)</param>
	Timer(unsigned int maxTime);
	~Timer();

	/// <summary>
	/// ���݂̎��Ԃ��t���[���ŕԂ��܂��B
	/// </summary>
	/// <returns></returns>
	unsigned int GetCurrentTime() { return currentTime; }

	/// <summary>
	/// �ő�v�����Ԃƌ��݂̎��Ԃ���������Ԃ��܂��B
	/// </summary>
	bool GetSameAsMaximumFlag();

};

