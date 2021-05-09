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
	/// <param name="maxTime">�ő�v������(�t���[��)</param>
	Timer(unsigned int maxTime);
	~Timer();

	/// <summary>
	/// ���݂̎��Ԃ��t���[���ŕԂ��܂��B
	/// </summary>
	/// <returns></returns>
	unsigned int GetTime() { return time; }

	/// <summary>
	/// �ő�v�����Ԃƌ��݂̎��Ԃ���������Ԃ��܂��B
	/// </summary>
	bool GetSameAsMaximumFlag();

	/// <summary>
	/// ���Ԃ��Z�b�g�������l�̔{���������ꍇ�Atrue��Ԃ��܂��B
	/// </summary>
	/// <param name="num"></param>
	/// <returns></returns>
	bool GetMultipleTime(const unsigned int num);

};

