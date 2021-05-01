#pragma once
#include"Sprite.h"
#include"PipelineState.h"

class Sprite2D :public Sprite
{
public:
	//�֐��őI�ׂ�悤�ɂ���
	enum SpriteAnchorPoint
	{
		SPRITE_ANCHOR_POINT_LEFT_UP,
		SPRITE_ANCHOR_POINT_LEFT_DOWN,
		SPRITE_ANCHOR_POINT_RIGHT_UP,
		SPRITE_ANCHOR_POINT_RIGHT_DOWN,
		SPRITE_ANCHOR_POINT_CENTER,
	};

private:
	//2D�̃J�����͌Œ肾����A�s�񐶐��֐����̃N���X�Ɏ������Ă���������
	static DirectX::XMMATRIX cameraMatrix;

	static PipelineState defaultPipeline;
public:
	Sprite2D();
	~Sprite2D();

 	static bool Initialize(const int winWidth,const int winHeight);
	

	void CreateSprite();


	void Draw(Texture* texture)override;

	static PipelineState GetDefaultPipeline() { return defaultPipeline; }
};

