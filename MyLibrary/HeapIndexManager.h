#pragma once
#include<vector>
#include<string>
class HeapIndexManager
{
private:
	int createNum;

	//�ǂ̔ԍ��̃��f�����`�悳��Ă邩(�c���Ă邩)���m�F���邽�߂̔z��
	std::vector<bool>drawCheckVector;
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="modelNum">��ʂɕ\�����郂�f���̍ő吔</param>
	HeapIndexManager(const int maxModelNum);
	~HeapIndexManager();

	/// <summary>
	/// �q�[�v�̃C���f�b�N�X���擾���܂��B
	/// </summary>
	/// <returns></returns>
	int GetHeapIndex();

	/// <summary>
	/// ���f���̕`�悪�I������Ƃ��A(�N���X�̃f�X�g���N�^�Ȃ�)�ŕK���Ăяo���Ă��������B
	/// </summary>
	/// <param name="heapIndex"></param>
	void DrawEndCallFunction(const int heapIndex);

};

