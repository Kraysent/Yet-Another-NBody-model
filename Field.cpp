#include "Field.h"
#include <iostream>
#include <algorithm>
using namespace std;

Field::Field(FieldConfig config)
{
    _cells = vector<vector<Cell>>(config.VerticalDivision);

    for (int i = 0; i < config.VerticalDivision; i++)
    {
        _cells[i] = vector<Cell>(config.HorizontalDivision);
    }

    _config = config;
}

void Field::Advance()
{
    updateField(); 

    for (int i = 0; i < _config.HorizontalDivision; i++)
    {
        for (int j = 0; j < _config.VerticalDivision; j++)
        {
            vector<Particle>& currParticles = _cells[j][i].Particles;

            for (int k = 0; k < currParticles.size(); k++)
            {
                Vector2 force = Vector2::NullVector();

                for (int p1 = 0; p1 < currParticles.size(); p1++)
                {
                    if (p1 != k)
                    {
                        force += ComputeForce(currParticles[k], currParticles[p1]);
                    }
                }

                uint32_t approx = _config.ApproximationWidth;
                int mini = max((int)(i-approx), 0);
                int maxi = min(i+approx, _config.HorizontalDivision - 1);
                int minj = max((int)(j-approx), 0);
                int maxj = min(j+approx, _config.VerticalDivision - 1);

                Vector2 res;
                
                for (int i1 = mini; i1 <= maxi; i1++)
                {
                    for (int j1 = minj; j1 <= maxj; j1++)
                    {
                        if ((i1 == i) && (j1 == j))
                        {
                            continue;
                        }
                        else
                        {
                            // compute forces from neighbouring cells

                            res = Vector2::NullVector();

                            for (const Particle& curr : _cells[j1][i1].Particles)
                            {
                                res += ComputeForce(currParticles[k], curr);
                            }

                            force += res;
                        }
                    }
                }

                force += Vector2(0, currParticles[k].Mass * 10 * _config.GravityStrength) * _config.GravityMode;
                              
                if (force.Magnitude() >= _config.MaxForce * currParticles[k].Mass)
                {
                    force.ScaleToMagnitude(_config.MaxForce * currParticles[k].Mass);
                }

                currParticles[k].AddForce(force);
            }
        }
    }

    reflectParticles();
}

void Field::InsertParticle(const Particle& p)
{
    double xSizePerCell = _config.Width / _config.HorizontalDivision;
    double ySizePerCell = _config.Height / _config.VerticalDivision;
    double x = max(0, min((int)(p.Position.X / xSizePerCell), (int)_config.HorizontalDivision - 1));
    double y = max(0, min((int)(p.Position.Y / ySizePerCell), (int)_config.VerticalDivision - 1));

    _cells[y][x].Particles.push_back(p);
}

void Field::ChangeVelocity(double factor)
{
    for (int i = 0; i < _config.HorizontalDivision; i++)
    {
        for (int j = 0; j < _config.VerticalDivision; j++)
        {
            vector<Particle>& currParticles = _cells[j][i].Particles;
            for (int k = 0; k < currParticles.size(); k++)
            {
                currParticles[k].Velocity = currParticles[k].Velocity * factor; 
            }
        }
    }
}

void Field::DeleteParticlesWithinRaduis(double x, double y, double radius)
{
    Vector2 pos(x, y);

    for (int i = 0; i < _config.HorizontalDivision; i++)
    {
        for (int j = 0; j < _config.VerticalDivision; j++)
        {
            vector<Particle>& currParticles = _cells[j][i].Particles;
            
            currParticles.erase(remove_if(currParticles.begin(), currParticles.end(), 
                                [pos, radius](Particle p) { return ((pos - p.Position).Magnitude() < radius); }),
                                currParticles.end());
        }
    }
}

void Field::ChangeVelocityWithinRadius(double x, double y, double radius, double factor)
{
    Vector2 pos(x, y);

    for (int i = 0; i < _config.HorizontalDivision; i++)
    {
        for (int j = 0; j < _config.VerticalDivision; j++)
        {
            vector<Particle>& currParticles = _cells[j][i].Particles;
            
            for (Particle& p : currParticles)
            {
                if ((pos - p.Position).Magnitude() < radius)
                {
                    p.Velocity *= factor;
                }
            }
        }
    }
}

void Field::UpdateGravityMode()
{
    _config.GravityMode = (_config.GravityMode + 1 + 1) % 3 - 1;
}

void Field::updateField()
{
    for (int i = 0; i < _config.HorizontalDivision; i++)
    {
        for (int j = 0; j < _config.VerticalDivision; j++)
        {
            vector<Particle> current;
            current.swap(_cells[j][i].Particles);
            _cells[j][i].Particles.clear();

            for (Particle p : current)
            {
                InsertParticle(p);
            }
        }
    }
}

void Field::reflectParticles()
{
    for (int i = 0; i < _config.HorizontalDivision; i++)
    {
        for (int j = 0; j < _config.VerticalDivision; j++)
        {
            vector<Particle>& currParticles = _cells[j][i].Particles;
            for (int k = 0; k < currParticles.size(); k++)
            {
                if (currParticles[k].Position.X > _config.Width) 
                {
                    currParticles[k].Velocity.InverseX();
                    currParticles[k].Velocity.X *= _config.WallElasticity;
                    currParticles[k].Position.X = _config.Width;
                }
                if (currParticles[k].Position.X < 0) 
                {
                    currParticles[k].Velocity.InverseX();
                    currParticles[k].Velocity.X *= _config.WallElasticity;
                    currParticles[k].Position.X = 0;
                }
                if (currParticles[k].Position.Y > _config.Height) 
                {
                    currParticles[k].Velocity.InverseY();
                    currParticles[k].Velocity.Y *= _config.WallElasticity;
                    currParticles[k].Position.Y = _config.Height;
                }
                if (currParticles[k].Position.Y < 0) 
                {
                    currParticles[k].Velocity.InverseY();
                    currParticles[k].Velocity.Y *= _config.WallElasticity;
                    currParticles[k].Position.Y = 0;
                }

                currParticles[k].Move(_config.dt);
            }
        }
    }
}

uint32_t Field::NumberOfParticlesAt(uint32_t x, uint32_t y) const
{
    return _cells[y][x].Particles.size();
}

uint32_t Field::NumberOfParticles() const
{
    uint32_t res = 0;

    for (int i = 0; i < _config.HorizontalDivision; i++)
    {
        for (int j = 0; j < _config.VerticalDivision; j++)
        {
            res += NumberOfParticlesAt(i, j);
        }
    }

    return res;
}

double Field::GetTemperature() const
{
    // This is not the average but the sum of the kinetical energies
    // but it works fine so leave it like this
    double avgE = 0;

    for (auto x : _cells)
    {
        for (auto y : x)
        {
            for (const Particle& p : y.Particles)
            {
                avgE += p.Mass * pow(p.Velocity.Magnitude(), 2) / 2;
            }
        }
    }

    avgE /= NumberOfParticles();

    return 2 * avgE / (3 * 1.38e-23);
}

FieldConfig Field::GetConfig() const
{
    return _config;
}

vector<vector<Cell>>::const_iterator Field::begin() const
{
    return _cells.begin();
}

vector<vector<Cell>>::const_iterator Field::end() const
{
    return _cells.end();
}

Vector2 Field::ComputeForce(const Particle& p1, const Particle& p2) const
{
    Vector2 r = p2.Position - p1.Position;
    Vector2 attraction = r.UnitVector() * 24 * p1.PotentialMin * pow(p1.ExclusionDistance + p2.ExclusionDistance, 6) / pow(r.Magnitude(), 7);
    attraction *= _config.AttractionStrength;
    Vector2 repulsion = r.UnitVector() * 48 * p1.PotentialMin * pow(p1.ExclusionDistance + p2.ExclusionDistance, 12) / pow(r.Magnitude(), 13);
    repulsion *= _config.RepulsionStrength;

    return attraction - repulsion;
}
