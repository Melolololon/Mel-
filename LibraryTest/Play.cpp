#include "Play.h"

#include"Matrix.h"
#include"Quaternion.h"
#include"ObjectManager.h"


#include"TestObject.h"

#include"XInputManager.h"


Play::Play()
{

}


Play::~Play()
{
}

void Play::initialize()
{


	t = std::make_shared<TestObject>();
}

void Play::update()
{
	ObjectManager::getInstance()->update();
	ObjectManager::getInstance()->isDeadCheck();

}

void Play::draw()
{
	if(t)
	t->draw();
	
	ObjectManager::getInstance()->draw();

	if (DirectInput::keyTrigger(DIK_A))
		isEnd = true;
}

void Play::end()
{
}

Scene* Play::getNextScene()
{
	return new Play();
}

void Play::loadResources()
{
}

