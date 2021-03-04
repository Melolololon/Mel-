#pragma once
#include"DirectInput.h"
#include<string>
#pragma region Object

#pragma endregion
class Scene
{

protected:
	bool isEnd;

public:
	Scene();
	virtual ~Scene();
	void falseIsEnd();
	virtual void initialize() = 0;//初期化
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void end() = 0;//終了処理
	virtual std::string getNextScene() = 0;//次のシーンの指定
	bool getIsEnd();//シーンが終わったかどうか

	void* getPtr();
};

