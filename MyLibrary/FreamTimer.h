#pragma once
#include"TimerManager.h"
class TimerManager;

#include<vector>

//���Z�q�I�[�o�[���[�h����?
//�}�l�[�W���[�ł�Update�ƃX�g�b�v�t���O�֐���߂āA
//�֐��Ăяo���Ă�ԃX�g�b�v���Ȃ�(Update����)���Ďd�l�ɂ���?

//�t���[���^�C�}�[(�v���O�����ˑ�)
class FreamTimer
{
private:
	//�^�C�}�[�̃|�C���^�̔z��(Update�p)
	std::vector<FreamTimer*>pTimers;

	//���݂̎���
	int time = 0;
	//���Z�b�g�����Ƃ��Ɍ��݂̎��Ԃ��㏑�����鎞��
	int resetTime = 0;
	//�ő�J�E���g����
	int maxTime = INT_MAX;
	//�J�E���g���邩�ǂ���
	bool isStop = true;
	//�^�C�������Z�b�g�����t���[�����ǂ���
	bool timeResetFream = false;
	//���Ԃ����Z���邩���Z���邩
	bool isDecrement = false;

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
	/// ���Ԃ����Z�b�g�^�C���Ń��Z�b�g���܂��B
	/// </summary>
	void ResetTime() { time = resetTime; }

	/// <summary>
	/// ���Ԃ�0�Ń��Z�b�g���܂��B
	/// </summary>
	void ResetTimeZero() { time = 0; }

#pragma region �Q�b�g


	/// <summary>
	/// �^�C�}�[����~���Ă��邩��Ԃ��܂��B
	/// </summary>
	/// <returns></returns>
	bool GetStopFlag() { return isStop; }

	/// <summary>
	/// �ő�v�����ԂɒB�����Ƃ��Ƀ��Z�b�g���邽�߂̎��Ԃ�Ԃ��܂��B
	/// </summary>
	/// <returns></returns>
	int GetTime() { return time; }

	/// <summary>
	/// �ŏ��v�����Ԃ��擾���܂��B
	/// </summary>
	/// <returns></returns>
	int GetResetTime() { return resetTime; }

	/// <summary>
	/// �ő�v�����Ԃ�Ԃ��܂��B
	/// </summary>
	/// <returns></returns>
	int GetMaxTime() { return maxTime; }

	/// <summary>
	/// �ő�v�����Ԃƌ��݂̎��Ԃ������ɂȂ����u�Ԃ�true��Ԃ��܂��B
	/// </summary>
	/// <returns></returns>
	bool GetSameAsMaximumFlag() { return timeResetFream; }

	/// <summary>
	/// ���Ԃ��Z�b�g�������l�̔{���������ꍇ�Atrue��Ԃ��܂��B
	/// </summary>
	/// <param name="num">���l</param>
	/// <returns></returns>
	bool GetMultipleTimeFlag(const int num){ return time % num == 0; }

	/// <summary>
	/// �Z�b�g�������Ԃ��ƂɃt���O��true��false�ɐ؂�ւ��ĕԂ��܂��Bfalse����X�^�[�g���܂��B
	/// </summary>
	/// <param name="num">���l</param>
	/// <returns></returns>
	int GetSetByNumberFlag(const int num) { return time % (num * 2) >= num; }
#pragma endregion

#pragma region �Z�b�g

	/// <summary>
	/// �^�C�}�[���~���邩�ǂ����̃t���O���Z�b�g���܂��A
	/// </summary>
	/// <param name="flag"></param>
	void SetStopFlag(const bool flag) { isStop = flag; }

	/// <summary>
	/// ���Ԃ����Z���Ă��������Z���Ă��������w�肵�܂��B
	/// </summary>
	/// <param name="flag"></param>
	void SetDecrementFlag(const bool flag) { isDecrement = flag; }

	/// <summary>
	/// �ő�v�����ԂɒB�����Ƃ��Ɍ��݂̎��Ԃ����Z�b�g���邽�߂̎��Ԃ�ݒ肵�܂��B
	/// </summary>
	/// <param name="num"></param>
	void SetResetTime(const int num) { resetTime = num; }

	/// <summary>
	/// �ő�v�����Ԃ�ݒ肵�܂��B
	/// </summary>
	/// <param name="num"></param>
	void SetMaxTime(const int num) { maxTime = num; }

	/// <summary>
	/// �^�C�}�[�̌��݂̎��Ԃ�ݒ肵�܂��B
	/// </summary>
	/// <param name="num"></param>
	void SetNowTime(const int num) { time = num; }
#pragma endregion

};

