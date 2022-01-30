#pragma once

#include <cstdint>
#include <string>
#include <SDL2/SDL.h>

struct DisplayConfig
{
public: 
    uint32_t WindowWidth;
    uint32_t WindowHeight;
    uint32_t MaxFPS = 60;
    std::string WindowTitle;
    SDL_Color TextColor = {0, 0, 0};
    SDL_Color BackgroundColor = {192, 192, 192};
    SDL_Color GridColor = {150, 150, 150, 255}; 
    double GridFrequency = 1;
    double VelocityClickFactor = 0.5;
    double TemperatureScale = 1.67e-27;
    bool IsPaused = false;
    bool IsGridEnabled = true;
    bool IsResizable = true;
};