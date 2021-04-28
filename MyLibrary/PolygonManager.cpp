#include "PolygonManager.h"



PolygonManager::PolygonManager()
{
}


PolygonManager::~PolygonManager()
{
}

PolygonManager* PolygonManager::GetInstance()
{
	static PolygonManager p;
	return &p;
}
//
//void PolygonManager::addPolygonVertex(std::string keyName, int* vert)
//{
//	verts.emplace(keyName, vert);
//}
//
//void PolygonManager::deletePolygonVertex(std::string keyName)
//{
//	Library::deleteVertexData(verts[keyName]);
//	verts.erase(keyName);
//}
//int* PolygonManager::getPolygonVertex(std::string keyName)
//{
//	return verts[keyName];
//}
//void PolygonManager::clearPolygonVertex()
//{
//	for (std::unordered_map<std::string, int*>::iterator it = verts.begin();
//		it != verts.end();
//		it++)
//	{
//		Library::deleteVertexData(it->second);
//	}
//}

//
//void PolygonManager::addPolygonHeap(std::string keyName, int* heap)
//{
//	heaps.emplace(keyName, heap);
//}
//void PolygonManager::deletePolygonHeap(std::string keyName)
//{
//	Library::deleteHeapData(heaps[keyName]);
//	heaps.erase(keyName);
//}
//int*  PolygonManager::getPolygonHeap(std::string keyName)
//{
//	return heaps[keyName];
//}
//void PolygonManager::clearPolygonHeap()
//{
//	for (std::unordered_map<std::string, int*>::iterator it = heaps.begin();
//		it != heaps.end();
//		it++)
//	{
//		Library::deleteHeapData(it->second);
//	}
//}


void PolygonManager::AddSprite(std::string keyName, sprite sprite) 
{
	sprites.emplace(keyName, sprite);
}

void PolygonManager::DeleteSprite(std::string keyName)
{
}

sprite PolygonManager::GetSprite(std::string keyName)
{
	return sprites[keyName];
}

void PolygonManager::ClearSprite()
{
}




void PolygonManager::AddTexture(std::string keyName, texture texture)
{
	textures.emplace(keyName, texture);
}
void PolygonManager::DeleteTexture(std::string keyName){}
texture PolygonManager::GetTexture(std::string keyName)
{

	return textures[keyName];
}
void PolygonManager::ClearTexture(){}


void PolygonManager::AddPolygonPoint(std::string keyName, point point)
{
	points.emplace(keyName, point);
}
void PolygonManager::DeletePolygonPoint(std::string keyName)
{
}
point PolygonManager::GetPolygonPoint(std::string keyName) 
{
	return points[keyName];
}
void PolygonManager::ClearPolygonPoint()
{
}


void PolygonManager::clear()
{/*
	for (std::unordered_map<std::string, int*>::iterator it = verts.begin();
		it != verts.end();
		it++)
	{
		Library::deleteVertexData(it->second);
	}*/

	//for (std::unordered_map<std::string, int*>::iterator it = heaps.begin();
	//	it != heaps.end();
	//	it++)
	//{
	//	Library::deleteHeapData(it->second);
	//}

	/*for (std::unordered_map<std::string, int*>::iterator it = points.begin();
		it != points.end();
		it++)
	{
		Library::deleteHeapData(it->second);
	}*/
}