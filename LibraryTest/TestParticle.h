#pragma once
#include <Particle3D.h>
#include<FrameTimer.h>

// �p�[�e�B�N����
class TestParticle :
    public MelLib::Particle3D
{
private:
    MelLib::FrameTimer timer;

    MelLib::Vector3 moveVec;
public:
    TestParticle();
    void Initialize()override;
    void Update()override;
    std::shared_ptr<Particle3D>GetNewPtr()const;

};

