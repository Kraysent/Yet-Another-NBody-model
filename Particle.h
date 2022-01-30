#pragma once

#include "Vector2.h"
#include <cstdint>
#include <cmath>

enum ParticleType
{
    Simple, 
    Repulsive
};

class Particle
{
public:
    Particle(double mass, Vector2 initialPos, Vector2 initialVel, double exclusionDistance, double potentialMin, ParticleType type);
    bool operator==(const Particle& p) const;
    void AddForce(const Vector2& force);
    void Move(double dt);

    Vector2 Velocity;
    Vector2 Position;
    double Mass;
    double ExclusionDistance;
    double PotentialMin;
    ParticleType Type;

private:
    Vector2 _force;
};

