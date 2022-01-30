#pragma once

struct Vector2
{
public:
    Vector2();
    Vector2(double x, double y);
    void InverseX();
    void InverseY();
    void Rotate(double angle);
    void ScaleToMagnitude(double magnitude);
    Vector2 UnitVector();
    double Magnitude() const;

    double X;
    double Y;

    bool operator== (const Vector2& v) const;
    Vector2 operator+ (const Vector2& v) const;
    Vector2 operator- (const Vector2& v) const;
    Vector2 operator* (const double a) const;
    Vector2 operator/ (const double a) const;
    Vector2 operator+= (const Vector2& v);
    Vector2 operator-= (const Vector2& v);
    Vector2 operator*= (const double a);
    Vector2 operator/= (const double a);

    static Vector2 NullVector();
};