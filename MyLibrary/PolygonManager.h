#pragma once
#include<string>
#include<unordered_map>
#include"Library.h"
class PolygonManager final//これ親クラス作って処理まとめる?(サウンドとかも同じ処理書くだろうし)(シングルトンだから無理)
{
private:
	PolygonManager();
	~PolygonManager();

	//std::unordered_map<std::string, vertex> verts;
	/*std::unordered_map<std::string, heap> heaps;*/
	std::unordered_map<std::string, sprite> sprites;
	std::unordered_map<std::string, texture> textures;
	std::unordered_map<std::string, point>points;

public:
	PolygonManager(const PolygonManager& p) = delete;
	PolygonManager& operator=(const PolygonManager& p) = delete;

	static PolygonManager* GetInstance();


	//void addPolygonVertex(std::string keyName, int* vert);
	//void deletePolygonVertex(std::string keyName);
	//int* getPolygonVertex(std::string keyName);
	//void clearPolygonVertex();//clearだと指定できなくてバッファ削除できない


	/*void addPolygonHeap(std::string keyName, int* heap);
	void deletePolygonHeap(std::string keyName);
	int* getPolygonHeap(std::string keyName);
	void clearPolygonHeap();*/

	void AddSprite(std::string keyName, sprite sprite);
	void DeleteSprite(std::string keyName);
	sprite GetSprite(std::string keyName);
	void ClearSprite();

	void AddTexture(std::string keyName, texture texture);
	void DeleteTexture(std::string keyName);
	texture GetTexture(std::string keyName);
	void ClearTexture();


	void AddPolygonPoint(std::string keyName, point point);
	void DeletePolygonPoint(std::string keyName);
	point GetPolygonPoint(std::string keyName);
	void ClearPolygonPoint();

	void clear();
};

