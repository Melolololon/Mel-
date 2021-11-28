#include "TestParticle.h"
#include<Random.h>

TestParticle::TestParticle()
	:Particle3D(MelLib::Color(255, 0, 0, 255))
{
}

void TestParticle::Initialize()
{
	moveVector.x = MelLib::Random::GetRandomFloatNumber(2, 2) - 1;
	moveVector.y = MelLib::Random::GetRandomFloatNumber(2, 2) - 1;
	moveVector.z = MelLib::Random::GetRandomFloatNumber(2, 2) - 1;
	moveVector = moveVector.Normalize();
}

void TestParticle::Update()
{
	particleSprite.SetPosition(particleSprite.GetPosition() + moveVector * 0.05f);
	t++;
	if (t >= 200)
	{
		TrueIsDeadFlag();
		t = 0;
	}

}
