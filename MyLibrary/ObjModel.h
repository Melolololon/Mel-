#pragma once
#include"Model.h"
#include"Texture.h"

//obj�̒��_�\���̂�A�{�[���Z�b�g�֐����g���A
//����ɒ��_�ɗp�ӂ����f�[�^�������������ꍇ�́A
//������p�����āAObj�̒��_�Ǝ����������f�[�^������������\���̂����A
//������g����map����
//��������΁A�f�[�^�����A�{�[���Z�b�g�֐��A�}�b�v�֐����g�����Ƃ��ł���
//�l�́AObj�̒��_�ƒǉ��f�[�^�Ŕz��𕪂��ĊǗ����Ă��炤

//����p�����Ȃ��Ă����쒸�_�g����悤�ɂł�����
//�ǂ����ɂ��邩�l����
//�p�����Ȃ��ƁA���쒸�_��Object�ɍ��Ȃ��Ƃ����Ȃ����A
//�����h�����߂ɃN���X��Draw�Ȃǂ̊֐������Ȃ��Ƃ����Ȃ����A
//�������|�����[�t�B�Y���̉��b���󂯂��Ȃ�(���̃��f���͂ł���̂�!)
//������p���̂ق�������?
//���쒸�_�̃T�C�Y�Z�b�g�֐����?
//ObjModel��FbxModel�̎��̓Z�b�g�������̉e�����󂯂Ȃ��悤�ɂ���?

//ObjModel���g���Ƃ��ȂǂɎ���\���̂̒萔�o�b�t�@�̃}�b�v���p�����Ȃ��Ă��ł���悤�ɂȂ��Ă��āA
//�f�[�^���܂Ƃ߂�ɂ̓N���X���Ȃ��Ƃ����Ȃ�����ǂ����݂��|�����[�t�B�Y���̉��b���󂯂��Ȃ�
//�|�����[�t�B�Y���̉��b���󂯂����ꍇ�́AModel�f�[�^�����������N���X�Ɏ����Őe�N���X����Čp�����Ă��炤?
//���ꂾ�Ɗ�����Model�N���X�ƈꏏ�ɂł��Ȃ�
//�����Őe�N���X��炸��Model��FbxObject�Ȃǂ̃N���X���p������΃|�����[�t�B�Y���̉��b���󂯂��?

//�Z�b�g�֐���邭�炢�Ȃ�A
//���[�h�֐��ɃZ�b�g���������܂܂ł���������
//�Ƃ肠����ObjModel��FbxModel�̎��̓Z�b�g�������̉e�����󂯂Ȃ��悤�ɂ���
//�Z�b�g�֐����Ȃ��ƃv���~�e�B�u���f���̊֐��S���ɃZ�b�g�֐����Ȃ��Ƃ����Ȃ��Ȃ�B
//����萔�o�b�t�@�\���̂��Z�b�g�֐����Ȃ��Ƃ����Ȃ�
//�萔�͑S���Ɏ�����������Ă���������

//�p�����Ȃ��ƒ��_�Ƃ����O��(Object�N���X)�ɍڂ��邱�ƂɂȂ�̂ŁA
//����όp�������ق����悳����

//��U���쒸�_�ۗ�����?

class ObjModel:public Model
{
private:

	static PipelineState defaultPipeline;

	std::string materialFileName;

	//���_
	std::vector<std::vector<ObjAnimationVertex>> vertices;
	std::vector<std::vector<DirectX::XMFLOAT3>> smoothNormal;

	//[obj���̃I�u�W�F�N�g��]�X���[�Y�V�F�[�f�B���O�̌v�Z�p�f�[�^
	std::vector< std::unordered_map < USHORT, std::vector<USHORT> >>smoothData;

	//[.obj���̃I�u�W�F�N�g����][���_����]
	std::vector<std::vector<int>> objBoneNums;//���_�Ɋ��蓖�Ă��Ă���{�[���ԍ�
	//[���f������(key�ŃA�N�Z�X)][�{�[������]
	std::vector<DirectX::XMFLOAT3> objBonePositions;//�{�[�����W
	//[�{�[������](�I�u�W�F�N�g���Ƃɕς���K�v�Ȃ������Ȃ̂ŁA�������Ă�)

	//�{�[����
	UINT boneNum;
	//[���f������][�{�[������]
	std::vector<std::vector<BoneData>>boneDatas;
	std::vector < std::vector<ParentBoneData>> parentBoneDatas;//�e�{�[���Ɖe���x

	std::vector<std::unique_ptr<Texture>>textures;

	/// <summary>
/// obj���f���̒��_��ǂݍ��݂܂��B
/// </summary>
/// <param name="path">���f���̃p�X</param>
/// <param name="loadUV">uv��ǂݍ��ނ�</param>
/// <param name="loadNormal">normal��ǂݍ��ނ�</param>
/// <param name="verticesSize">�V�F�[�_�[�ɑ��闘�p�҂��p�ӂ������_�f�[�^�̃o�C�g��(�p�ӂ��Ȃ��ꍇ��0)</param>
	void LoadModelVertices
	(
		const std::string& path,
		const bool loadUV,
		const bool loadNormal,
		const size_t vertexSize
	);

	void LoadModelMaterial
	(
		const std::string& directryPath,
		const int createNum,
		const size_t constDataSize
	);


	void MapBoneMatrix(const int modelNum);

public:
	ObjModel();
	~ObjModel();


	/// <summary>
	/// .obj���f����ǂݍ��݂܂��B
	/// </summary>
	/// <param name="path">�p�X</param>
	/// <param name="loadUV">UV��ǂݍ��ނ��ǂ���</param>
	/// <param name="createNum">������</param>
	///// <param name="vertexSize">�V�F�[�_�[�ɑ��闘�p�҂��p�ӂ������_�f�[�^�̃o�C�g��(�p�ӂ��Ȃ��ꍇ��0)</param>
	/// <param name="constDataSize">�V�F�[�_�[�ɑ��闘�p�҂��p�ӂ����萔�f�[�^�̃o�C�g��(�p�ӂ��Ȃ��ꍇ��0)</param>
	void LoadModel
	(
		const std::string& path,
		const bool loadUV,
		const int modelNum,
		//const size_t vertexSize,
		const size_t constDataSize
	);

#pragma region ����


	void SetBoneMoveVector
	(
		const Vector3& vector,
		const int boneNum,
		const int modelNum
	)
	{
		boneDatas[modelNum][boneNum].moveVector = vector.ToXMFLOAT3();
	}

	void SetBoneScale
	(
		const Vector3& scale,
		const int boneNum,
		const int modelNum
	)
	{
		boneDatas[modelNum][boneNum].scale = scale.ToXMFLOAT3();
	}

	void SetBoneAngle
	(
		const Vector3& angle,
		const int boneNum,
		const int modelNum
	)
	{
		boneDatas[modelNum][boneNum].angle = angle.ToXMFLOAT3();
	}

	void SetParentBone
	(
		const int bone,
		const int parentBone,
		const int modelNum
	)
	{
		parentBoneDatas[modelNum][bone].parentBoneNum = parentBone;
	}

	/// <summary>
	/// �e�̃E�B�����ǂ̂��炢�󂯂邩��ݒ肵�܂�
	/// </summary>
	/// <param name="impact">�{��</param>
	/// <param name="boneNum"></param>
	/// <param name="modelNum"></param>
	void SetMoveVectorImpact
	(
		const Vector3& impact,
		const int boneNum,
		const int modelNum
	)
	{
		parentBoneDatas[modelNum][boneNum].moveVectorImpact = impact.ToXMFLOAT3();
	}


	void SetAngleImpact
	(
		const Vector3& impact,
		const int boneNum,
		const int modelNum
	)
	{
		parentBoneDatas[modelNum][boneNum].angleImpact = impact.ToXMFLOAT3();
	}

	void SetScaleImpact
	(
		const Vector3& impact,
		const int boneNum,
		const int modelNum
	)
	{
		parentBoneDatas[modelNum][boneNum].scaleImpact = impact.ToXMFLOAT3();
	}


#pragma endregion

#pragma region ���擾
	std::vector<Vector3>GetBonePosition();
#pragma endregion


	static bool Initialize();
	void Draw(const int modelNum)override;

	static PipelineState GetDefaultPipeline() { return defaultPipeline; }
};

