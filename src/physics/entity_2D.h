#ifndef __pandasdl_entity_2d_h__
#define __pandasdl_entity_2d_h__

#include "../math/vector2.h"
#include "../math/rectangle.h"
#include "../general/timer.h"
#include "collision_shape.h"

namespace PandaSDL
{
    // forward declarations
    class CollisionShape;
    
    class Entity2D
    {
        public:
            Entity2D();
            Entity2D(const Vector2 &position, unsigned int width = 0, unsigned int height = 0);
            Entity2D(const Rectangle &rect);
            ~Entity2D();
            
            unsigned int Width, Height;
            PandaSDL::Vector2 Position, Velocity;
            std::vector<std::shared_ptr<PandaSDL::CollisionShape>> CollisionShapes;
            
            void Update(const Timer &gameTimer);
            
            void AddCollisionRectangle(Rectangle rect);
            void AddCollisionCircle(Vector2 position, float radius);
            
        protected:
    };
}

#endif
