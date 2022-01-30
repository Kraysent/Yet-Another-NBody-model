#include "ParticleFactory.h"
using namespace std;

ParticleFactory::ParticleFactory()
{
    _rnd = new Random();
}

Particle* ParticleFactory::CreateStandartParticle(Vector2 position)
{
    Vector2 vel = Vector2(_rnd->GenerateRandomDouble(-2, 2),
                          _rnd->GenerateRandomDouble(-2, 2));
    return new Particle(10, position, vel, 10, 1000, ParticleType::Simple);
}

Particle* ParticleFactory::CreateRepulsiveParticle(Vector2 position)
{
    double disp = 500;
    Vector2 vel = Vector2(_rnd->GenerateRandomDouble(-disp, disp),
                          _rnd->GenerateRandomDouble(-disp, disp));
    return new Particle(10, position, vel, 20, 1000, ParticleType::Repulsive);
}