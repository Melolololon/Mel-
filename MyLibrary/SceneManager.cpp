#include "SceneManager.h"



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
		//シーンを保存
		Scene* previousScene = currentScene;
		//切り替え
		currentScene = currentScene->GetNextScene();
		//同じポインタセット防止
		if (currentScene == previousScene)assert(0);
		//前のシーンを削除
		delete previousScene;

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
