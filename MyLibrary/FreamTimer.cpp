#include "FreamTimer.h"

FreamTimer::FreamTimer()
{
	time = 0;
	maxTime = 0;

	TimerManager::GetInstance()->AddFreamTimer(this);
}

FreamTimer::FreamTimer(const  int maxTime)
{
	time = 0;
	this->maxTime = maxTime;

	TimerManager::GetInstance()->AddFreamTimer(this);
}

FreamTimer::~FreamTimer()
{
	TimerManager::GetInstance()->EraseFreamTimer(this);
}

void FreamTimer::Update()
{
	if (isStop)return;

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

bool FreamTimer::GetMultipleTimeFlag(const  int num)
{
	return time % num == 0;
}
