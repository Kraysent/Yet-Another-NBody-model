#pragma once
#include "Particle.h"
#include "Vector2.h"
#include "Random.h"
#include <SDL2/SDL.h>

class ParticleFactory
{
public:
    ParticleFactory();
    Particle* CreateStandartParticle(Vector2 position);
    Particle* CreateRepulsiveParticle(Vector2 position);

private:
    Random* _rnd;
};