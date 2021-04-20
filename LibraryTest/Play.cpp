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
sprite tS2;
texture tex;
texture tex2;

ModelData m;
ModelData box;
void Play::initialize()
{
	Library::createPoint(1, &tP);
	Library::createSprite(&tS);
	Library::createSprite(&tS2);
	tex = Library::loadTexture(L"Resources/Texture/testTexture.png");
	tex2 = Library::loadTexture(L"Resources/Texture/testTexture2.png");
	Library::setCamera({ 0,0,-10 }, { 0,0,0 }, { 0,1,0 });

	m.key = "tank";
	std::string mat;
	Library::loadOBJVertex("Resources/Obj/tank_body.obj", true, true, &mat, m);
	Library::loadOBJMaterial("Resources/Obj/", mat, 1, m);

	Vector3 point;
	Vector3 max;
	Vector3 min;
	Vector3 size;
	Library::getModelBoxCollisionData
	(
		m,
		&point,
		&min,
		&max,
		&size
	);

	box.key = "box";
	Library::create3DBox(size, box);
	Library::createHeapData2({ 255,255,255,255 }, 1, box);
	Library::setPosition(point, box, 0);
	
}

Vector3 pPos = 0;
Vector3 angle = 0;
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


	if (DirectInput::keyState(DIK_W))
		angle.y += 3.1f;
	if (DirectInput::keyState(DIK_S))
		angle.y -= 3.1f;
	if (DirectInput::keyState(DIK_A))
		angle.x += 3.1f;
	if (DirectInput::keyState(DIK_D))
		angle.x -= 3.1f;
	Library::setAngle(angle, m, 0);
	Library::setAngle(angle, box, 0);
}

void Play::draw()
{
	ObjectManager::getInstance()->draw();

	//Library::drawSprite({ 0,0 }, tS, tex);
	//Library::drawSprite({ 50,0 }, tS2, tex2);
	//Library::draw3DSprite({ 0,0,0 }, { 1,1 }, tS, tex);
	//if(DirectInput::keyState(DIK_Z))
	//Library::draw3DSpriteAnimation({ 0,0,0 }, { 5,5 }, { 0,0 }, {128,128}, tS, tex);
	//else
	//	Library::draw3DSpriteAnimation({ 0,0,0 }, { 5,5 }, { 0,0 }, { 128,128 }, tS, tex2);
	//Library::drawSprite3DBox({ 0,0,0 }, { 5,5 }, { 0,255,0,255 }, tS);
	//Library::drawSprite3DBox({ 3,0,3 }, { 5,5 }, { 255,0,0,255 }, tS2);

	if (DirectInput::keyState(DIK_SPACE))
		Library::setIsBillboard(true, true, true);
	else
		Library::setIsBillboard(false, false, false);
	//Library::drawPointTexture(pPos, tP, tex, 0);
	
	if(DirectInput::keyState(DIK_Z))
	Library::drawGraphic(m, 0);
	if (DirectInput::keyState(DIK_X))
	Library::drawGraphic(box, 0);
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

