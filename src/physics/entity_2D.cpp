#include "entity_2D.h"

PandaSDL::Entity2D::Entity2D()
    : Width(0), Height(0), CanMove(false)
{
}

PandaSDL::Entity2D::Entity2D(const Vector2 &position, unsigned int width, unsigned int height)
    : Position(position), Width(width), Height(height)
{
}

PandaSDL::Entity2D::Entity2D(const Rectangle &rect)
    : Position(rect.X, rect.Y), Width(rect.Width), Height(rect.Height)
{
}

PandaSDL::Entity2D::~Entity2D()
{
    CollisionShapes.clear();
}

void PandaSDL::Entity2D::Update(const Timer &gameTimer)
{
    Position += Velocity * gameTimer.GetDelta();
}

void PandaSDL::Entity2D::AddCollisionRectangle(Rectangle rect)
{
    CollisionShapes.push_back(std::make_shared<PandaSDL::CollisionRectangle>(this, rect));
}

void PandaSDL::Entity2D::AddCollisionCircle(Vector2 position, float radius)
{
    CollisionShapes.push_back(std::make_shared<PandaSDL::CollisionCircle>(this, position, radius));
}
