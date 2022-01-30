#include "Vector2.h"
#include <cmath>
using namespace std;

Vector2::Vector2() 
    : X(0), Y(0)
{}

Vector2::Vector2(double x, double y) 
    : X(x), Y(y)
{ }

void Vector2::InverseX()
{
    X = -X;
}

void Vector2::InverseY()
{
    Y = -Y;
}

void Vector2::Rotate(double angle)
{
    angle = angle * 180 / M_PI;
    double oldX = X;
    double oldY = Y;

    X = oldX * cos(angle) - oldY * sin(angle);
    Y = oldX * sin(angle) + oldY * cos(angle);
}

Vector2 Vector2::UnitVector()
{
    return (X == 0 && Y == 0) ? NullVector() : Vector2(X / Magnitude(), Y / Magnitude());
}

void Vector2::ScaleToMagnitude(double magnitude)
{
    Vector2 unit = UnitVector();

    X = unit.X * magnitude;
    Y = unit.Y * magnitude;
}

double Vector2::Magnitude() const
{
    return sqrt(X * X + Y * Y);
}

Vector2 Vector2::operator+(const Vector2& v) const
{
    Vector2 res(X + v.X, Y + v.Y);

    return res;
}

Vector2 Vector2::operator-(const Vector2& v) const
{
    Vector2 res(X - v.X, Y - v.Y);

    return res;
}

Vector2 Vector2::operator*(double a) const
{
    Vector2 res(X * a, Y * a);

    return res;
}

Vector2 Vector2::operator/(double a) const
{
    Vector2 res(X / a, Y / a);

    return res;
}

Vector2 Vector2::operator+=(const Vector2& v)
{
    X += v.X;
    Y += v.Y;

    return *this;
}

Vector2 Vector2::operator-=(const Vector2& v)
{
    X -= v.X;
    Y -= v.Y;

    return *this;
}

Vector2 Vector2::operator*=(double a)
{
    X *= a;
    Y *= a;

    return *this;
}

Vector2 Vector2::operator/=(double a)
{
    X /= a;
    Y /= a;

    return *this;
}

bool Vector2::operator==(const Vector2& v) const
{
    return (v.X == X) && (v.Y == Y);
}

Vector2 Vector2::NullVector()
{
    return Vector2(0, 0);
}
