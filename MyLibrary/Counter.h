#pragma once
#include<limits>

//ポインタを配列にまとめられない(瞬間の判定をとれない)から、クラステンプレートやめた

//カウンタークラス
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

#pragma region セット
	void SetCount(const int num) { count = num; }
	void SetResetNum(const int num) { resetNum = num; }
#pragma endregion


#pragma region ゲット


	int GetCount() { return count; }

#pragma endregion

};

//カウンタークラス(float型)
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

#pragma region セット
	void SetCount(const float num) { count = num; }
	void SetResetNum(const float num) { resetNum = num; }
#pragma endregion


#pragma region ゲット


	float GetCount() { return count; }

#pragma endregion

};
