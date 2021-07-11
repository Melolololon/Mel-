#include "SceneManager.h"
#include<assert.h>

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

		//�V�[���擾
		Scene* newScene = currentScene->GetNextScene();
		//�����|�C���^�Z�b�g�h�~
		if (newScene == currentScene)assert(0);
		//�V�[�����폜
		delete currentScene;
	    //����ւ�
		currentScene = newScene;

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
