#pragma once
#include<Particle3D.h>
class TestParticle :
    public MelLib::Particle3D
{
private:
    MelLib::Vector3 moveVector;
public:
    TestParticle();
    ~TestParticle(){}
    void Initialize()override;
    void Update()override;
};

