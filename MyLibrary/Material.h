#pragma once
#include<d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include<wrl.h>

#include"Values.h"
#include"Texture.h"

namespace MelLib
{
	//Material���p�����ăp�����[�^�𑝂₹��悤�ɂ���
	//�p�����Ă��ǂ�����ăV�F�[�_�[�ɓn��?���p�҂�Map���Ă��炤�����Ȃ�?
	//�萔�o�b�t�@�����R�ɍ���悤�ɂ����ق�������?

	//�}�e���A�������܂Ƃ߂��N���X�B
	struct Material
	{
		template<class T>
		using ComPtr = Microsoft::WRL::ComPtr<T>;

		//����
		Value3<float>ambient = 0.2f;
		//�g�U���ˌ�
		Value3<float>diffuse = 0.7f;
		//���ʔ��ˌ�
		Value3<float>specular = 0.0f;
		//�A���t�@�l
		float alpha = 1.0f;

	private:
		//�e�N�X�`��
		std::vector<Texture*>pTextures;
		ComPtr<ID3D12DescriptorHeap>textureHeap;
	};
}
