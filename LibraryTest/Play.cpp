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
	
	Library::loadOBJVertex("Resources/Obj/manyV.obj", true, true, &material, modelD);
	Library::loadOBJMaterial("Resources/Obj/", material, 1, modelD);
	//Library::setAngle({ 0,180,0 }, modelD, 0);
}


Play::~Play()
{
}

void Play::initialize()
{
}
Vector3 vec(1);

float addAngleY = 40.0f;
int bairituNum = -3;
void Play::update()
{
	//今、xとzの+-が逆になってるから、どこがおかしいか探す
	//モデル自身の回転のせいだった(ボーン適応させた後にモデル回転させたから)
	if (DirectInput::keyState(DIK_D))
		vec.y += 5.0f;
	if (DirectInput::keyState(DIK_A))
		vec.y -= 5.0f;

	Library::setOBJBoneAngle(vec, 0, modelD, 0);
	//Library::setOBJBoneAngle(vec, 1, modelD, 0);


	Library::setParentOBJBone(1, 0, modelD);
	if (DirectInput::keyState(DIK_Z)) 
	{

		Library::setParentOBJBoneAngleImpact(1, { 0.2f,0.2f,0.2f }, modelD);
	}
	else
	{

		Library::setParentOBJBoneAngleImpact(1, { 1,1,1 }, modelD);
		
	}

	/*if (DirectInput::keyState(DIK_Z))
		addAngleY += 5.0f;
	if (DirectInput::keyState(DIK_X))
		addAngleY -= 5.0f;

	if (DirectInput::keyState(DIK_S)) {


		Vector3 addVec(0, addAngleY *bairituNum, 0);
		Library::setOBJBoneAngle(vec + addVec, 0, modelD, 0);

		addVec = { 0, addAngleY * (bairituNum + 1) , 0 };
		Library::setOBJBoneAngle(vec + addVec, 1, modelD, 0);

		addVec = { 0,addAngleY  *(bairituNum + 2),0 };
		Library::setOBJBoneAngle(vec + addVec, 2, modelD, 0);

		addVec = { 0,addAngleY  *(bairituNum + 3),0 };
		Library::setOBJBoneAngle(vec + addVec, 3, modelD, 0);


		addVec = { 0,addAngleY * (bairituNum + 4),0 };
		Library::setOBJBoneAngle(vec + addVec, 4, modelD, 0);

		addVec = { 0,addAngleY *(bairituNum + 5),0 };
		Library::setOBJBoneAngle(vec + addVec, 5, modelD, 0);
	}
	else
	{
		Library::setOBJBoneAngle(vec, 0, modelD, 0);
		Library::setOBJBoneAngle(vec, 1, modelD, 0);
		Library::setOBJBoneAngle(vec, 2, modelD, 0);
		Library::setOBJBoneAngle(vec, 3, modelD, 0);
		Library::setOBJBoneAngle(vec, 4, modelD, 0);
		Library::setOBJBoneAngle(vec, 5, modelD, 0);
	}*/
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
