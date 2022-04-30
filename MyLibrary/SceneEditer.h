#pragma once

#include"GameObject.h"

namespace MelLib 
{
	// objects�̃I�u�W�F�N�g���ǂ��}�l�[�W���[�ɒǉ�����?

	class SceneEditer
	{
	private:
		SceneEditer(){}
		~SceneEditer(){}

		SceneEditer(SceneEditer& s) = delete;
		SceneEditer& operator=(SceneEditer& s) = delete;

		void Save();
	private:
		// �����ێ����Ƃ��΃��W�I�{�^���̒l��for�ŉ񂵂ăf�[�^�ɃA�N�Z�X�ł���(int�ŃA�N�Z�X�ł���)����map
		// �����𑼂̔z��ɕۑ�����̂����������B���[�v����K�v�Ȃ��Ȃ邩�瑁���B���ǃ������g��
		// for�Ŏ擾����ƎQ�Ƃɑ���ł��Ȃ����Avector�̃|�C���^�͌l�I�Ɏg�������Ȃ����Afor�ł����Q�Ƃł��Ȃ����番����
		// �I�u�W�F�N�g
		std::unordered_map< std::string,std::vector<std::shared_ptr<MelLib::GameObject>>>pObjects;
		std::vector<std::shared_ptr<MelLib::GameObject>>test;
		// ���W�I�{�^���̖߂�l�ŏ��objects�ɃA�N�Z�X���邽�߂̔z��
		std::unordered_map<int ,std::string>objectOrderDatas;
		 
		// �I������Ă���^�C�v
		int selectType = 0;

		GameObject* pSelectObject = nullptr;

	public:
		static SceneEditer* GetInstance();

		/// <summary>
		/// �I�u�W�F�N�g���G�f�B�^�ɓo�^���܂��B
		/// </summary>
		/// <param name="object">�I�u�W�F�N�g</param>
		/// <param name="objectType">�I�u�W�F�N�g�̎��</param>
		void RegisterObject(const std::shared_ptr<MelLib::GameObject>& pObject,const std::string& objectType);
		
		void Update();

		void Draw();
	};
}