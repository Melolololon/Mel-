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
		//�I������
		currentScene->Finitialize();

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
