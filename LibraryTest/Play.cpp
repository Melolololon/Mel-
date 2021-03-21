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

	//�{�[�����ɏ�Z���Ă邩�瓮�����Ă��Ӗ��Ȃ�
	//Library::setPosition({ 0,5,0 }, modelD, 0);


	//����Ń{�b�N�X�o������createHeap������

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

//	//���Ax��z��+-���t�ɂȂ��Ă邩��A�ǂ��������������T��
//	//���f�����g�̉�]�̂���������(�{�[���K����������Ƀ��f����]����������)
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
//	//�ł���
//	//OK�Ȃ̂ɂł��ĂȂ��Ǝv������ł�
//	//�S�Ď����̃{�[������ɉ񂷂Ȃ�S�������Ă܂Ƃ߂��Ɋg�k�A��]�A���s�ړ����J��Ԃ��Ȃ��Ƃ����Ȃ����ǁA
//	//�q�{�[���ɑΉ����Ă�I�u�W�F�N�g�����ׂĈ�ԏ�̐e(�e��-1�̃{�[��)��ŉ񂷂Ȃ�A�܂Ƃ߂Ă��܂��Ă���
//	//MMD�Ȃǂ�3D���f������������A�q�͈�ԏ�̐e��ŉ���Ă��̂ł���ŗǂ�
//	Library::setParentOBJBoneAngleImpact(4, {1,1,1}, modelD);
//	Library::setParentOBJBoneAngleImpact(3, {1,1,1}, modelD);

}

void Play::draw()
{


	Library::setPipeline(PIPELINE_OBJ_ANIMATION);
	Library::setPosition({ 0,0,-0.5 }, d, 0);


	//d2�̕`��R�}���h���ɃZ�b�g���Ȃ��ƁBd�̃R�}���h�Z�b�g���Ɏ����̌��ɔ|�����Ȃ�(DirectX���C���t���ĂȂ�)�̂�
	//���u�����h�ł��ĂȂ�����
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
