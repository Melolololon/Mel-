#include "FrameTimer.h"

std::vector<FrameTimer*>FrameTimer::pTimers;

FrameTimer::FrameTimer()
{
	pTimers.push_back(this);
}

FrameTimer::FrameTimer(const  int maxTime)
{
	time = 0;
	this->maxTime = maxTime;
	pTimers.push_back(this);
}

FrameTimer::~FrameTimer()
{
	int count = 0;
	for(auto& p : pTimers)
	{
		if(p == this)
		{
			break;
		}
		count++;
	}
	pTimers.erase(pTimers.begin() + count);
}

void FrameTimer::AllUpdate()
{
	for (auto& p : pTimers)
	{
		p->Update();
	}
}

void FrameTimer::Update()
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


