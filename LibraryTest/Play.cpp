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

	//3がアングルとか設定してなくて0になってたから、4の影響度いじっても0掛けちゃって変化なかった
	//行列格納してそれ掛けてもダメ(自分の影響度を計算できなくなる)
	//親の角度とか格納して自分にセットされてる親をかけるときだけ、計算する?それでも無理
	//自分の親や、親の親(-1になるまで)の角度や影響度をすべて格納して、計算するしかない?
	
	//2行目(行列格納してそれ掛けてもダメ(自分の影響度を計算できなくなる))
	//の方法を行い、回転影響度は回転行列にかかわるところだけ影響度掛けるなどすればいい?
	//ビュー行列とかかけないからその辺心配する必要ない。かかわるところ掛けて大丈夫だと思う
	//乗算後じゃなくて、乗算前のを個別に格納すればいける?

	//必要なもの
	//-1になるまでに掛けるボーンのスケール、角度、移動量とそれらの影響度
	//行列にする前に、先に計算する(スケールの場合、全ての角度と影響度をかけて、その数値を利用して、行列を作る)
	//無理でした

	//親の親の行列取得して、
	//親 *= 親の親にすればいい?
	//親の作るときに影響度掛けて、
	//自分 *= 親にする?
	//無理

	//親の親の行列を計算する(この時、親と子の影響度を掛ける)
	//親の行列を計算する(あとでやるため、ここで親 * 親の親をする必要はない)(この時、子に影響度を掛ける)
	//子の行列を計算する
	//子 * 親 * 親の親 の順で計算
	Library::setParentOBJBoneAngleImpact(4, { 1.5,1.5,1.5 }, modelD);
	Library::setParentOBJBoneAngleImpact(3, { 1.5,1.5,1.5 }, modelD);


	//Library::setParentOBJBone(4, 5, modelD);
	//Library::setParentOBJBone(3, 4, modelD);
	///*Library::setParentOBJBone(2, 3, modelD);
	//Library::setParentOBJBone(1, 2, modelD);
	//Library::setParentOBJBone(0, 1, modelD);*/

	///*Library::setParentOBJBoneAngleImpact(0, { 2,2,2 }, modelD);
	//Library::setParentOBJBoneAngleImpact(1, { 2,2,2 }, modelD);
	//Library::setParentOBJBoneAngleImpact(2, { 2,2,2 }, modelD);*/
	//Library::setParentOBJBoneAngleImpact(3, { 2,2,2 }, modelD);
	//Library::setParentOBJBoneAngleImpact(4, { 2,2,2 }, modelD);
	////Library::setParentOBJBoneAngleImpact(1, { 2,2,2 }, modelD);
	////Library::setParentOBJBoneAngleImpact(2, { 2,2,2 }, modelD);
	///*Library::setParentOBJBone(0, 1, modelD);
	//Library::setParentOBJBone(1, 2, modelD);
	//Library::setParentOBJBone(2, 3, modelD);*/
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
