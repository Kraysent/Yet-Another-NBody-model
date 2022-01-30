#pragma once

#include "Cell.h"
#include "FieldConfig.h"
#include <vector>
#include <cstdint>
#include <cmath>
#include <stdexcept>

class Field
{
public:
    Field(FieldConfig config);
    void Advance();
    void InsertParticle(const Particle& p);
    void ChangeVelocity(double factor);
    void DeleteParticlesWithinRaduis(double x, double y, double radius);
    void ChangeVelocityWithinRadius(double x, double y, double radius, double factor);
    void UpdateGravityMode();
    uint32_t NumberOfParticlesAt(uint32_t x, uint32_t y) const;
    uint32_t NumberOfParticles() const;
    double GetTemperature() const;
    FieldConfig GetConfig() const;
    Vector2 ComputeForce(const Particle& p1, const Particle& p2) const;

    std::vector<std::vector<Cell>>::const_iterator begin() const;
    std::vector<std::vector<Cell>>::const_iterator end() const;

private:
    void updateField();
    void reflectParticles();

private:
    std::vector<std::vector<Cell>> _cells;
    FieldConfig _config;
    double _wallElasticity;
};