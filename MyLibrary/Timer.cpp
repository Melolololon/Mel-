#include "Timer.h"

Timer::Timer(unsigned int maxTime)
{
	currentTime = 0;
	maxTime = 0;

	TimerManager::GetInstance()->AddTimer(this);
}

Timer::~Timer()
{
}


bool Timer::GetSameAsMaximumFlag()
{
	if (currentTime == maxTime)
		return true;
	return false;
}
