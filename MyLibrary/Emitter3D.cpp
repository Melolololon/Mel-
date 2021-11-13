#include "Emitter3D.h"

void MelLib::Emitter3D::CheckChangeUpdateDrawFlag()
{
	if (isStop)
	{
		return;
	}
	if (releaseTimer.GetMaxOverFlag())
	{
		for (auto& flag : particleUpdateDrawFlag)
		{
			if (!flag)
			{
				flag = true;
				releaseTimer.ResetTimeZero();
				return;
			}

		}
	}
}

void MelLib::Emitter3D::ParticleUpdate()
{


	for (int i = 0;i < particles.size(); i++)
	{
		if (!particleUpdateDrawFlag[i])return;

		particles[i]->Update();

		if (particles[i]->GetIsDead())
		{
			particles[i]->ResetParametor();
			particles[i]->Initialize();

			if (isStop)
			{
				particleUpdateDrawFlag[i] = false;
			}
		}
		
	}

}

MelLib::Emitter3D::Emitter3D(const std::vector<std::shared_ptr<Particle3D>>& pParticle, const Vector3& pos, unsigned int releaseTime)
{
	// pParticleを元にパーティクルを生成
	//particles.resize(particleNum, pParticle);

	particles = pParticle;
	particleUpdateDrawFlag.resize(particles.size(), false);
	releaseTimer.SetMaxTime(releaseTime);

	releaseTimer.SetStopFlag(false);

}

void MelLib::Emitter3D::Update()
{
	CheckChangeUpdateDrawFlag();
	ParticleUpdate();
}

void MelLib::Emitter3D::Draw()
{
	for (int i = 0;i < particles.size(); i++)
	{
		if (!particleUpdateDrawFlag[i])return;
		particles[i]->Draw();
	}
}
