#pragma once
#include"TimerManager.h"
class TimerManager;

class FreamTimer
{
private:
	int time = 0;
	int maxTime = 0;
	bool isStop = true;
	bool timeResetFream = false;


	void Update();
public:
	friend TimerManager;

	FreamTimer();
	/// <summary>
	/// 
	/// </summary>
	/// <param name="maxTime">�ő�v������</param>
	FreamTimer(const int maxTime);
	~FreamTimer();


	/// <summary>
	/// ���Ԃ����Z�b�g���܂��B
	/// </summary>
	void ResetTime() { time = 0; }

	/// <summary>
	/// �^�C�}�[����~���Ă��邩��Ԃ��܂��B
	/// </summary>
	/// <returns></returns>
	bool GetStopFlag() { return isStop; }

	/// <summary>
	/// ���݂̎��Ԃ�Ԃ��܂��B
	/// </summary>
	/// <returns></returns>
	int GetTime() { return time; }

	/// <summary>
	/// �ő�v�����Ԃ�Ԃ��܂��B
	/// </summary>
	/// <returns></returns>
	int GetMaxTime() { return maxTime; }

	/// <summary>
	/// �ő�v�����Ԃƌ��݂̎��Ԃ������ꍇ�Atrue��Ԃ��܂��B
	/// </summary>
	bool GetSameAsMaximumFlag();

	/// <summary>
	/// ���Ԃ��Z�b�g�������l�̔{���������ꍇ�Atrue��Ԃ��܂��B
	/// </summary>
	/// <param name="num"></param>
	/// <returns></returns>
	bool GetMultipleTimeFlag(const int num);

	/// <summary>
	/// �^�C�}�[���~���邩�ǂ����̃t���O���Z�b�g���܂��A
	/// </summary>
	/// <param name="flag"></param>
	void SetStopFlag(const bool flag) { isStop = flag; }

	/// <summary>
	/// �ő�v�����Ԃ�ݒ肵�܂��B
	/// </summary>
	/// <param name="num"></param>
	void SetMaxTime(const int num) { maxTime = num; }

	/// <summary>
	/// �^�C�}�[�̎��Ԃ�ݒ肵�܂��B
	/// </summary>
	/// <param name="num"></param>
	void SetNowTime(const int num) { time = num; }
};

