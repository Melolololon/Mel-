#include "TimerManager.h"
TimerManager::TimerManager()
{
	timers.reserve(100);
}
TimerManager::~TimerManager(){}

TimerManager* TimerManager::GetInstance()
{
	static TimerManager t;
	return &t;
}

void TimerManager::Update()
{
	for (auto& t : timers)
		t->Update();
}


void TimerManager::AddTimer(Timer* timer)
{
	timers.push_back(timer);
}


