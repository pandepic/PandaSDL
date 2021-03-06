#ifndef __pandasdl_collisionmanager_h__
#define __pandasdl_collisionmanager_h__

#include "collision_shape.h"
#include "../math/vector2.h"
#include "../general/timer.h"
#include "entity_2D.h"
#include "../graphics/primitive_batch_2D.h"

namespace PandaSDL
{
    class CollisionManager
    {
        public:
            CollisionManager();
            ~CollisionManager();
            
            void Update(const Timer &gameTimer);
            void Draw(std::shared_ptr<PrimitiveBatch2D> primitiveBatch, Color color, bool canMove = false);
            
            void RegisterEntity(std::shared_ptr<Entity2D> entity);
            void RemoveEntity(std::shared_ptr<Entity2D> entity);
            
        protected:
            std::vector<std::shared_ptr<Entity2D>> _entities;
    };
}

#endif