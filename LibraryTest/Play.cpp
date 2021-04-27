#include "Play.h"

#include"Matrix.h"
#include"Quaternion.h"
#include"ObjectManager.h"


#include"TestObject.h"

#include"XInputManager.h"
#include"FbxModel.h"
std::unique_ptr<FbxModel> model = std::make_unique<FbxModel>();
Play::Play()
{

}


Play::~Play()
{
}
void Play::initialize()
{
	model->loadModel
	(
		"Resources/cube/cube.fbx",
		2
	);
	
}

Vector3 angle = 0;
void Play::update()
{
	ObjectManager::getInstance()->update();
	ObjectManager::getInstance()->isDeadCheck();


	if (DirectInput::keyState(DIK_W))
		angle.y += 3.1f;
	if (DirectInput::keyState(DIK_S))
		angle.y -= 3.1f;
	if (DirectInput::keyState(DIK_A))
		angle.x += 3.1f;
	if (DirectInput::keyState(DIK_D))
		angle.x -= 3.1f;

	//model->setAngle(angle,0);
	model->setPosition({50,0,0}, 1);
}

void Play::draw()
{
	ObjectManager::getInstance()->draw();

	model->draw(0);
	model->draw(1);
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

