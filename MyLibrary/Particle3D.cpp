#include "Particle3D.h"



MelLib::Particle3D::Particle3D(const Color& color)
{
	particleSprite.Create(color);
}

MelLib::Particle3D::Particle3D(Texture* pTexture)
{
	particleSprite.Create(pTexture);
}

MelLib::Particle3D::Particle3D(const Particle3D& particle)
{
	particleSprite = particle.particleSprite;
}

void MelLib::Particle3D::Initialize()
{
}

void MelLib::Particle3D::Update()
{
}

void MelLib::Particle3D::Draw()
{
	particleSprite.Draw();
}
