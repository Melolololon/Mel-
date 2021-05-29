#include "TimerManager.h"

TimerManager::TimerManager()
{
	freamTimers.reserve(100);
}
TimerManager::~TimerManager(){}

TimerManager* TimerManager::GetInstance()
{
	static TimerManager t;
	return &t;
}

void TimerManager::Update()
{
	for (auto& t : freamTimers) 
	{
		t->Update();
	}
}


void TimerManager::AddFreamTimer(FreamTimer* timer)
{
	freamTimers.push_back(timer);
}

void TimerManager::EraseFreamTimer(FreamTimer* timer)
{
	auto freamTimerSize = freamTimers.size();
	for (int i = 0; i < freamTimerSize;i++)
	{
		if (freamTimers[i] == timer)
			freamTimers.erase(freamTimers.begin() + i);
	}

}

