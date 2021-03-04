#pragma once
#include<vector>
#include<unordered_map>
#include"Scene.h"
class SceneManager final
{

private:

	SceneManager();
	~SceneManager();

	std::unordered_map <std::string, Scene*>scenes;//�V�[��
	std::unordered_map <std::string, Scene*>smallScenes;//�V�[���ŌĂяo���V�[��()
	std::string currentScene;//���݂̃V�[��
public:

	SceneManager(const SceneManager& sceneManager) = delete;//�R�s�[�R���X�g���N�^
	SceneManager& operator=(const SceneManager& sceneManager) = delete;//�R�s�[������Z�q
	static SceneManager* getInstace();

	void initialize();
	void update();
	void draw();
	void end();

	/// <summary>
	/// �V�[���̒ǉ�
	/// </summary>
	/// <param name="key">�V�[����</param>
	/// <param name="scene">�V�[���N���X</param>
	void addScene(std::string key, Scene* scene);

	/// <summary>
	/// �V�[�����擾���܂�
	/// </summary>
	/// <param name="key"></param>
	/// <returns></returns>
	void* getScenePtr(std::string key);
};

