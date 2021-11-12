#include "Emitter3D.h"

void MelLib::Emitter3D::CheckChangeUpdateDrawFlag()
{
	if (!isStop)return;

	if (releaseTimer.GetMaxOverFlag()) 
	{
		for (auto& flag : particleUpdateDrawFlag)
		{
			if(!flag)
			{
				flag = true;
				return;
			}

		}
	}
}

void MelLib::Emitter3D::ParticleUpdate()
{
	for (int i = 0; particles.size(); i++)
	{
		if (!particleUpdateDrawFlag[i])return;

		particles[i].Update();

		if (particles[i].GetIsDead())
		{
			particles[i].ResetParametor();
			particles[i].Initialize();

			if (isStop)
			{
				particleUpdateDrawFlag[i] = false;
			}
		}
	}
}

MelLib::Emitter3D::Emitter3D(const Particle3D& pParticle, unsigned int particleNum,unsigned int releaseTime)
{
	// pParticleを元にパーティクルを生成
	particles.resize(particleNum, pParticle);
	particleUpdateDrawFlag.resize(particleNum, false);
	releaseTimer.SetMaxTime(releaseTime);
}

void MelLib::Emitter3D::Update()
{
	CheckChangeUpdateDrawFlag();
	ParticleUpdate();
}

void MelLib::Emitter3D::Draw()
{
	for (int i = 0; particles.size(); i++)
	{
		if (!particleUpdateDrawFlag[i])return;
		particles[i].Draw();
	}
}
