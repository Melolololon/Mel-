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
		//�ŏ��ɒǉ����ꂽ�V�[���̃L�[����
		currentScene = key;
	}

}

void SceneManager::update()
{

	if (scenes[currentScene]->getIsEnd())
	{
		//�I������
		scenes[currentScene]->end();

		//�V�[���؂�ւ�
		currentScene = scenes[currentScene]->getNextScene();

		//������
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
