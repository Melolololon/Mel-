#pragma once
#include"Sprite3D.h"
#include"FrameTimer.h"

// 使い方
// 自分でパーティクルクラスを作る
// 作ったパーティクルクラスに継承する
// 自分の思い通りに更新処理を書く
// コンストラクタに情報をセットする

namespace MelLib
{
	// スプライト3Dを利用したパーティクルを作るときに継承する基底クラス
	class Particle3D
	{
	protected:
		Sprite3D particleSprite;

		Vector3 startPosition;
		bool isDead = false;

	protected:
		/// <summary>
		/// パーティクルが消える(初期位置に戻す)かどうかのフラグをtrueにします。
		/// </summary>
		void TrueIsDeadFlag() { isDead = true; }

	public:
		Particle3D() {}
		Particle3D(const Color& color);
		Particle3D(Texture* pTexture);

		/// <summary>
		/// エミッターにセットする用のコンストラクタ
		/// </summary>
		/// <param name="particle"></param>
		Particle3D(const Particle3D& particle);

		/// <summary>
		/// エミッターにセットする用のコンストラクタ
		/// </summary>
		/// <param name="particle"></param>
		Particle3D(const Particle3D& particle, const Vector3& pos);
		virtual ~Particle3D() {}
		
		virtual void Initialize();
		virtual void Update();
		void Draw();
		virtual std::shared_ptr<Particle3D>GetNewPtr()const { return nullptr; }


		/// <summary>
		/// 寿命が来たときに戻す関数
		/// </summary>
		void ResetParametor() 
		{
			particleSprite.SetPosition(startPosition); 
			isDead = false;
		}

		bool GetIsDead() { return isDead; }
		
	};
}
