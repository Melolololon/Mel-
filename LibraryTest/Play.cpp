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

	//3���A���O���Ƃ��ݒ肵�ĂȂ���0�ɂȂ��Ă�����A4�̉e���x�������Ă�0�|��������ĕω��Ȃ�����
	//�s��i�[���Ă���|���Ă��_��(�����̉e���x���v�Z�ł��Ȃ��Ȃ�)
	//�e�̊p�x�Ƃ��i�[���Ď����ɃZ�b�g����Ă�e��������Ƃ������A�v�Z����?����ł�����
	//�����̐e��A�e�̐e(-1�ɂȂ�܂�)�̊p�x��e���x�����ׂĊi�[���āA�v�Z���邵���Ȃ�?
	
	//2�s��(�s��i�[���Ă���|���Ă��_��(�����̉e���x���v�Z�ł��Ȃ��Ȃ�))
	//�̕��@���s���A��]�e���x�͉�]�s��ɂ������Ƃ��낾���e���x�|����Ȃǂ���΂���?
	//�r���[�s��Ƃ������Ȃ����炻�̕ӐS�z����K�v�Ȃ��B�������Ƃ���|���đ��v���Ǝv��
	//��Z�ザ��Ȃ��āA��Z�O�̂��ʂɊi�[����΂�����?

	//�K�v�Ȃ���
	//-1�ɂȂ�܂łɊ|����{�[���̃X�P�[���A�p�x�A�ړ��ʂƂ����̉e���x
	//�s��ɂ���O�ɁA��Ɍv�Z����(�X�P�[���̏ꍇ�A�S�Ă̊p�x�Ɖe���x�������āA���̐��l�𗘗p���āA�s������)
	//�����ł���

	//�e�̐e�̍s��擾���āA
	//�e *= �e�̐e�ɂ���΂���?
	//�e�̍��Ƃ��ɉe���x�|���āA
	//���� *= �e�ɂ���?
	//����

	//�e�̐e�̍s����v�Z����(���̎��A�e�Ǝq�̉e���x���|����)
	//�e�̍s����v�Z����(���Ƃł�邽�߁A�����Őe * �e�̐e������K�v�͂Ȃ�)(���̎��A�q�ɉe���x���|����)
	//�q�̍s����v�Z����
	//�q * �e * �e�̐e �̏��Ōv�Z
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
