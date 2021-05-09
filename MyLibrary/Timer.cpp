#include "Timer.h"

Timer::Timer(unsigned int maxTime)
{
	time = 0;
	maxTime = 0;

	TimerManager::GetInstance()->AddTimer(this);
}

Timer::~Timer()
{
}


bool Timer::GetSameAsMaximumFlag()
{
	return time == maxTime;
}

bool Timer::GetMultipleTime(const unsigned int num)
{
	return time % num == 0;
}
