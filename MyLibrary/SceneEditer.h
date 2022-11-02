#pragma once

#include"GameObject.h"

namespace MelLib 
{
	// objects�̃I�u�W�F�N�g���ǂ��}�l�[�W���[�ɒǉ�����?

	// �ۑ����邲�ƂɎ����o�b�N�A�b�v�Ƃ�悤�ɂ�����A
	// ����ۑ�����Ctrl+Z�Ŗ߂��悤�ɂ��������ǂ���

	// �I�u�W�F�N�g�ꗗ�̓G�f�B�^�[�̋@�\�Ȃ̂ł�����Ɏ������Ă���
	// �I�u�W�F�N�g�}�l�[�W���[�Ɏ�������ƌ������ɂ��I���I�t�t���O�p�ӂ��Ȃ��Ƃ����Ȃ������p�҂���₱�����Ȃ�(�������̃t���O�Q�Ƃ��Ă���������)
	// �I�u�W�F�N�g�}�l�[�W���[�ɃI�u�W�F�N�g�擾����֐�����āA�I�΂ꂽ�I�u�W�F�N�g���擾���Ă���ɉ����ĕ`�悷��p�����[�^�[�̃E�B���h�E��ς���
	// GUIValueManager�ɃE�B���h�E�̕\���t���O���Z�b�g����֐����
	// GameObject�N���X�ł��̃t���O���Z�b�g�ł���悤�ɂ���
	// ����ɂ��A�V�[���G�f�B�^�[�őI�΂ꂽ���̂̂ݕ\�����邱�Ƃ��ł���

	// ���I�΂�Ă�I�u�W�F�N�g�̃E�B���h�E���\��
	// �V�����I�u�W�F�N�g�����ݑI������Ă���I�u�W�F�N�g������ϐ��ɒǉ�
	// �V�����I�u�W�F�N�g�̃E�B���h�E��\��

	// �ǉ������I�u�W�F�N�g��o�^�ł���悤�ɂ���
	// �o�^����Ƃ��ɍ�����o�^�֐����g���Ίy

	class SceneEditer
	{

	private:
		SceneEditer(){}
		~SceneEditer(){}

		SceneEditer(SceneEditer& s) = delete;
		SceneEditer& operator=(SceneEditer& s) = delete;


		void Save();
		void SaveEditData();
		void InputEditDataName();
		void SaveRegisterObject();

		void Load();

		void UpdateSelectObject();

		void DrawObjectList();

		void SetDrawWindowFlag(const std::vector<std::string>& objNames);

		void RegisterSelectObject();
		void InputObjectName();
		void InputObjectType();

		std::string GetObjectType(const GameObject& object)const;
	private:
		static const std::string EDIT_DATA_FORMAT;
		static const std::string REGISTER_OBJECT_DATA_FORMAT;

		// �����ێ����Ƃ��΃��W�I�{�^���̒l��for�ŉ񂵂ăf�[�^�ɃA�N�Z�X�ł���(int�ŃA�N�Z�X�ł���)����map
		// �����𑼂̔z��ɕۑ�����̂����������B���[�v����K�v�Ȃ��Ȃ邩�瑁���B���ǃ������g��
		// for�Ŏ擾����ƎQ�Ƃɑ���ł��Ȃ����Avector�̃|�C���^�͌l�I�Ɏg�������Ȃ����Afor�ł����Q�Ƃł��Ȃ����番����
		// ���W�I�{�^���̏��Ԃ߂��Ⴍ����ɂȂ邩��map�ɕύX
		// map��string���Ɩ��O���ɕ��ׂĂ����
		// �I�u�W�F�N�g
		std::map< std::string,std::map<std::string,std::shared_ptr<MelLib::GameObject>>>pRegisterObjects;
		// ���W�I�{�^���̖߂�l�ŏ��objects�ɃA�N�Z�X���邽�߂̔z��
		// ���W�I�{�^���̖߂�l�ł��̔z�񂩂�pObjects�̃L�[�����o������ApObjects��vector�ɃA�N�Z�X����
		std::unordered_map<std::string,std::unordered_map<int ,std::string>>registerObjectOrderDatas;
		std::vector<std::string>registerObjectTypes;
		std::vector<std::string>registerObjectNames;
		 
		// �I������Ă���^�C�v
		int selectType = 0;

		GameObject* pEditSelectObject = nullptr;

		bool isEdit = false;
		bool releaseEdit = false;

		int selectListObjectNum = 0;
		std::string selectListObjectName;

		bool inpttingObjectName = false;
		bool inpttingObjectType = false;
		bool inpttingEditDataName = false;

		std::string inputObjectName;
		std::string inputObjectType;
		std::string inputEditDataName;
	public:
		static SceneEditer* GetInstance();


		/// <summary>
		/// �I�u�W�F�N�g���G�f�B�^�ɓo�^���܂��B
		/// </summary>
		/// <param name="object">�I�u�W�F�N�g</param>
		/// <param name="objectType">�I�u�W�F�N�g�̎��(Actor��StageObject�Ȃ�)</param>
		void RegisterObject(const std::shared_ptr<MelLib::GameObject>& pObject,const std::string& objectType);
		
		void Update();
		void Draw();

		/// <summary>
		/// �G�f�B�^�̃I���I�t��ݒ肵�܂��B�����[�X�r���h���ɃG�f�B�^���I���ɂł���悤�ɂ��ĂȂ��ꍇ�̓I���ɂł��܂���B
		/// </summary>
		/// <param name="flag"></param>
		void SetEditFlag(const bool flag);
		
		/// <summary>
		/// �����[�X�r���h���ɃG�f�B�^���I���ɂł���悤�ɂ��邩�ǂ�����ݒ肵�܂��B
		/// </summary>
		/// <param name="flag"></param>
		void SetReleaseEditFlag(const bool flag) { releaseEdit = flag; }
	};
}
