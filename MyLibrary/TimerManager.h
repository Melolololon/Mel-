#pragma once
#include<vector>
#include"FreamTimer.h"
class FreamTimer;

class TimerManager
{
private:
	TimerManager();
	~TimerManager();

	std::vector<FreamTimer*>freamTimers;
public:
	TimerManager(TimerManager& t) = delete;
	TimerManager& operator=(TimerManager& t) = delete;
	static TimerManager* GetInstance();

	void AddFreamTimer(FreamTimer* timer);
	void EraseFreamTimer(FreamTimer* timer);
	void Update();
};

