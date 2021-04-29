#pragma once
#include"Sprite.h"
#include"PipelineState.h"

class Sprite2D :public Sprite
{
public:
	//関数で選べるようにする
	enum SpriteAnchorPoint
	{
		SPRITE_ANCHOR_POINT_LEFT_UP,
		SPRITE_ANCHOR_POINT_LEFT_DOWN,
		SPRITE_ANCHOR_POINT_RIGHT_UP,
		SPRITE_ANCHOR_POINT_RIGHT_DOWN,
		SPRITE_ANCHOR_POINT_CENTER,
	};

private:
	//2Dのカメラは固定だから、行列生成関数このクラスに持たせてもいいかも
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

