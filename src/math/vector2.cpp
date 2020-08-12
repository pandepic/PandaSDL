#include "vector2.h"

PandaSDL::Vector2::Vector2()
: X(0), Y(0)
{
}

PandaSDL::Vector2::Vector2(double x, double y)
{
    X = x;
    Y = y;
}

PandaSDL::Vector2::Vector2(int x, int y)
{
    X = (double)x;
    Y = (double)y;
}

PandaSDL::Vector2::Vector2(unsigned int x, unsigned int y)
{
    X = (double)x;
    Y = (double)y;
}

PandaSDL::Vector2::Vector2(float f)
{
    X = (double)f;
    Y = (double)f;
}

PandaSDL::Vector2::~Vector2()
{
}

bool PandaSDL::Vector2::IsZero()
{
    if (X == 0 && Y == 0)
        return true;
    
    return false;
}

std::string PandaSDL::Vector2::ToString()
{
    return "[" + std::to_string(X) + ":" + std::to_string(Y) + "]";
}

PandaSDL::Vector2 PandaSDL::Vector2::operator+(const Vector2 &v)
{
    return PandaSDL::Vector2(X + v.X, Y + v.Y);
}

PandaSDL::Vector2 PandaSDL::Vector2::operator-(const Vector2 &v)
{
    return PandaSDL::Vector2(X - v.X, Y - v.Y);
}

PandaSDL::Vector2 PandaSDL::Vector2::operator*(const Vector2 &v)
{
    return PandaSDL::Vector2(X * v.X, Y * v.Y);
}

PandaSDL::Vector2 PandaSDL::Vector2::operator/(const Vector2 &v)
{
    return PandaSDL::Vector2(X / v.X, Y / v.Y);
}

PandaSDL::Vector2 PandaSDL::Vector2::operator*(const int &i)
{
    return PandaSDL::Vector2(X * i, Y * i);
}

PandaSDL::Vector2 PandaSDL::Vector2::operator*(const float &f)
{
    return PandaSDL::Vector2(X * f, Y * f);
}

PandaSDL::Vector2 PandaSDL::Vector2::operator*(const double &d)
{
    return PandaSDL::Vector2(X * d, Y * d);
}

PandaSDL::Vector2& PandaSDL::Vector2::operator+=(const PandaSDL::Vector2 &v)
{
    X += v.X;
    Y += v.Y;
    
    return *this;
}

PandaSDL::Vector2& PandaSDL::Vector2::operator-=(const PandaSDL::Vector2 &v)
{
    X -= v.X;
    Y -= v.Y;
    
    return *this;
}

PandaSDL::Vector2& PandaSDL::Vector2::operator*=(const PandaSDL::Vector2 &v)
{
    X *= v.X;
    Y *= v.Y;
    
    return *this;
}

PandaSDL::Vector2& PandaSDL::Vector2::operator/=(const PandaSDL::Vector2 &v)
{
    X /= v.X;
    Y /= v.Y;
    
    return *this;
}

PandaSDL::Vector2 PandaSDL::Vector2::Zero()
{
    return PandaSDL::Vector2(0, 0);
}
