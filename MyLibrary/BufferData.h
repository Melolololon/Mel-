#pragma once
struct ConstBufferData
{
	enum BufferType
	{
		BUFFER_TYPE_NONE,//生成しない
		BUFFER_TYPE_COMMON,//全モデル共通
		BUFFER_TYPE_EACH_MODEL,//各モデルごと
		BUFFER_TYPE_EACH_MODEL_OBJECT,//各モデル各オブジェクトごと
	};

	BufferType bufferType = BUFFER_TYPE_NONE;//バッファの数
	size_t bufferDataSize = 0;//バッファ1つのサイズ
};