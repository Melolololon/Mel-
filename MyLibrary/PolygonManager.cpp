#include "PolygonManager.h"



PolygonManager::PolygonManager()
{
}


PolygonManager::~PolygonManager()
{
}

PolygonManager* PolygonManager::getInstance()
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


void PolygonManager::addSprite(std::string keyName, sprite sprite) 
{
	sprites.emplace(keyName, sprite);
}

void PolygonManager::deleteSprite(std::string keyName)
{
}

sprite PolygonManager::getSprite(std::string keyName)
{
	return sprites[keyName];
}

void PolygonManager::clearSprite()
{
}




void PolygonManager::addTexture(std::string keyName, texture texture)
{
	textures.emplace(keyName, texture);
}
void PolygonManager::deleteTexture(std::string keyName){}
texture PolygonManager::getTexture(std::string keyName)
{

	return textures[keyName];
}
void PolygonManager::clearTexture(){}


void PolygonManager::addPolygonPoint(std::string keyName, point point)
{
	points.emplace(keyName, point);
}
void PolygonManager::deletePolygonPoint(std::string keyName)
{
}
point PolygonManager::getPolygonPoint(std::string keyName) 
{
	return points[keyName];
}
void PolygonManager::clearPolygonPoint()
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