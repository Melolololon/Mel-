#include "Play.h"

#include"Matrix.h"
#include"Quaternion.h"
#include"ObjectManager.h"

#include"TestObject.h"

#include"XInputManager.h"
ModelData d;
ModelData d2;
ModelData modelD;
font f;

ModelData tankHead;

Play::Play()
{

	
	std::string material;
	modelD.key = "biru";
	
	//Library::loadOBJVertex("Resources/Obj/konnyakuBone.obj", true, true, &material, modelD);
	Library::loadOBJVertex("Resources/Obj/test.obj", true, true, &material, modelD);
	Library::loadOBJMaterial("Resources/Obj/", material, 2, modelD);
	//Library::setAngle({ 0,180,0 }, modelD, 0);

	Library::setPosition({ 5,0,0 }, modelD, 1);


	//これでボックス出たからcreateHeapが悪い

	//d.key = "Board";
	//Library::createBoard({ 1,1 }, d);
	//Library::createHeapData2({ 0,255,255,255 }, 1, d);

	//d2.key = "Board2";
	//Library::createBoard({ 5,5 },  d2);
	//Library::createHeapData2({ 255,0,0,255 }, 1, d2);
	////Library::loadOBJMaterial("Resources/Obj/", material, 2, d);

	f = Library::loadSpriteFont(L"Resources/Font/font.png", { 14,7 });


	//ObjectManager::getInstance()->addObject(new TestObject({ 0,0,0 }));
	//ObjectManager::getInstance()->addObject(new TestObject({ 10,0,10 }));
	ObjectManager::getInstance()->objectSort(ObjectManager::OBJECT_SORT_NEAR_DISTANCE,false);

	tankHead.key = "tankHead";
	Library::loadOBJVertex("Resources/Obj/tank_head.obj", true, true, &material, tankHead);
	Library::loadOBJMaterial("Resources/Obj/", material, 1, tankHead);
}


Play::~Play()
{
}

void Play::initialize()
{

}
Vector3 vec(1);

float addAngle = 0.0f;
int bairituNum = -3;
float modelAng = 0;

UCHAR subAlpha = 0;

void Play::update()
{
	ObjectManager::getInstance()->update();

	if (DirectInput::keyState(DIK_A))
		addAngle += 3;
	if (DirectInput::keyState(DIK_S))
		addAngle -= 3;
	if (DirectInput::keyState(DIK_Z))
		modelAng += 3;
	if (DirectInput::keyState(DIK_X))
		modelAng -= 3;

	Library::setOBJBoneAngle({ 0,addAngle ,0 }, 0, modelD, 0);
	Library::setAngle({ 0,modelAng,0 }, modelD, 0);
	Library::setOBJBoneAngle({ 0,addAngle ,0 }, 0, modelD, 1);
	Library::setAngle({ 0,modelAng,0 }, modelD, 1);


}

Vector3 spherePos = 0;
Vector3 size = { 4,8,8 };
Vector3 center = { 6,0,0 };

void Play::draw()
{

	
	ObjectManager::getInstance()->draw();
	//Library::drawGraphic(d, 0);

	//Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	//Library::drawGraphic(modelD, 0);
	//Library::drawGraphic(modelD, 1);
	//Library::drawGraphic(modelD, 1);*/
	Library::drawGraphic(tankHead, 0);


	if (DirectInput::keyState(DIK_A))
		spherePos.x -= 0.2f;
	if (DirectInput::keyState(DIK_D))
		spherePos.x += 0.2f;
	if (DirectInput::keyState(DIK_S))
		spherePos.z -= 0.2f;
	if (DirectInput::keyState(DIK_W))
		spherePos.z += 0.2f;

	BoxHitDirection dis;
	bool flag = LibMath::sphereAndBoxCollision
	(
		spherePos,
		2.0f,
		center,
		size,
		&dis
	);

	//std::string str = std::to_string(flag) + "_" + std::to_string(spherePos.x) + ","+std::to_string(spherePos.z);
	std::string str = std::to_string(XInputManager::leftStickAngle(1));
	Library::drawsSpriteFontString({ 0,0 }, { 30,30 }, str,&f);

	

}

void Play::end()
{
}

std::string Play::getNextScene()
{
	return "";
}
