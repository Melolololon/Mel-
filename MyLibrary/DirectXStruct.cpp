#include"DirectXStruct.h"
#include"DirectX12.h"
SmartModelData::SmartModelData()
{
}

SmartModelData::~SmartModelData()
{
	DirectX12::getInstance()->deletePolygonData(modelData);
	DirectX12::getInstance()->deleteHeapData(modelData);
}
