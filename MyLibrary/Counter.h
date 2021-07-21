#pragma once
#include<limits>

//�|�C���^��z��ɂ܂Ƃ߂��Ȃ�(�u�Ԃ̔�����Ƃ�Ȃ�)����A�N���X�e���v���[�g��߂�

//�J�E���^�[�N���X
class Counter
{
private:
	int count = 0;
	int preCount = -1;
	int resetNum = 0;
	int maxCount = INT_MAX;
	int minCount = INT_MIN;


public:
	Counter() {}
	Counter(const int num) :count(num), preCount(count - 1) {}


	void operator++() { count++; }
	void operator--() { count--; }

	void IncrementCount() { count++; }
	void DecrementCount() { count--; }
	void AddCount(const int num) { count += num; }
	void SubCount(const int num) { count -= num; }
	void ResetCount() { count = resetNum; }
	void ResetCountZero() { count = 0; }

#pragma region �Z�b�g
	void SetCount(const int num) { count = num; }
	void SetResetNum(const int num) { resetNum = num; }
#pragma endregion


#pragma region �Q�b�g


	int GetCount() { return count; }

#pragma endregion

};

//�J�E���^�[�N���X(float�^)
class CounterF
{
private:
	float count = 0;
	float preCount = -1;
	float resetNum = 0;
	float maxCount = FLT_MAX;
	float minCount = FLT_MIN;


public:
	CounterF() {}
	CounterF(const float num) :count(num), preCount(count - 1) {}


	void operator++() { count++; }
	void operator--() { count--; }

	void IncrementCount() { count++; }
	void DecrementCount() { count--; }
	void AddCount(const float num) { count += num; }
	void SubCount(const float num) { count -= num; }
	void ResetCount() { count = resetNum; }
	void ResetCountZero() { count = 0; }

#pragma region �Z�b�g
	void SetCount(const float num) { count = num; }
	void SetResetNum(const float num) { resetNum = num; }
#pragma endregion


#pragma region �Q�b�g


	float GetCount() { return count; }

#pragma endregion

};
