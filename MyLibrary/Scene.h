#pragma once
#include<string>

namespace MelLib 
{
	class Scene
	{

	protected:
		bool isEnd = false;

	public:
		Scene();
		virtual ~Scene();
		void FalseIsEnd();
		virtual void Initialize() = 0;//������
		virtual void Update() = 0;
		virtual void Draw() = 0;
		virtual void Finitialize() = 0;//�I������
		virtual Scene* GetNextScene() = 0;//���̃V�[���̎w��
		bool GetIsEnd();//�V�[�����I��������ǂ���
		void* GetPtr();
		void ResourceBatchDeletion();
	};
}
