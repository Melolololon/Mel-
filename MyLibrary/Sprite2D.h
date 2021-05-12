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

	void MatrixMap(Texture* texture);
public:
	Sprite2D();
	~Sprite2D();

 	static bool Initialize(const int winWidth,const int winHeight);
	
	void CreateSprite();
	void Draw(Texture* texture)override;
	/// <summary>
	/// �\���͈͂��w�肵�ĕ`�悵�܂��B
	/// </summary>
	/// <param name="leftUpPos">������W</param>
	/// <param name="rightDownPos">�E�����W</param>
	/// <param name="texture"></param>
	void SelectDrawAreaDraw(const Vector2& leftUpPos, const Vector2& rightDownPos, Texture* texture);

#pragma region ����
	void SetPosition(const Vector2& pos) { constData.position = { pos.x,pos.y,0 }; }
	void SetAngle(const float& angle) { constData.angle = { 0,0,angle }; }
	void SetScale(const Vector2& scale) { constData.scale = scale.ToXMFLOAT2(); }
#pragma endregion


	static PipelineState GetDefaultPipeline() { return defaultPipeline; }
};

