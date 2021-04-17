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
	static SceneManager* getInstace();

	void initialize(Scene* startScene);
	void update();
	void draw();
	void end();

	/// <summary>
	/// �V�[����؂�ւ��鏈�����s���܂�
	/// </summary>
	/// <param name="nextScene"></param>
	void chengeScene(Scene* nextScene);


	Scene* getCurrentScene();
};

