#include "Particle.h"
using namespace std;

Particle::Particle(double mass, Vector2 initialPos, Vector2 initialVel, 
                  double exclusionDistance, double potentialMin, ParticleType type)
    : Mass(mass), Position(initialPos), Velocity(initialVel), 
    ExclusionDistance(exclusionDistance), PotentialMin(potentialMin), Type(type)
{ }

void Particle::AddForce(const Vector2& force)
{
    _force = _force + force;
}

void Particle::Move(double dt)
{
    Velocity = Velocity + _force / Mass * dt;
    Position = Position + Velocity * dt;
    _force = Vector2::NullVector();
}

bool Particle::operator==(const Particle& p) const
{
    return (Mass == p.Mass) 
        && (Position == p.Position) 
        && (Velocity == p.Velocity);
}