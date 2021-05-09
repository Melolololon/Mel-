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

float Random::GetRandomNumberFloat(int number)
{
	return static_cast<float>((rand() % number));
}

int Random::GetRandomNumberRangeSelect(const int& start, const int& end)
{
	return GetRandomNumber(abs(end - start) + 1) + start;
}

float Random::GetRandomNumberRangeSelectFloat(const int& start, const int& end)
{
	return static_cast<float>(GetRandomNumber(abs(end - start) + 1) + start);
}
