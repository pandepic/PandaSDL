#ifndef __pandasdl_entity_2d_h__
#define __pandasdl_entity_2d_h__

#include "../math/vector2.h"
#include "../general/timer.h"

namespace PandaSDL
{
    class Entity2D
    {
        public:
            Entity2D();
            ~Entity2D();
            
            void Update(const Timer &gameTimer);
            
            unsigned int Width, Height;
            PandaSDL::Vector2 Position, Velocity;
            
        protected:
    };
}

#endif
