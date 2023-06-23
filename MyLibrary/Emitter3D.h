#pragma once
#include"GameObject.h"
#include"Particle3D.h"

// 使い方
// パーティクルクラスを作成する
// コンストラクタにパーティクルと設定を渡す
// GameObjectManagerに突っ込む

namespace MelLib
{
    class Emitter3D :public GameObject
	{
    private:
        //std::vector<std::shared_ptr<Particle3D>>particles;
        std::vector<std::shared_ptr<Particle3D>>particles;

        // Update、Drawを行うかどうかのフラグ
        std::vector<bool>particleUpdateDrawFlag;
        
        // 放出間隔
        FrameTimer releaseTimer;
        // 放出停止フラグ
        bool isStop = false;

        
    private:

        void CheckChangeUpdateDrawFlag();

        /// <summary>
        /// パーティクル更新処理
        /// </summary>
        void ParticleUpdate();

    public:
       // Emitter3D(const std::vector<std::shared_ptr<Particle3D>>& pParticle, const Vector3& pos, unsigned int releaseTime);
        
        /// <summary>
        /// 
        /// </summary>
        /// <param name="pParticle">パーティクルのインスタンス</param>
        /// <param name="particleNum">生成数</param>
        /// <param name="releaseTime">消えるまでの時間</param>
        /// <param name="pos">初期位置</param>
        /// <param name="name">ウィンドウの名前</param>
        Emitter3D(const Particle3D& pParticle, unsigned int particleNum, unsigned int releaseTime, const Vector3& pos,const std::string& name);
        ~Emitter3D() {}

        void Update()override;
        void Draw()override;

        void SetIsStop(const bool flag) 
        {
            isStop = flag; 
            releaseTimer.SetStopFlag(flag);
        }
        
       /* void SetPosition(const Vector3& pos) { SetPosition(pos); }

        Vector3 GetPosition()const { return GetPosition(); }*/
	};
}

