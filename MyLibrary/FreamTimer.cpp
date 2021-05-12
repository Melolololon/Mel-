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
	time++;
	if (time == maxTime)
		time = 0;
}


bool FreamTimer::GetSameAsMaximumFlag()
{
	return time == maxTime;
}

bool FreamTimer::GetMultipleTimeFlag(const unsigned int num)
{
	return time % num == 0;
}
