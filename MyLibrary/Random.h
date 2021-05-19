#pragma once
#include"Vector.h"
class Random
{
private:
	Random(){}
	~Random(){}

	static int CalcRandomMag(const unsigned int fineness);
public:
	static void Initialize();
	static int GetRandomNumber(const int num);
	static float GetRandomNumberFloat(const int num, const unsigned int fineness);

	static int GetRandomNumberRangeSelect(const int start, const int end);
	static float GetRandomNumberRangeSelectFloat(const int start, const int end, const unsigned int fineness);
	

	/*static Vector2 GetRandomNumberVector2(const Vector2& vec2Num);
	static Vector2 GetRandomNumberRangeSelectVector2(const Vector2& vec2NumStart, const Vector2& vec2NumEnd);*/
};

