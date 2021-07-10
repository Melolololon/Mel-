#include "SceneManager.h"
#include<assert.h>

SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{

}

SceneManager* SceneManager::GetInstace()
{
	static SceneManager inst;
	return &inst;
}

void SceneManager::SetStartScene(Scene* startScene)
{

	if (!startScene)
		assert(0);

	currentScene = startScene;
	currentScene->Initialize();
}

void SceneManager::Update()
{


	
	if (currentScene->GetIsEnd())
	{
		//終了処理
		currentScene->Finitialize();

		//シーン切り替え

		//シーン取得
		Scene* newScene = currentScene->GetNextScene();
		//同じポインタセット防止
		if (newScene == currentScene)assert(0);
		//シーンを削除
		delete currentScene;
	    //入れ替え
		currentScene = newScene;

		//初期化
		currentScene->FalseIsEnd();
		currentScene->Initialize();
	}

	currentScene->Update();

}

void SceneManager::Draw()
{
	currentScene->Draw();
}

void SceneManager::Finitialize()
{
	if (currentScene)
		delete currentScene;
	
}



Scene* SceneManager::GetCurrentScene()
{
	return currentScene;
}
