#include "FreamTimer.h"

FreamTimer::FreamTimer()
{
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
	timeResetFream = false;
	if (isStop)return;


	if (isDecrement)
		time--;
	else
		time++;

	if (time == maxTime)
	{
		time = resetTime;
		timeResetFream = true;
	}
}


