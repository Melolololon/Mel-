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
	virtual void initialize() = 0;//������
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void end() = 0;//�I������
	virtual std::string getNextScene() = 0;//���̃V�[���̎w��
	bool getIsEnd();//�V�[�����I��������ǂ���

	void* getPtr();
};

