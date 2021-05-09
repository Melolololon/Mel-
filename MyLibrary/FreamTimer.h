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
	/// <param name="maxTime">�ő�v������(�t���[��)</param>
	FreamTimer(const unsigned int maxTime);
	~FreamTimer();

	/// <summary>
	/// ���݂̎��Ԃ��t���[���ŕԂ��܂��B
	/// </summary>
	/// <returns></returns>
	unsigned int GetTime() { return time; }

	/// <summary>
	/// �ő�v�����Ԃƌ��݂̎��Ԃ������ꍇ�Atrue��Ԃ��܂��B
	/// </summary>
	bool GetSameAsMaximumFlag();

	/// <summary>
	/// ���Ԃ��Z�b�g�������l�̔{���������ꍇ�Atrue��Ԃ��܂��B
	/// </summary>
	/// <param name="num"></param>
	/// <returns></returns>
	bool GetMultipleTimeFlag(const unsigned int num);

};

