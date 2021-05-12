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

	void MatrixMap(Texture* texture);
public:
	Sprite2D();
	~Sprite2D();

 	static bool Initialize(const int winWidth,const int winHeight);
	
	void CreateSprite();
	void Draw(Texture* texture)override;
	/// <summary>
	/// 表示範囲を指定して描画します。
	/// </summary>
	/// <param name="leftUpPos">左上座標</param>
	/// <param name="rightDownPos">右下座標</param>
	/// <param name="texture"></param>
	void SelectDrawAreaDraw(const Vector2& leftUpPos, const Vector2& rightDownPos, Texture* texture);

#pragma region 操作
	void SetPosition(const Vector2& pos) { constData.position = { pos.x,pos.y,0 }; }
	void SetAngle(const float& angle) { constData.angle = { 0,0,angle }; }
	void SetScale(const Vector2& scale) { constData.scale = scale.ToXMFLOAT2(); }
#pragma endregion


	static PipelineState GetDefaultPipeline() { return defaultPipeline; }
};

