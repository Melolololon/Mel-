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

void SceneManager::initialize(Scene* startScene)
{
	if (!startScene)
		assert(0);

	currentScene = startScene;
	currentScene->initialize();
}

void SceneManager::update()
{

	if (currentScene->getIsEnd())
	{
		//終了処理
		currentScene->end();

		//シーン切り替え
		//シーンを保存
		Scene* previousScene = currentScene;
		//切り替え
		currentScene = currentScene->getNextScene();
		//同じポインタセット防止
		if (currentScene == previousScene)assert(0);
		//前のシーンを削除
		delete previousScene;

		//初期化
		currentScene->falseIsEnd();
		currentScene->initialize();
	}

	currentScene->update();

}

void SceneManager::draw()
{
	currentScene->draw();
}

void SceneManager::end()
{
	if (currentScene)
		delete currentScene;
}



Scene* SceneManager::getCurrentScene()
{
	return currentScene;
}
