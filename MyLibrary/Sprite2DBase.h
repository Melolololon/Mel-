#pragma once
#include"Sprite.h"
#include"PipelineState.h"

namespace MelLib
{
	//�����_�[�^�[�Q�b�g�𕪗������������
	//�X�v���C�g2D���N���X
	class Sprite2DBase :public Sprite
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


	protected:
		static PipelineState defaultPipeline;


		void MatrixMap(Texture* texture);

		//void Create(const Color& color)override;
		//void Create(Texture* pTexture)override;

		//��̊֐���Sprite2D�Ɉړ�
		//�V���������֐�(Sprite2D�ART�Ɏg����֐�)
		void SpriteInitialize();
	public:
		Sprite2DBase() {}
		//Sprite2DBase(const Color& color);
		//Sprite2DBase(Texture* pTexture);
		virtual ~Sprite2DBase() {}


		static bool Initialize(const int winWidth, const int winHeight);


		//void Draw(const std::string& rtName = "")override;



#pragma region ����
		void SetPosition(const Vector2& pos) { constData.position = { pos.x,pos.y,0 }; }
		void SetAngle(const float& angle) { constData.angle = { 0,0,angle }; }
		void SetScale(const Vector2& scale) { constData.scale = scale.ToXMFLOAT2(); }
#pragma endregion

#pragma region �Q�b�g
		Vector2 GetPosition()const { return Vector2(constData.position.x, constData.position.y); }
#pragma endregion



		static PipelineState GetDefaultPipeline() { return defaultPipeline; }
	};
}
