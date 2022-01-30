#pragma once

#include <random>
#include <cstdint>

class Random
{
public:
    Random(uint32_t seed = 62626);
    double GenerateRandomDouble(double min, double max);

private:
    std::uniform_real_distribution<double> _realEngine;
    std::default_random_engine _intEngine;
};