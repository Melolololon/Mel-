#include "FreamTimer.h"

FreamTimer::FreamTimer()
{
	time = 0;
	maxTime = 0;

	TimerManager::GetInstance()->AddFreamTimer(this);
}

FreamTimer::FreamTimer(const unsigned int maxTime)
{
	time = 0;
	this->maxTime = maxTime;

	TimerManager::GetInstance()->AddFreamTimer(this);
}

FreamTimer::~FreamTimer()
{
}

void FreamTimer::Update()
{
	timeResetFream = false;
	time++;
	if (time == maxTime) 
	{
		time = 0;
		timeResetFream = true;
	}
}


bool FreamTimer::GetSameAsMaximumFlag()
{
	return timeResetFream;
}

bool FreamTimer::GetMultipleTimeFlag(const unsigned int num)
{
	return time % num == 0;
}
