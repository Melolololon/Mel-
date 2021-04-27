#include "Play.h"

#include"Matrix.h"
#include"Quaternion.h"
#include"ObjectManager.h"


#include"TestObject.h"

#include"XInputManager.h"
#include"FbxModel.h"
#include"ObjModel.h"
std::unique_ptr<FbxModel> fbxModel = std::make_unique<FbxModel>();
std::unique_ptr<ObjModel> objModel = std::make_unique<ObjModel>();
Play::Play()
{

}


Play::~Play()
{
}
void Play::initialize()
{
	fbxModel->loadModel
	(
		"Resources/cube/cube.fbx",
		2
	);

	objModel->loadModel
	(
		"Resources/Obj/tank_head.obj",
		true,
		1
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
	fbxModel->setPosition({50,0,0}, 1);
}

void Play::draw()
{
	ObjectManager::getInstance()->draw();

	/*fbxModel->draw(0);
	fbxModel->draw(1);*/

	Library::setCamera({ 0,0,-20 }, { 0,0,0 }, { 0,1,0 });
	objModel->draw(0);
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

