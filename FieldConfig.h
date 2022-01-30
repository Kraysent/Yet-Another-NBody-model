#pragma once

#include <cstdint>

struct FieldConfig
{
public:
    double Height = 500;
    double Width = 500;
    uint32_t HorizontalDivision = 30;
    uint32_t VerticalDivision = 30;
    double dt = 0.005;
    double WallElasticity = 0.5;
    int8_t GravityMode = 0;
    double GravityStrength = 10;
    uint32_t ApproximationWidth = 1;
    double AttractionStrength = 10;
    double RepulsionStrength = 1;
    double MaxForce = 1e4;
};