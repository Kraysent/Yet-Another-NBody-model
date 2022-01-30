#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>
#include <array>
#include <vector>
#include <cstdint>
#include "Field.h"
#include "Particle.h"
#include "Vector2.h"
#include "ParticleFactory.h"
#include "DisplayConfig.h"

template<uint8_t N> struct Button;

class Display
{
public: 
    Display(Field* field, DisplayConfig config);
    void Start();
    ~Display();

private:
    void DrawParticles();
    void MakeScreenshot();
    void DrawText(const std::string& text, int32_t x, int32_t y);
    void DrawButtons();
    void DrawGrid();
    void DrawSelection();
    void PollEvents();
    void On_MouseUp(Sint32 x, Sint32 y);
    void On_KeyDown(SDL_Keycode code);

private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    ParticleFactory* _factory;
    std::vector<SDL_Texture*> _particleTextures;
    SDL_Texture* _selectionTexture;
    Button<3>* _NButton;
    Button<3>* _GrButton;
    Button<3>* _TButton;
    Button<2>* _GButton;
    Button<2>* _PButton;
    Field* _field;
    TTF_Font* _textFont;
    bool _active;
    bool _screenshotActive;
    bool _selectionStarted;
    double _xScale, _yScale;
    SDL_Point _mousePosition;
    double _selectionRadius;
    double _time;
    DisplayConfig _config;
    
};

template<uint8_t N>
struct Button
{
public:
    Button(std::array<SDL_Texture*, N> sprites, int32_t x, int32_t y, int8_t defaultValue = 1)
        : _sprites(sprites), X(x), Y(y)
    {
        _state = defaultValue % N;
        SDL_QueryTexture(_sprites[0], nullptr, nullptr, &_w, &_h);
    }

    ~Button()
    {
        for (SDL_Texture* tx : _sprites)
        {
            SDL_DestroyTexture(tx);
        }
    }

    SDL_Texture* GetSprite() const
    {
        return _sprites[_state];
    }

    int32_t GetWidth() const
    {
        return _w;
    }

    int32_t GetHeight() const
    {
        return _h;
    }

    uint8_t GetState() const
    {
        return _state;
    } 

    void UpdateState()
    {
        _state = (_state + 1) % N;
    }

    bool IsHovered(int32_t mx, int32_t my) const
    {
        if ((mx < X + GetWidth()) && (mx > X) &&
            (my < Y + GetHeight()) && (my > Y))
        {
            return true;
        }

        return false;
    }

    int32_t X, Y;

private:
    std::array<SDL_Texture*, N> _sprites;
    uint8_t _state;
    int32_t _w, _h;
};
