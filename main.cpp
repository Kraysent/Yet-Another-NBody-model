#include <iostream>
#include <cstdint>
#include "Field.h"
#include "Random.h"
#include "Display.h"
#include "DisplayConfig.h"
#include "ParticleFactory.h"
#include "FieldConfig.h"
using namespace std;

Field* CreateRandomField();
Field* CreateGridField();
Field* CreateEmptyField();

int main()
{
    DisplayConfig dispConfig;
    dispConfig.WindowWidth = 700;
    dispConfig.WindowHeight = 700;
    dispConfig.IsGridEnabled = false;

    Field *field = CreateGridField();

    Display *display = new Display(field, dispConfig);

    display->Start();

    return 0;
}

Field* CreateEmptyField()
{
    FieldConfig conf;
    conf.Width = 500;
    conf.Height = 500;
    conf.HorizontalDivision = 40;
    conf.VerticalDivision = 40;
    conf.dt = 0.005;
    conf.WallElasticity = 0.5;

    Field* field = new Field(conf);

    return field;
}

Field* CreateGridField()
{
    FieldConfig conf;
    conf.Width = 1000;
    conf.Height = 1000;
    conf.HorizontalDivision = 25;
    conf.VerticalDivision = 40;
    conf.dt = 0.005;
    conf.WallElasticity = 0.5;
    conf.GravityStrength = 10;

    Field* field = new Field(conf);
    ParticleFactory* factory = new ParticleFactory(); 

    for (double x = 0; x < conf.Width; x += conf.Width / 70)
    {
        for (double y = conf.Height / 5; y < conf.Height; y += conf.Height / 70)
        {
            Vector2 pos = Vector2(x, y);
            Particle* p = factory->CreateStandartParticle(pos);

            field->InsertParticle(*p);
        }
    }

    return field;
}

Field* CreateRandomField()
{
    FieldConfig conf;
    conf.Width = 500;
    conf.Height = 500;
    conf.HorizontalDivision = 10;
    conf.VerticalDivision = 10;
    conf.dt = 0.005;
    conf.WallElasticity = 0.5;

    uint32_t N = 100;
    Random rnd;
    Field* field = new Field(conf);
    ParticleFactory* factory = new ParticleFactory(); 

    for (int i = 0; i < N; i++)
    {
        Vector2 pos = Vector2(rnd.GenerateRandomDouble(0, conf.Width),
                              rnd.GenerateRandomDouble(0, conf.Height));
        Particle* p = factory->CreateStandartParticle(pos);

        field->InsertParticle(*p);
    }

    return field;
}