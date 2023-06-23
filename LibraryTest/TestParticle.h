#pragma once
#include <Particle3D.h>
class TestParticle :
    public MelLib::Particle3D
{
public:
    TestParticle();
    void Initialize()override;
    void Update()override;
    std::shared_ptr<Particle3D>GetNewPtr()const;

};

