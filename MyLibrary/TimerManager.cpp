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
		t->Update();
}


void TimerManager::AddFreamTimer(FreamTimer* timer)
{
	freamTimers.push_back(timer);
}


