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
	Library::setCamera({ 0,0,-10 }, { 0,0,0 }, { 0,1,0 });
}

Vector3 pPos = 0;

void Play::update()
{
	ObjectManager::getInstance()->update();
	ObjectManager::getInstance()->isDeadCheck();

	if (DirectInput::keyState(DIK_UP))
		pPos.y += 0.1f;
	if (DirectInput::keyState(DIK_DOWN))
		pPos.y -= 0.1f;
	if (DirectInput::keyState(DIK_RIGHT))
		pPos.x += 0.1f;
	if (DirectInput::keyState(DIK_LEFT))
		pPos.x -= 0.1f;

	//Library::setPointScale({ 10,10}, tP, 0);
}

void Play::draw()
{
	ObjectManager::getInstance()->draw();

	//Library::drawSprite({ 0,0 }, tS, tex);
	Library::draw3DSprite({ 0,0,0 }, { 1,1 }, tS, tex);

	/*if (DirectInput::keyState(DIK_SPACE))
		Library::setIsBillboard(true, true, true);
	else
		Library::setIsBillboard(false, false, false);*/
	//Library::drawPointTexture(pPos, tP, tex, 0);
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

