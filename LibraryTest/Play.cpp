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

	//�{�[�����ɏ�Z���Ă邩�瓮�����Ă��Ӗ��Ȃ�
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
	//���Ax��z��+-���t�ɂȂ��Ă邩��A�ǂ��������������T��
	//���f�����g�̉�]�̂���������(�{�[���K����������Ƀ��f����]����������)

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

	
	//�ł���
	//OK�Ȃ̂ɂł��ĂȂ��Ǝv������ł�
	//�S�Ď����̃{�[������ɉ񂷂Ȃ�S�������Ă܂Ƃ߂��Ɋg�k�A��]�A���s�ړ����J��Ԃ��Ȃ��Ƃ����Ȃ����ǁA
	//�q�{�[���ɑΉ����Ă�I�u�W�F�N�g�����ׂĈ�ԏ�̐e(�e��-1�̃{�[��)��ŉ񂷂Ȃ�A�܂Ƃ߂Ă��܂��Ă���
	//MMD�Ȃǂ�3D���f������������A�q�͈�ԏ�̐e��ŉ���Ă��̂ł���ŗǂ�
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
