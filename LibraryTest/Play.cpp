#include "Play.h"

#include"Matrix.h"

ModelData d;
ModelData modelD;
Play::Play()
{

	d.key = "Board";
	Library::createBoard({ 1,1 }, dimention3D, d);
	Library::createHeapData2({ 255,255,255,255 }, 1, d);

	std::string material;
	modelD.key = "biru";
	
	Library::loadOBJVertex("Resources/Obj/test.obj", true, true, &material, modelD);
	Library::loadOBJMaterial("Resources/Obj/", material, 1, modelD);
	Library::setAngle({ 0,180,0 }, modelD, 0);
}


Play::~Play()
{
}

void Play::initialize()
{
}
Vector3 vec(0);
void Play::update()
{
	if (DirectInput::keyState(DIK_A))
		vec.y += 3.0f;
	if (DirectInput::keyState(DIK_D))
		vec.y -= 3.0f;


	Library::setOBJBoneAngle(vec, 0, modelD, 0);
}

void Play::draw()
{
	Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	//Library::drawGraphic(d, 0);
	Library::drawGraphic(modelD, 0);

}

void Play::end()
{
}

std::string Play::getNextScene()
{
	return "";
}
