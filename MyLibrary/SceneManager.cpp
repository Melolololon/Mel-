#include "SceneManager.h"
#include"SceneEditer.h"
#include<assert.h>
#include<typeinfo>


using namespace MelLib;

SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{

}

void MelLib::SceneManager::Change()
{

	//シーン取得
	Scene* newScene = currentScene->GetNextScene();


	//終了処理
	currentScene->Finalize();

	// 違うシーンに切り替える場合、リソースを消す
	if (typeid(currentScene) != typeid(newScene))
	{
		//一括削除対象リソースを削除
		currentScene->ResourceBatchDeletion();
	}

	//シーン切り替え

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

SceneManager* SceneManager::GetInstance()
{
	static SceneManager inst;
	return &inst;
}

void SceneManager::SetStartScene(Scene* startScene)
{

	if (!startScene) assert(0);

	//SceneEditer::GetInstance().

	currentScene = startScene;
	currentScene->Initialize();
}

void SceneManager::Update()
{

	if (!currentScene || SceneEditer::GetInstance()->GetIsEdit())return;
	
	if (currentScene->GetIsEnd())Change();

	currentScene->Update();

}

void SceneManager::Draw()
{
	if (!currentScene)return;
	currentScene->Draw();
}

void SceneManager::Finalize()
{
	if (currentScene)
	{
		currentScene->Finalize();
		delete currentScene;
	}
	
}

Scene* SceneManager::GetCurrentScene()
{
	return currentScene;
}

std::string MelLib::SceneManager::GetCurrentSceneName()
{
	std::string name = typeid(*currentScene).name();

	// 先頭6文字("class ")を削除
	name.erase(name.begin(), name.begin() + 6);

	return name;
}
