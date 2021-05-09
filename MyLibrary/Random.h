#pragma once
class Random
{
private:
	Random();
	~Random();
public:
	static void Initialize();
	static int GetRandomNumber(const int num);
	static float GetRandomNumberFloat(const int num);
	static int GetRandomNumberRangeSelect(const int& start, const int& end);
	static float GetRandomNumberRangeSelectFloat(const int& start, const int& end);
};

