#include "Play.h"

#include"Matrix.h"
#include"Quaternion.h"

ModelData d;
ModelData modelD;
Play::Play()
{

	d.key = "Board";
	Library::createBoard({ 1,1 }, dimention3D, d);
	Library::createHeapData2({ 255,255,255,255 }, 1, d);

	std::string material;
	modelD.key = "biru";
	
	Library::loadOBJVertex("Resources/Obj/konnyakuBone.obj", true, true, &material, modelD);
	Library::loadOBJMaterial("Resources/Obj/", material, 1, modelD);
	//Library::setAngle({ 0,180,0 }, modelD, 0);

	//ボーンを先に乗算してるから動かしても意味ない
	//Library::setPosition({ 0,5,0 }, modelD, 0);
}


Play::~Play()
{
}

void Play::initialize()
{
}
Vector3 vec(1);

float addAngleY = 0.0f;
int bairituNum = -3;
float modelZ = 0;
void Play::update()
{
	//今、xとzの+-が逆になってるから、どこがおかしいか探す
	//モデル自身の回転のせいだった(ボーン適応させた後にモデル回転させたから)

/*
	if (DirectInput::keyState(DIK_A))
		modelZ += 2.0f;
	if (DirectInput::keyState(DIK_S))
		modelZ -= 2.0f;
	Library::setAngle({ 0,0,modelZ }, modelD, 0);*/

	if (DirectInput::keyState(DIK_Z))
		addAngleY += 2.0f;
	if (DirectInput::keyState(DIK_X))
		addAngleY -= 2.0f;

	Vector3 addVec(0, 0, addAngleY);
	Library::setOBJBoneAngle(addVec, 2, modelD, 0);

	Library::setParentOBJBone(3, 2, modelD);
	Library::setParentOBJBone(4, 3, modelD);

	
	//できた
	//OKなのにできてないと思い込んでた
	//全て自分のボーンを基準に回すなら全部足してまとめずに拡縮、回転、平行移動を繰り返さないといけないけど、
	//子ボーンに対応してるオブジェクトをすべて一番上の親(親が-1のボーン)基準で回すなら、まとめてしまっていい
	//MMDなどで3Dモデル動かしたら、子は一番上の親基準で回ってたのでこれで良し
	Library::setParentOBJBoneAngleImpact(4, {1,1,1}, modelD);
	Library::setParentOBJBoneAngleImpact(3, {1,1,1}, modelD);

}

void Play::draw()
{
	Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	//Library::drawGraphic(d, 0);
	Library::drawGraphic(modelD, 0);

	Quaternion p(10,10,10,10);
	Quaternion q(10,10,10,10);
	p = p * q;
	Quaternion r = getRotateQuaternion({ 10,0,0 }, { 0,1,0 }, 180);

	Vector2 v;
	v = LibMath::angleToVector2(90);
	
	Matrix m1 = getIdentityMatrix();
	Matrix m2 = m1;

	int z = 0;


}

void Play::end()
{
}

std::string Play::getNextScene()
{
	return "";
}
