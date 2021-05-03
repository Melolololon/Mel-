#pragma once
#include<vector>
#include<unordered_map>
#include"Scene.h"
class SceneManager final
{

private:

	SceneManager();
	~SceneManager();

	Scene* currentScene = nullptr;
public:

	SceneManager(const SceneManager& sceneManager) = delete;//�R�s�[�R���X�g���N�^
	SceneManager& operator=(const SceneManager& sceneManager) = delete;//�R�s�[������Z�q
	static SceneManager* GetInstace();

	void Initialize(Scene* startScene);
	void Update();
	void Draw();
	void Finitialize();

	/// <summary>
	/// �V�[����؂�ւ��鏈�����s���܂�
	/// </summary>
	/// <param name="nextScene"></param>
	void ChengeScene(Scene* nextScene);


	Scene* GetCurrentScene();
};

