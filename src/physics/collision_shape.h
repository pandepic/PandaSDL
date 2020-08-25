#ifndef __pandasdl_collisionshape_h__
#define __pandasdl_collisionshape_h__

#include "entity_2D.h"
#include "../math/vector2.h"
#include "../math/rectangle.h"
#include "../graphics/primitive_batch_2D.h"

namespace PandaSDL
{
    enum class eCollisionShapeType
    {
        RECTANGLE,
        CIRCLE,
    };
    
    // forward declarations
    class Entity2D;
    
    class CollisionShape
    {
        public:
            CollisionShape(eCollisionShapeType type, Entity2D* parent, PandaSDL::Vector2 relativePosition);
            virtual ~CollisionShape();
            
            eCollisionShapeType Type;
            Entity2D* Parent;
            // if parent is null then this is an absolute position
            PandaSDL::Vector2 RelativePosition;
            
            virtual void Draw(std::shared_ptr<PrimitiveBatch2D> primitiveBatch, Color color) {}
            virtual bool Intersects(const std::shared_ptr<CollisionShape> &shape) = 0;
            
            // get position accounting for relative position and parent
            virtual PandaSDL::Vector2 GetPosition();
            
        protected:
    };
    
    class CollisionRectangle : public CollisionShape
    {
        public:
            CollisionRectangle(Entity2D* parent, PandaSDL::Rectangle rect)
            : CollisionShape(eCollisionShapeType::RECTANGLE, parent, PandaSDL::Vector2(rect.X, rect.Y)), Rectangle(rect) {}
            
            ~CollisionRectangle() {}
            
            PandaSDL::Rectangle Rectangle;
            
            virtual void Draw(std::shared_ptr<PrimitiveBatch2D> primitiveBatch, Color color) override;
            virtual bool Intersects(const std::shared_ptr<CollisionShape> &shape) override;
            
            PandaSDL::Rectangle GetPositionedRectangle();
            
        protected:
    };
    
    class CollisionCircle : public CollisionShape
    {
        public:
            CollisionCircle(Entity2D* parent, PandaSDL::Vector2 relativePosition, float radius)
            : CollisionShape(eCollisionShapeType::CIRCLE, parent, relativePosition), Radius(radius) {}
            
            ~CollisionCircle() {}
            
            float Radius;
            
            virtual void Draw(std::shared_ptr<PrimitiveBatch2D> primitiveBatch, Color color) override;
            virtual bool Intersects(const std::shared_ptr<CollisionShape> &shape) override;
            
        protected:
    };
}

#endif