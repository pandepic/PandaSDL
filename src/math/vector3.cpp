#include "vector3.h"

PandaSDL::Vector3::Vector3()
: X(0), Y(0), Z(0)
{
}

PandaSDL::Vector3::~Vector3()
{
}

PandaSDL::Vector3::Vector3(double x, double y, double z)
    : X(x), Y(y), Z(z)
{
}

PandaSDL::Vector3::Vector3(int x, int y, int z)
    : X(x), Y(y), Z(z)
{
}

PandaSDL::Vector3::Vector3(unsigned int x, unsigned int y, unsigned int z)
    : X(x), Y(y), Z(z)
{
}

PandaSDL::Vector3::Vector3(float f)
    : X(f), Y(f), Z(f)
{
}

bool PandaSDL::Vector3::IsZero()
{
    if (X == 0 && Y == 0 && Z == 0)
        return true;
    
    return false;
}

std::string PandaSDL::Vector3::ToString()
{
    return "[" + std::to_string(X) + ":" + std::to_string(Y) + ":" + std::to_string(Z) + "]";
}

PandaSDL::Vector3 PandaSDL::Vector3::operator+(const PandaSDL::Vector3 &v)
{
    return PandaSDL::Vector3(X + v.X, Y + v.Y, Z + v.Z);
}

PandaSDL::Vector3 PandaSDL::Vector3::operator-(const PandaSDL::Vector3 &v)
{
    return PandaSDL::Vector3(X - v.X, Y - v.Y, Z - v.Z);
}

PandaSDL::Vector3 PandaSDL::Vector3::operator*(const PandaSDL::Vector3 &v)
{
    return PandaSDL::Vector3(X * v.X, Y * v.Y, Z * v.Z);
}

PandaSDL::Vector3 PandaSDL::Vector3::operator/(const PandaSDL::Vector3 &v)
{
    return PandaSDL::Vector3(X / v.X, Y / v.Y, Z / v.Z);
}

PandaSDL::Vector3 PandaSDL::Vector3::operator*(const int &i)
{
    return PandaSDL::Vector3(X * i, Y * i, Z * i);
}

PandaSDL::Vector3 PandaSDL::Vector3::operator*(const float &f)
{
    return PandaSDL::Vector3(X * f, Y * f, Z * f);
}

PandaSDL::Vector3 PandaSDL::Vector3::operator*(const double &d)
{
    return PandaSDL::Vector3(X * d, Y * d, Z * d);
}

PandaSDL::Vector3& PandaSDL::Vector3::operator+=(const PandaSDL::Vector3 &v)
{
    X += v.X;
    Y += v.Y;
    Z += v.Z;
    
    return *this;
}

PandaSDL::Vector3& PandaSDL::Vector3::operator-=(const PandaSDL::Vector3 &v)
{
    X -= v.X;
    Y -= v.Y;
    Z -= v.Z;
    
    return *this;
}

PandaSDL::Vector3& PandaSDL::Vector3::operator*=(const PandaSDL::Vector3 &v)
{
    X *= v.X;
    Y *= v.Y;
    Z *= v.Z;
    
    return *this;
}

PandaSDL::Vector3& PandaSDL::Vector3::operator/=(const PandaSDL::Vector3 &v)
{
    X /= v.X;
    Y /= v.Y;
    Z /= v.Z;
    
    return *this;
}

PandaSDL::Vector3 PandaSDL::Vector3::Zero()
{
    return PandaSDL::Vector3(0, 0, 0);
}
