#include "HeapIndexManager.h"
#include<Windows.h>

HeapIndexManager::HeapIndexManager(const int maxModelNum)
{
	createNum = maxModelNum;
	drawCheckVector.resize(maxModelNum,false);
}

HeapIndexManager::~HeapIndexManager()
{}


int HeapIndexManager::GetHeapIndex()
{
	for (int i = 0; i < createNum; i++)
	{
		if(!drawCheckVector[i])
		{
			drawCheckVector[i] = true;
			return i;
		}
	}

	OutputDebugString(L"画面に表示できる数を超えているモデルがあります。確認してください。\0");
	return -1;
}



