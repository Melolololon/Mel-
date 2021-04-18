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
point tP;
sprite tS;
texture tex;

void Play::initialize()
{
	Library::createPoint(1, &tP);
	Library::createSprite(&tS);
	tex = Library::loadTexture(L"Resources/Texture/testTexture.png");
}

void Play::update()
{
	ObjectManager::getInstance()->update();
	ObjectManager::getInstance()->isDeadCheck();

}

void Play::draw()
{
	ObjectManager::getInstance()->draw();

	Library::drawSprite({ 0,0 }, tS, tex);
	Library::drawPointTexture({ 0,0,-3 }, tP, tex, 0);
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

