#include "TestParticle.h"



TestParticle::TestParticle() : Particle3D(MelLib::Color(255, 255))
{
}

void TestParticle::Initialize()
{
	particleSprite.SetBillboardFlag(true, true, true);
	particleSprite.SetScale(2);
}

void TestParticle::Update()
{
	particleSprite.SetPosition(particleSprite.GetPosition() + MelLib::Vector3(0.05f, 0, 0));

	if (particleSprite.GetPosition().x >= 2)isDead = true;
}

std::shared_ptr<MelLib::Particle3D> TestParticle::GetNewPtr() const
{
	return std::make_shared<TestParticle>();
}
