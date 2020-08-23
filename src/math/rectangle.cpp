#include "rectangle.h"

PandaSDL::Rectangle::Rectangle()
    : X(0), Y(0), Width(0), Height(0)
{
}

PandaSDL::Rectangle::Rectangle(const PandaSDL::Vector2 &vec, int width, int height)
    : X(vec.X), Y(vec.Y), Width(width), Height(height)
{
}

PandaSDL::Rectangle::Rectangle(int x, int y, int width, int height)
    : X(x), Y(y), Width(width), Height(height)
{
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

PandaSDL::Rectangle PandaSDL::Rectangle::operator+(const PandaSDL::Rectangle &rect)
{
    return PandaSDL::Rectangle(X + rect.X, Y + rect.Y, Width + rect.Width, Height + rect.Height);
}

PandaSDL::Rectangle PandaSDL::Rectangle::operator-(const PandaSDL::Rectangle &rect)
{
    return PandaSDL::Rectangle(X - rect.X, Y - rect.Y, Width - rect.Width, Height - rect.Height);
}

PandaSDL::Rectangle PandaSDL::Rectangle::operator*(const PandaSDL::Rectangle &rect)
{
    return PandaSDL::Rectangle(X * rect.X, Y * rect.Y, Width * rect.Width, Height * rect.Height);
}

PandaSDL::Rectangle PandaSDL::Rectangle::operator/(const PandaSDL::Rectangle &rect)
{
    return PandaSDL::Rectangle(X / rect.X, Y / rect.Y, Width / rect.Width, Height / rect.Height);
}

PandaSDL::Rectangle PandaSDL::Rectangle::operator+(const PandaSDL::Vector2 &vec)
{
    return PandaSDL::Rectangle(X + (int)vec.X, Y + (int)vec.Y, Width, Height);
}

PandaSDL::Rectangle PandaSDL::Rectangle::operator-(const PandaSDL::Vector2 &vec)
{
    return PandaSDL::Rectangle(X - (int)vec.X, Y - (int)vec.Y, Width, Height);
}

PandaSDL::Rectangle PandaSDL::Rectangle::operator*(const PandaSDL::Vector2 &vec)
{
    return PandaSDL::Rectangle(X * (int)vec.X, Y * (int)vec.Y, Width, Height);
}

PandaSDL::Rectangle PandaSDL::Rectangle::operator/(const PandaSDL::Vector2 &vec)
{
    return PandaSDL::Rectangle(X / (int)vec.X, Y / (int)vec.Y, Width, Height);
}

bool PandaSDL::Rectangle::operator==(const PandaSDL::Vector2 &vec)
{
    return (X == (int)vec.X && Y == (int)vec.Y);
}

bool PandaSDL::Rectangle::operator!=(const PandaSDL::Vector2 &vec)
{
    return !(X == (int)vec.X && Y == (int)vec.Y);
}

bool PandaSDL::Rectangle::operator==(const PandaSDL::Rectangle &rect)
{
    return (X == rect.X && Y == rect.Y && Width == rect.Width && Height == rect.Height);
}

bool PandaSDL::Rectangle::operator!=(const PandaSDL::Rectangle &rect)
{
    return !(X == rect.X && Y == rect.Y && Width == rect.Width && Height == rect.Height);
}

PandaSDL::Rectangle PandaSDL::Rectangle::Empty()
{
    return PandaSDL::Rectangle(0, 0, 0, 0);
}

bool PandaSDL::Rectangle::Contains(const PandaSDL::Vector2 &vec) const
{
    return ((((X <= vec.X) && (vec.X < (X + Width))) && (Y <= vec.Y)) && (vec.Y < (Y + Height)));
}

bool PandaSDL::Rectangle::Contains(const PandaSDL::Rectangle &rect) const
{
    return ((((X <= rect.X) && ((rect.X + rect.Width) <= (X + Width))) && (Y <= rect.Y)) && ((rect.Y + rect.Height) <= (Y + Height)));
}

bool PandaSDL::Rectangle::Intersects(const PandaSDL::Rectangle &rect) const
{
    return (rect.X < (X + Width) && X < (rect.X + rect.Width) && rect.Y < (Y + Height) && Y < (rect.Y + rect.Height));
}

PandaSDL::Rectangle PandaSDL::Rectangle::Intersects(const PandaSDL::Rectangle &rect1, const PandaSDL::Rectangle &rect2)
{
    if (!rect1.Intersects(rect2))
        return PandaSDL::Rectangle::Empty();
    
    auto right = std::min(rect1.X + rect1.Width, rect2.X + rect2.Width);
    auto left = std::max(rect1.X, rect2.X);
    auto top = std::max(rect1.Y, rect2.Y);
    auto bottom = std::min(rect1.Y + rect1.Height, rect2.Y + rect2.Height);
    
    return { left, top, right - left, bottom - top };
}
