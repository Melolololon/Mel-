#include "Play.h"

#include"Matrix.h"
#include"Quaternion.h"
#include"ObjectManager.h"
ModelData d;
ModelData d2;
ModelData modelD;
font f;
Play::Play()
{

	
	std::string material;
	modelD.key = "biru";
	
	//Library::loadOBJVertex("Resources/Obj/konnyakuBone.obj", true, true, &material, modelD);
	Library::loadOBJVertex("Resources/Obj/test.obj", true, true, &material, modelD);
	Library::loadOBJMaterial("Resources/Obj/", material, 2, modelD);
	//Library::setAngle({ 0,180,0 }, modelD, 0);

	//ボーンを先に乗算してるから動かしても意味ない
	//Library::setPosition({ 0,5,0 }, modelD, 0);


	//これでボックス出たからcreateHeapが悪い

	d.key = "Board";
	Library::createBoard({ 1,1 }, d);
	Library::createHeapData2({ 0,255,255,255 }, 1, d);

	d2.key = "Board2";
	Library::createBoard({ 5,5 },  d2);
	Library::createHeapData2({ 255,0,0,255 }, 1, d2);
	//Library::loadOBJMaterial("Resources/Obj/", material, 2, d);

	f = Library::loadSpriteFont(L"Resources/Font/font.png", { 14,7 });
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

UCHAR subAlpha = 0;

void Play::update()
{
	if (DirectInput::keyState(DIK_W))
		subAlpha++;
	if (DirectInput::keyState(DIK_S))
		subAlpha--;


	Library::setSubColor({ 0,0,0,subAlpha }, d, 0);

//	//今、xとzの+-が逆になってるから、どこがおかしいか探す
//	//モデル自身の回転のせいだった(ボーン適応させた後にモデル回転させたから)
//
///*
	if (DirectInput::keyState(DIK_A))
		modelZ += 2.0f;
	if (DirectInput::keyState(DIK_S))
		modelZ -= 2.0f;
	Library::setAngle({ 0,modelZ,0 }, modelD, 0);
//
//	if (DirectInput::keyState(DIK_Z))
//		addAngleY += 2.0f;
//	if (DirectInput::keyState(DIK_X))
//		addAngleY -= 2.0f;
//
//	Vector3 addVec(0, 0, addAngleY);
//	Library::setOBJBoneAngle(addVec, 2, modelD, 0);
//
//	Library::setParentOBJBone(3, 2, modelD);
//	Library::setParentOBJBone(4, 3, modelD);
//
//	
//	//できた
//	//OKなのにできてないと思い込んでた
//	//全て自分のボーンを基準に回すなら全部足してまとめずに拡縮、回転、平行移動を繰り返さないといけないけど、
//	//子ボーンに対応してるオブジェクトをすべて一番上の親(親が-1のボーン)基準で回すなら、まとめてしまっていい
//	//MMDなどで3Dモデル動かしたら、子は一番上の親基準で回ってたのでこれで良し
//	Library::setParentOBJBoneAngleImpact(4, {1,1,1}, modelD);
//	Library::setParentOBJBoneAngleImpact(3, {1,1,1}, modelD);

}

void Play::draw()
{


	Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	Library::setPosition({ 0,0,-0.5 }, d, 0);


	//d2の描画コマンドを先にセットしないと。dのコマンドセット時に自分の後ろに板ポリがない(DirectXが気が付いてない)ので
	//αブレンドできてなかった
	//Library::drawGraphic(d2, 0);
	//Library::drawGraphic(d, 0);
	

	std::string s = std::to_string(subAlpha);
	Library::drawsSpriteFontString({ 0,0 }, { 30,30 }, s, &f);
	Library::drawGraphic(modelD, 0);
	//Library::drawGraphic(modelD, 1);*/


}

void Play::end()
{
}

std::string Play::getNextScene()
{
	return "";
}
