#pragma once
#include"DirectInput.h"
#include<string>
#pragma region Object

#pragma endregion
class Scene
{

protected:
	bool isEnd;

public:
	Scene();
	virtual ~Scene();
	void falseIsEnd();
	virtual void Initialize() = 0;//������
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void end() = 0;//�I������
	virtual Scene* GetNextScene() = 0;//���̃V�[���̎w��
	bool GetIsEnd();//�V�[�����I��������ǂ���

	void* GetPtr();
};

