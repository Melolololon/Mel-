#include "SceneManager.h"



SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{

}

SceneManager* SceneManager::getInstace()
{
	static SceneManager inst;
	return &inst;
}

void SceneManager::Initialize(Scene* startScene)
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
		currentScene->end();

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
		currentScene->falseIsEnd();
		currentScene->Initialize();
	}

	currentScene->Update();

}

void SceneManager::Draw()
{
	currentScene->Draw();
}

void SceneManager::end()
{
	if (currentScene)
		delete currentScene;
}



Scene* SceneManager::GetCurrentScene()
{
	return currentScene;
}
