#include "collision_shape.h"

PandaSDL::CollisionShape::CollisionShape(eCollisionShapeType type, Entity2D* parent, PandaSDL::Vector2 relativePosition)
    : Type(type), Parent(parent), RelativePosition(relativePosition)
{
}

PandaSDL::CollisionShape::~CollisionShape()
{
}

PandaSDL::Vector2 PandaSDL::CollisionShape::GetPosition()
{
    if (Parent == nullptr)
        return RelativePosition;
    
    return RelativePosition + Parent->Position;
}

void PandaSDL::CollisionRectangle::Draw(std::shared_ptr<PrimitiveBatch2D> primitiveBatch, Color color)
{
    primitiveBatch->DrawFilledRectangle(PandaSDL::Rectangle(GetPosition(), Rectangle.Width, Rectangle.Height), color);
}

bool PandaSDL::CollisionRectangle::Intersects(const std::shared_ptr<CollisionShape> &shape)
{
    switch (shape->Type)
    {
        case eCollisionShapeType::CIRCLE:
        {
            auto circleShape = static_cast<CollisionCircle*>(shape.get());
            return GetPositionedRectangle().CircleIntersects(circleShape->GetPosition(), circleShape->Radius);
        }
        break;
        
        case eCollisionShapeType::RECTANGLE:
        {
            auto rectShape = static_cast<CollisionRectangle*>(shape.get());
            return GetPositionedRectangle().Intersects(rectShape->GetPositionedRectangle());
        }
        break;
    }
    
    return false;
}

PandaSDL::Rectangle PandaSDL::CollisionRectangle::GetPositionedRectangle()
{
    auto position = GetPosition();
    return PandaSDL::Rectangle(position, Rectangle.Width, Rectangle.Height);
}

void PandaSDL::CollisionCircle::Draw(std::shared_ptr<PrimitiveBatch2D> primitiveBatch, Color color)
{
    primitiveBatch->DrawFilledCircle(GetPosition(), Radius, color);
}

bool PandaSDL::CollisionCircle::Intersects(const std::shared_ptr<CollisionShape> &shape)
{
    switch (shape->Type)
    {
        case eCollisionShapeType::CIRCLE:
        {
            auto circleShape = static_cast<CollisionCircle*>(shape.get());
            if (GetPosition().Distance(circleShape->GetPosition()) <= (Radius + circleShape->Radius))
                return true;
        }
        break;
        
        case eCollisionShapeType::RECTANGLE:
        {
            auto rectShape = static_cast<CollisionRectangle*>(shape.get());
            return rectShape->GetPositionedRectangle().CircleIntersects(GetPosition(), Radius);
        }
        break;
    }
    
    return false;
}
