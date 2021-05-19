#include "Random.h"
#include<random>
#include<ctime>
void Random::Initialize()
{
	srand(static_cast<unsigned int>(time(NULL)));
}
int Random::GetRandomNumber(int number)
{
	return rand() % number;
}

int Random::CalcRandomMag(const unsigned int fineness)
{
	int mag = 1;
	for (int i = 0; i < fineness; i++)
	{
		mag *= 10;
		if (i >= 4)
			break;
	}
	return;
}

float Random::GetRandomNumberFloat(int number, const unsigned int fineness)
{
	int mag = CalcRandomMag(fineness);
	return static_cast<float>( (rand() % number * mag) / mag);
}

int Random::GetRandomNumberRangeSelect(const int start, const int end)
{
	return GetRandomNumber(abs(end - start) + 1) + start;
}

float Random::GetRandomNumberRangeSelectFloat(const int start, const int end, const unsigned int fineness)
{

	int mag = CalcRandomMag(fineness);

	return static_cast<float>(GetRandomNumber(abs(end - start) + 1) + start);
}


//
//Vector2 Random::GetRandomNumberVector2(const Vector2& vec2Num)
//{
//	return Vector2
//	(
//		GetRandomNumberFloat(vec2Num.x),
//		GetRandomNumberFloat(vec2Num.y)
//	);
//}
//
//
//static Vector2 GetRandomNumberRangeSelectVector2(const Vector2& vec2Num, const Vector2& vec2NumEnd)
//{
//
//}
