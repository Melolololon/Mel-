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
		//�I������
		currentScene->end();

		//�V�[���؂�ւ�
		//�V�[����ۑ�
		Scene* previousScene = currentScene;
		//�؂�ւ�
		currentScene = currentScene->GetNextScene();
		//�����|�C���^�Z�b�g�h�~
		if (currentScene == previousScene)assert(0);
		//�O�̃V�[�����폜
		delete previousScene;

		//������
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
