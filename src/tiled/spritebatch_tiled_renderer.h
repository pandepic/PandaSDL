#ifndef __pandasdl_spritebatchtiledrenderer_h__
#define __pandasdl_spritebatchtiledrenderer_h__

#include "../general/game.h"
#include "../general/camera_2D.h"
#include "../graphics/spritebatch.h"
#include "../graphics/texture_2D.h"
#include "tiled_map.h"
#include "../math/vector2.h"

namespace PandaSDL
{
    class SpritebatchTiledRenderer
    {
        public:
            SpritebatchTiledRenderer();
            ~SpritebatchTiledRenderer();
            
            void Setup(std::shared_ptr<PandaSDL::TiledMap> tiledMap, std::shared_ptr<PandaSDL::Spritebatch> spriteBatch);
            void Draw(const PandaSDL::Camera2D &camera, bool below, PandaSDL::Vector2 worldOffset = PandaSDL::Vector2::Zero());
            
        protected:
            unsigned int _mapWidth, _mapHeight;
            unsigned int _tileWidth, _tileHeight;
            
            std::shared_ptr<PandaSDL::TiledMap> _tiledMap;
            std::shared_ptr<PandaSDL::Spritebatch> _spriteBatch;
            std::shared_ptr<PandaSDL::Texture2D> _tilesheet;
            
            std::vector<std::shared_ptr<PandaSDL::TiledMapLayer>> _belowLayers;
            std::vector<std::shared_ptr<PandaSDL::TiledMapLayer>> _aboveLayers;
    };
}

#endif