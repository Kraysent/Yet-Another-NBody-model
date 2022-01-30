#include "Random.h"
using namespace std;

Random::Random(uint32_t seed)
{
    srand(time(NULL));
    _intEngine = default_random_engine(rand() * 5);
}

double Random::GenerateRandomDouble(double min, double max)
{
    _realEngine = uniform_real_distribution<double>(min, max);

    return _realEngine(_intEngine);
}