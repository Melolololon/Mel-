#pragma once
#include<Particle3D.h>
class TestParticle :
    public MelLib::Particle3D
{
private:

public:
    TestParticle();
    ~TestParticle(){}
    void Initialize()override;
    void Update()override;
};

