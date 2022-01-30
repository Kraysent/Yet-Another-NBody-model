#include "Display.h"
using namespace std;

Display::Display(Field* field, DisplayConfig config) 
    : _field(field), _config(config)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    Uint32 flags = SDL_WINDOW_SHOWN | (SDL_WINDOW_RESIZABLE * _config.IsResizable);

    SDL_CreateWindowAndRenderer(_config.WindowWidth, _config.WindowHeight, flags, &_window, &_renderer);
    SDL_SetWindowTitle(_window, _config.WindowTitle.c_str());

    _selectionTexture = SDL_CreateTextureFromSurface(_renderer, IMG_Load("Resources/pictures/selection_sprite.png"));
    _textFont = TTF_OpenFont("Resources/fonts/DroidSans.ttf", 16);
    _factory = new ParticleFactory();
    _particleTextures = vector<SDL_Texture*>();

    _particleTextures.push_back(SDL_CreateTextureFromSurface(_renderer, 
                                                             IMG_Load("Resources/pictures/simple_sprite.bmp")));
    _particleTextures.push_back(SDL_CreateTextureFromSurface(_renderer, 
                                                             IMG_Load("Resources/pictures/repulsive_sprite.bmp")));

    _NButton = new Button<3>({SDL_CreateTextureFromSurface(_renderer, IMG_Load("Resources/pictures/N_left_button.png")), 
                       SDL_CreateTextureFromSurface(_renderer, IMG_Load("Resources/pictures/N_center_button.png")),
                       SDL_CreateTextureFromSurface(_renderer, IMG_Load("Resources/pictures/N_right_button.png"))},
                       _config.WindowWidth - 100, 10, 1);
    _GrButton = new Button<3>({SDL_CreateTextureFromSurface(_renderer, IMG_Load("Resources/pictures/Gr_left_button.png")), 
                       SDL_CreateTextureFromSurface(_renderer, IMG_Load("Resources/pictures/Gr_center_button.png")),
                       SDL_CreateTextureFromSurface(_renderer, IMG_Load("Resources/pictures/Gr_right_button.png"))},
                       _config.WindowWidth - 100, 40, _field->GetConfig().GravityMode + 1);
    _TButton = new Button<3>({SDL_CreateTextureFromSurface(_renderer, IMG_Load("Resources/pictures/T_left_button.png")), 
                       SDL_CreateTextureFromSurface(_renderer, IMG_Load("Resources/pictures/T_center_button.png")),
                       SDL_CreateTextureFromSurface(_renderer, IMG_Load("Resources/pictures/T_right_button.png"))},
                       _config.WindowWidth - 100, 70);
    _GButton = new Button<2>({SDL_CreateTextureFromSurface(_renderer, IMG_Load("Resources/pictures/G_left_button.png")), 
                       SDL_CreateTextureFromSurface(_renderer, IMG_Load("Resources/pictures/G_right_button.png"))},
                       _config.WindowWidth - 100, 100, _config.IsGridEnabled);
    _PButton = new Button<2>({SDL_CreateTextureFromSurface(_renderer, IMG_Load("Resources/pictures/P_left_button.png")),
                              SDL_CreateTextureFromSurface(_renderer, IMG_Load("Resources/pictures/P_right_button.png"))},
                              _config.WindowWidth - 100, 130, _config.IsPaused + 1);

    _active = false;
    _selectionStarted = false;
    _screenshotActive = false;
    _selectionRadius = 50;
    _mousePosition = SDL_Point{0, 0};
    _time = 0;

    _xScale = _config.WindowWidth / (double)_field->GetConfig().Width;
    _yScale = _config.WindowHeight / (double)_field->GetConfig().Height;
}

void Display::Start()
{
    _active = true;
    uint32_t frameTime = 0;
    uint32_t frameStart = 0;
    uint32_t framesNumber = 0;
    uint32_t currFrameTime = 0;
    uint32_t frameDelay = 1000 / _config.MaxFPS;

    while (_active)
    {
        frameStart = SDL_GetTicks();

        PollEvents();
        SDL_SetRenderDrawColor(_renderer, _config.BackgroundColor.r, 
                                          _config.BackgroundColor.g, 
                                          _config.BackgroundColor.b, 
                                          _config.BackgroundColor.a);
        SDL_RenderClear(_renderer);
        
        if (_config.IsGridEnabled) DrawGrid();
        DrawParticles();
        if (_screenshotActive) MakeScreenshot();
        if (_selectionStarted) DrawSelection();
        DrawText("Temperature: " + to_string((_field->GetTemperature() * _config.TemperatureScale)), 10, 10);
        DrawText("FPS: " + to_string((int32_t)((double)framesNumber  / (frameTime) * 1000)), 10, 30);
        DrawText("Number of particles: " + to_string(_field->NumberOfParticles()), 10, 50);
        DrawText("Time: " + to_string(_time), 10, 70);
        DrawButtons();

        SDL_RenderPresent(_renderer);

        if (!_config.IsPaused)
        {
            _field->Advance();
            _time += _field->GetConfig().dt;

            framesNumber++;

            if (framesNumber % 1'000 == 0)
            {
                frameTime = 0;
                framesNumber = 0;
            }

            // Limit frame rate

            currFrameTime = (SDL_GetTicks() - frameStart);

            if (frameDelay > currFrameTime)
            {
                SDL_Delay(frameDelay - currFrameTime);
            }

            frameTime += (SDL_GetTicks() - frameStart);
        }
    }
}

void Display::DrawParticles()
{
    // Iterate over each and every particle and get its position

    for (const vector<Cell>& vc : *_field)
    {
        for (const Cell& c : vc)
        {
            for (const Particle& p : c.Particles)
            {
                Vector2 coords;
                coords.X = p.Position.X * _xScale;
                coords.Y = p.Position.Y * _yScale;
                SDL_Rect input, out;

                SDL_QueryTexture(_particleTextures[(uint8_t)p.Type], nullptr, nullptr, &input.w, &input.h);
                input.x = 0;
                input.y = 0;
                SDL_QueryTexture(_particleTextures[(uint8_t)p.Type], nullptr, nullptr, &out.w, &out.h);
                out.x = coords.X - out.w / 2;
                out.y = coords.Y - out.h / 2;

                SDL_RenderCopy(_renderer, _particleTextures[(uint8_t)p.Type], &input, &out);
            }
        }
    }
}

void Display::MakeScreenshot()
{
    int w, h;
    SDL_GetRendererOutputSize(_renderer, &w, &h);

    SDL_Surface* shot = SDL_CreateRGBSurface(0, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels(_renderer, NULL, SDL_PIXELFORMAT_ARGB8888, shot->pixels, shot->pitch);
    SDL_SaveBMP(shot, ("model_" + to_string(_time) + ".bmp").c_str());
    SDL_FreeSurface(shot);

    _screenshotActive = false;
}

void Display::DrawText(const string& text, int32_t x, int32_t y)
{
    SDL_Surface* textSurface = TTF_RenderText_Blended(_textFont, text.c_str(), _config.TextColor);
    SDL_Rect outRect{x, y, textSurface->w, textSurface->h};
    SDL_Texture* textTX = SDL_CreateTextureFromSurface(_renderer, textSurface);

    SDL_RenderCopy(_renderer, textTX, nullptr, &outRect);

    SDL_DestroyTexture(textTX);
}

void Display::DrawButtons()
{
    SDL_Rect outNRect{_NButton->X, _NButton->Y, _NButton->GetWidth(), _NButton->GetHeight()};
    SDL_RenderCopy(_renderer, _NButton->GetSprite(), nullptr, &outNRect);

    SDL_Rect outGrRect{_GrButton->X, _GrButton->Y, _GrButton->GetWidth(), _GrButton->GetHeight()};
    SDL_RenderCopy(_renderer, _GrButton->GetSprite(), nullptr, &outGrRect);

    SDL_Rect outTRect{_TButton->X, _TButton->Y, _TButton->GetWidth(), _TButton->GetHeight()};
    SDL_RenderCopy(_renderer, _TButton->GetSprite(), nullptr, &outTRect);

    SDL_Rect outGRect{_GButton->X, _GButton->Y, _GButton->GetWidth(), _GButton->GetHeight()};
    SDL_RenderCopy(_renderer, _GButton->GetSprite(), nullptr, &outGRect);

    SDL_Rect outPRect{_PButton->X, _PButton->Y, _PButton->GetWidth(), _PButton->GetHeight()};
    SDL_RenderCopy(_renderer, _PButton->GetSprite(), nullptr, &outPRect);
}

void Display::DrawGrid()
{
    int32_t stepx = (int32_t)(_config.WindowWidth / _field->GetConfig().HorizontalDivision) / _config.GridFrequency;
    int32_t stepy = (int32_t)(_config.WindowHeight / _field->GetConfig().VerticalDivision) / _config.GridFrequency;
    SDL_SetRenderDrawColor(_renderer, _config.GridColor.r, _config.GridColor.g, _config.GridColor.b, _config.GridColor.a);

    for (int32_t x = stepx; x < _config.WindowWidth; x += stepx)
    {
        SDL_RenderDrawLine(_renderer, x, 0, x, _config.WindowHeight);
    }
    for (int32_t y = stepy; y < _config.WindowHeight; y += stepy)
    {
        SDL_RenderDrawLine(_renderer, 0, y, _config.WindowWidth, y);
    }
}

void Display::DrawSelection()
{
    int N = 30;
    SDL_Rect input, out;

    SDL_QueryTexture(_selectionTexture, nullptr, nullptr, &input.w, &input.h);
    input.x = 0;
    input.y = 0;

    for (double angle = 0; angle < 2 * M_PI; angle += 2 * M_PI / N)
    {
        SDL_QueryTexture(_selectionTexture, nullptr, nullptr, &out.w, &out.h);
        out.x = _mousePosition.x + _selectionRadius * cos(angle) - out.w / 2;
        out.y = _mousePosition.y + _selectionRadius * _yScale / _xScale * sin(angle) - out.h / 2;
        SDL_RenderCopy(_renderer, _selectionTexture, &input, &out);
    }
}

void Display::PollEvents()
{
    SDL_Event currEvent;
        
    while (SDL_PollEvent(&currEvent))
    {
        switch (currEvent.type)
        {
            case SDL_QUIT:
                cerr << "Quitting program" << endl;
                _active = false;
                break;
            case SDL_MOUSEMOTION:
                _mousePosition = SDL_Point{currEvent.motion.x, currEvent.motion.y};
                break;
            case SDL_MOUSEBUTTONUP:
                On_MouseUp(currEvent.button.x, currEvent.button.y);
                break;
            case SDL_KEYDOWN:
                On_KeyDown(currEvent.key.keysym.sym);
                break;
            case SDL_MOUSEWHEEL:
                if (currEvent.wheel.y < 0) _selectionRadius = min(_selectionRadius + 5, 200.0);
                if (currEvent.wheel.y > 0) _selectionRadius = max(_selectionRadius - 5, 15.0);
                break;
            case SDL_WINDOWEVENT:
            {
                if (currEvent.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    if (currEvent.window.data1 < 350)
                    {
                        SDL_SetWindowSize(_window, 350, currEvent.window.data2);
                        _config.WindowWidth = 350;
                    }
                    else
                    {
                        _config.WindowWidth = currEvent.window.data1;
                    }
                    

                    if (currEvent.window.data2 < 350)
                    {
                        SDL_SetWindowSize(_window, currEvent.window.data1, 350);
                        _config.WindowHeight = 350;
                    }
                    else
                    {
                        _config.WindowHeight = currEvent.window.data2;
                    }

                    _xScale = _config.WindowWidth / _field->GetConfig().Width;                
                    _yScale = _config.WindowHeight / _field->GetConfig().Height;
                    _NButton->X = _config.WindowWidth - 100;
                    _GrButton->X = _config.WindowWidth - 100;
                    _TButton->X = _config.WindowWidth - 100;
                    _GButton->X = _config.WindowWidth - 100;
                    _PButton->X = _config.WindowWidth - 100;

                    if (currEvent.window.data1 < 350)
                    {
                        SDL_SetWindowSize(_window, 350, currEvent.window.data2);
                    }

                    if (currEvent.window.data2 < 350)
                    {
                        SDL_SetWindowSize(_window, currEvent.window.data1, 350);
                    } 
                }
                break;
            }
            default:
                //cerr << "Unhandled event" << endl;
                break;
        }
    }
}

void Display::On_MouseUp(int32_t x, int32_t y)
{
    if (_NButton->IsHovered(x, y))
    {
        _NButton->UpdateState();

        if (_NButton->GetState() == 0)
        {
            _selectionStarted = true;
        }    
        else
        {
            _selectionStarted = false;
        }
    }
    else if (_GButton->IsHovered(x, y))
    {
        _GButton->UpdateState();
        _config.IsGridEnabled = !_config.IsGridEnabled;
    }
    else if (_PButton->IsHovered(x, y))
    {
        _PButton->UpdateState();
        _config.IsPaused = !_config.IsPaused;
    }
    else if (_GrButton->IsHovered(x, y))
    {
        _GrButton->UpdateState();
        _field->UpdateGravityMode();
    }
    else if (_TButton->IsHovered(x, y))
    {
        _TButton->UpdateState();

        if (_TButton->GetState() != 1)
        {
            _selectionStarted = true;
        }
        else
        {
            _selectionStarted = false;
        }
    }
    else
    {
        if(_NButton->GetState() == 0)
        {
            _field->DeleteParticlesWithinRaduis(x / _xScale, y / _yScale, _selectionRadius / _xScale);
        }
        else if (_NButton->GetState() == 2)
        {
            Particle* p = _factory->CreateRepulsiveParticle(Vector2(x / _xScale, y / _yScale));
            
            _field->InsertParticle(*p);
        }

        if (_TButton->GetState() == 0)
        {
            _field->ChangeVelocityWithinRadius(x / _xScale, y / _yScale, _selectionRadius / _xScale, _config.VelocityClickFactor);
        }
        else if (_TButton->GetState() == 2)
        {
            _field->ChangeVelocityWithinRadius(x / _xScale, y / _yScale, _selectionRadius / _xScale, 1 /_config.VelocityClickFactor);
        }
    }
}

void Display::On_KeyDown(SDL_Keycode code)
{
    if (code == SDLK_c) // cool the gas
    {
        _field->ChangeVelocity(0.5);
    }
    else if (code == SDLK_f) // freeze (?) the gas
    {
        _field->ChangeVelocity(0.01);
    }
    else if (code == SDLK_h) // heat the gas
    {
        _field->ChangeVelocity(1.2);
    }
    else if (code == SDLK_s)
    {
        _screenshotActive = true;
    }
    
}

Display::~Display()
{
    for (SDL_Texture* tx : _particleTextures)
    {
        SDL_DestroyTexture(tx);
    }

    SDL_DestroyWindow(_window);
    SDL_DestroyRenderer(_renderer);
    SDL_Quit();
}
