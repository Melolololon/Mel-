#include "SceneManager.h"
#include"SceneEditer.h"
#include<assert.h>
#include<typeinfo>
#include"GameObjectManager.h"

using namespace MelLib;

SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{

}

void MelLib::SceneManager::Change()
{
	// 強制削除
	// エディターでのシーンの読み込み直し時に現在のシーンに生成などの処理がないと消えてしまうため、
	// 一旦強制的に消して毎回読み込ませるようにする
	GameObjectManager::GetInstance()->AllEraseObject();

	//シーン取得
	Scene* newScene = currentScene->GetNextScene();


	//終了処理
	currentScene->Finalize();

	// エディターでテストプレイしていたら繰り返す
	// そうでなかったら新規シーンに切り替え
	if (!SceneEditer::GetInstance()->GetEditerFlag())
	{
		// 違うシーンに切り替える場合、リソースを消す
		if (typeid(currentScene) != typeid(newScene))
		{
			//一括削除対象リソースを削除
			currentScene->ResourceBatchDeletion();
		}

		//シーン切り替え
		//シーンを削除
		delete currentScene;
		//入れ替え
		currentScene = newScene;

	}

	// 初期化
	GameObject::ResetObjectNumbers();
	currentScene->FalseIsEnd();
	SceneEditer::GetInstance()->LoadTestPlaySaveData();
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

	if (!currentScene
		|| SceneEditer::GetInstance()->GetIsEdit()
		&& SceneEditer::GetInstance()->GetReleaeEdit()
		&& SceneEditer::GetInstance()->GetEditerFlag())return;

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

void MelLib::SceneManager::ReLoadScene()
{
	GameObjectManager::GetInstance()->AllEraseObject();
	if (!currentScene)return;
	currentScene->Initialize();
	GameObject::ResetObjectNumbers();
}
