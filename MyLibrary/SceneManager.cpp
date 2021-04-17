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
		//�I������
		currentScene->end();

		//�V�[���؂�ւ�
		//�V�[����ۑ�
		Scene* previousScene = currentScene;
		//�؂�ւ�
		currentScene = currentScene->getNextScene();
		//�����|�C���^�Z�b�g�h�~
		if (currentScene == previousScene)assert(0);
		//�O�̃V�[�����폜
		delete previousScene;

		//������
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
