#include "HeapIndexManager.h"
#include<Windows.h>

HeapIndexManager::HeapIndexManager(const int maxModelNum)
{
	if(maxModelNum <= 0)
		createNum = 1;
	else
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

	OutputDebugString(L"��ʂɕ\���ł��鐔�𒴂��Ă��郂�f��������܂��B�m�F���Ă��������B\0");
	return -1;
}

void HeapIndexManager::DrawEndCallFunction(const int heapIndex)
{
	if(heapIndex >= 0 && heapIndex < createNum)
	drawCheckVector[heapIndex] = false; 
}

