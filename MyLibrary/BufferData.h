#pragma once
struct ConstBufferData
{
	enum BufferType
	{
		BUFFER_TYPE_NONE,//�������Ȃ�
		BUFFER_TYPE_COMMON,//�S���f������
		BUFFER_TYPE_EACH_MODEL,//�e���f������
		BUFFER_TYPE_EACH_MODEL_OBJECT,//�e���f���e�I�u�W�F�N�g����
	};

	BufferType bufferType;//�o�b�t�@�̐�
	size_t bufferDataSize;//�o�b�t�@1�̃T�C�Y
};