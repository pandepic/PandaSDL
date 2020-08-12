#include "rectangle.h"

PandaSDL::Rectangle::Rectangle()
: X(0), Y(0), Width(0), Height(0)
{
}

PandaSDL::Rectangle::Rectangle(int x, int y, int width, int height)
{
    X = x;
    Y = y;
    Width = width;
    Height = height;
}

PandaSDL::Rectangle::~Rectangle()
{
}

bool PandaSDL::Rectangle::IsEmpty()
{
    if (X == 0 && Y == 0 && Width == 0 && Height == 0)
        return true;

    return false;
}

std::string PandaSDL::Rectangle::ToString()
{
    return "[" + std::to_string(X) + ":" + std::to_string(Y) + ":" + std::to_string(Width) + ":" + std::to_string(Height) + "]";
}

PandaSDL::Rectangle PandaSDL::Rectangle::operator+(const PandaSDL::Rectangle &r)
{
    return PandaSDL::Rectangle(X + r.X, Y + r.Y, Width + r.Width, Height + r.Height);
}

PandaSDL::Rectangle PandaSDL::Rectangle::operator-(const PandaSDL::Rectangle &r)
{
    return PandaSDL::Rectangle(X - r.X, Y - r.Y, Width - r.Width, Height - r.Height);
}

PandaSDL::Rectangle PandaSDL::Rectangle::operator*(const PandaSDL::Rectangle &r)
{
    return PandaSDL::Rectangle(X * r.X, Y * r.Y, Width * r.Width, Height * r.Height);
}

PandaSDL::Rectangle PandaSDL::Rectangle::operator/(const PandaSDL::Rectangle &r)
{
    return PandaSDL::Rectangle(X / r.X, Y / r.Y, Width / r.Width, Height / r.Height);
}

PandaSDL::Rectangle PandaSDL::Rectangle::operator+(const PandaSDL::Vector2 &v)
{
    return PandaSDL::Rectangle(X + (int)v.X, Y + (int)v.Y, Width, Height);
}

PandaSDL::Rectangle PandaSDL::Rectangle::operator-(const PandaSDL::Vector2 &v)
{
    return PandaSDL::Rectangle(X - (int)v.X, Y - (int)v.Y, Width, Height);
}

PandaSDL::Rectangle PandaSDL::Rectangle::operator*(const PandaSDL::Vector2 &v)
{
    return PandaSDL::Rectangle(X * (int)v.X, Y * (int)v.Y, Width, Height);
}

PandaSDL::Rectangle PandaSDL::Rectangle::operator/(const PandaSDL::Vector2 &v)
{
    return PandaSDL::Rectangle(X / (int)v.X, Y / (int)v.Y, Width, Height);
}

PandaSDL::Rectangle PandaSDL::Rectangle::Empty()
{
    return PandaSDL::Rectangle(0, 0, 0, 0);
}
