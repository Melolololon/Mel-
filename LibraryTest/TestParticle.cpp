#include "TestParticle.h"

#include<Random.h>

// Particle3D�̈����ŐF�܂��̓e�N�X�`�����w��
TestParticle::TestParticle() : Particle3D(MelLib::Color(255, 255))
{
}

void TestParticle::Initialize()
{
	particleSprite.SetBillboardFlag(true, true, true);
	particleSprite.SetScale(0.2f);

	timer.ResetTimeZero();
	timer.SetMaxTime(60 * 0.5);
	timer.SetStartFlag(true);

	moveVec = MelLib::Random::GetRandomVector3NumberRangeSelect(-1.0f, 1.0f, 1);
	moveVec = moveVec.Normalize();
}

void TestParticle::Update()
{
	// �����ɍX�V������ǉ�
	particleSprite.SetPosition(particleSprite.GetPosition() + moveVec * 0.2f);

	// ���Ԃŏ���
	if (timer.GetMaxOverFlag())isDead = true;
}

std::shared_ptr<MelLib::Particle3D> TestParticle::GetNewPtr() const
{
	return std::make_shared<TestParticle>();
}
