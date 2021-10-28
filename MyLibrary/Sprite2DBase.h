#pragma once
#include"Sprite.h"
#include"PipelineState.h"

namespace MelLib
{
	//レンダーターゲットを分離したから消す
	//スプライト2D基底クラス
	class Sprite2DBase :public Sprite
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


	protected:
		static PipelineState defaultPipeline;


		void MatrixMap(Texture* texture);

		//void Create(const Color& color)override;
		//void Create(Texture* pTexture)override;

		//上の関数をSprite2Dに移動
		//新しい生成関数(Sprite2D、RTに使える関数)
		void SpriteInitialize();
	public:
		Sprite2DBase() {}
		//Sprite2DBase(const Color& color);
		//Sprite2DBase(Texture* pTexture);
		virtual ~Sprite2DBase() {}


		static bool Initialize(const int winWidth, const int winHeight);


		//void Draw(const std::string& rtName = "")override;



#pragma region 操作
		void SetPosition(const Vector2& pos) { constData.position = { pos.x,pos.y,0 }; }
		void SetAngle(const float& angle) { constData.angle = { 0,0,angle }; }
		void SetScale(const Vector2& scale) { constData.scale = scale.ToXMFLOAT2(); }
#pragma endregion

#pragma region ゲット
		Vector2 GetPosition()const { return Vector2(constData.position.x, constData.position.y); }
#pragma endregion



		static PipelineState GetDefaultPipeline() { return defaultPipeline; }
	};
}
