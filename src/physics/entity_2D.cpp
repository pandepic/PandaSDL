#include "entity_2D.h"

PandaSDL::Entity2D::Entity2D()
{
}

PandaSDL::Entity2D::~Entity2D()
{
}

void PandaSDL::Entity2D::Update(const Timer &gameTimer)
{
    Position += Velocity * gameTimer.GetDelta();
}
