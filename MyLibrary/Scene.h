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
	virtual void Initialize() = 0;//初期化
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void end() = 0;//終了処理
	virtual Scene* GetNextScene() = 0;//次のシーンの指定
	bool GetIsEnd();//シーンが終わったかどうか

	void* GetPtr();
};

