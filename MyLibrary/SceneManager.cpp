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

void SceneManager::initialize()
{
	currentScene = "";
}

void SceneManager::addScene(std::string key, Scene* scene)
{
	scenes.emplace(key, scene);

	if (currentScene == "")
	{
		scene->falseIsEnd();
		scene->initialize();
		//最初に追加されたシーンのキーを代入
		currentScene = key;
	}

}

void SceneManager::update()
{

	if (scenes[currentScene]->getIsEnd())
	{
		//終了処理
		scenes[currentScene]->end();

		//シーン切り替え
		currentScene = scenes[currentScene]->getNextScene();

		//初期化
		scenes[currentScene]->falseIsEnd();
		scenes[currentScene]->initialize();
	}

	scenes[currentScene]->update();
}

void SceneManager::draw()
{
	scenes[currentScene]->draw();
}

void SceneManager::end()
{
	for (std::unordered_map<std::string, Scene*>::iterator it = scenes.begin();
		it != scenes.end();
		it++)
	{
		delete it->second;
	}
	scenes.clear();
}

void* SceneManager::getScenePtr(std::string key)
{
	return scenes[key]->getPtr();
}
