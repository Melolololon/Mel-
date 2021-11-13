#pragma once
#include<Particle3D.h>
class TestParticle :
    public MelLib::Particle3D
{
private:
    MelLib::Vector3 moveVector;
    int t = 0;
public:
    TestParticle();
    ~TestParticle(){}
    void Initialize()override;
    void Update()override;
};

