#include "collision_manager.h"

PandaSDL::CollisionManager::CollisionManager()
{
    
}

PandaSDL::CollisionManager::~CollisionManager()
{
    
}

void PandaSDL::CollisionManager::Update(const Timer &gameTimer)
{
    auto delta = gameTimer.GetDelta();
    
    for (auto &entity : _entities)
    {
        // todo : find a way to more efficiently group moving and static entities to avoid this check
        if (entity->Velocity.IsZero())
            continue;
        
        bool allowMovement = true;
        auto prevPosition = entity->Position;
        entity->Position += entity->Velocity * delta;
        
        // todo : use spatial hash for efficient checking instead of checking everything vs everything
        for (auto &checkEntity : _entities)
        {
            if (entity != checkEntity)
            {
                for (const auto &entityShape : entity->CollisionShapes)
                {
                    for (const auto &checkEntityShape : checkEntity->CollisionShapes)
                    {
                        if (entityShape->Intersects(checkEntityShape))
                            allowMovement = false;
                    }
                }
            }
        }
        
        if (!allowMovement)
            entity->Position = prevPosition;
    }
}

void PandaSDL::CollisionManager::Draw(std::shared_ptr<PrimitiveBatch2D> primitiveBatch, Color color)
{
    for (auto &entity : _entities)
    {
        for (auto &shape : entity->CollisionShapes)
        {
            shape->Draw(primitiveBatch, color);
        }
    }
}

void PandaSDL::CollisionManager::RegisterEntity(std::shared_ptr<Entity2D> entity)
{
    if (entity == nullptr)
        return;
    
    _entities.push_back(entity);
}

void PandaSDL::CollisionManager::RemoveEntity(std::shared_ptr<Entity2D> entity)
{
    auto find = std::find(_entities.begin(), _entities.end(), entity);
    if (find != _entities.end())
       _entities.erase(std::remove(_entities.begin(), _entities.end(), entity), _entities.end());
}
