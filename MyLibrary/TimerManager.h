#pragma once
#include<vector>
#include"Timer.h"
class Timer;

class TimerManager
{
private:
	TimerManager();
	~TimerManager();

	std::vector<Timer*>timers;
public:
	TimerManager(TimerManager& t) = delete;
	TimerManager& operator=(TimerManager& t) = delete;
	static TimerManager* GetInstance();

	void AddTimer(Timer* timer);
	void Update();
};

