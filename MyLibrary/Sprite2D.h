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
	static std::unordered_map<std::string, std::unique_ptr<Sprite2D>> pSprite2D;

	//2D�̃J�����͌Œ肾����A�s�񐶐��֐����̃N���X�Ɏ������Ă���������
	static DirectX::XMMATRIX cameraMatrix;

	void Create(const Color& color)override;
	void Create(Texture* pTexture)override;
protected:
	static PipelineState defaultPipeline;


	void MatrixMap(Texture* texture);

public:
	Sprite2D(){}
	Sprite2D(const Color& color);
	Sprite2D(Texture* pTexture);
	virtual ~Sprite2D();


 	static bool Initialize(const int winWidth,const int winHeight);
	
	static void Create(const Color& color, const std::string& name);
	static void Create(Texture* pTexture, const std::string& name);
	static void Delete(const std::string& name);
	static Sprite2D* Get(const std::string& name ) { return pSprite2D[name].get(); }
	
	
	//�����_�[�^�[�Q�b�g��Draw���g�����߁A���z�֐��ɂ��Ă�
	 void Draw(const std::string& rtName = "")override;
	
	

#pragma region ����
	void SetPosition(const Vector2& pos) { constData.position = { pos.x,pos.y,0 }; }
	void SetAngle(const float& angle) { constData.angle = { 0,0,angle }; }
	void SetScale(const Vector2& scale) { constData.scale = scale.ToXMFLOAT2(); }
#pragma endregion


	static PipelineState GetDefaultPipeline() { return defaultPipeline; }
};

