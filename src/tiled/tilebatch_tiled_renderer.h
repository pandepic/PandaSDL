#ifndef __pandasdl_tilebatchtiledrenderer_h__
#define __pandasdl_tilebatchtiledrenderer_h__

#include "../general/game.h"
#include "../general/camera_2D.h"
#include "../graphics/tilebatch.h"
#include "../graphics/texture_2D.h"
#include "tiled_map.h"

namespace PandaSDL
{
    class TilebatchTiledRenderer
    {
        public:
            TilebatchTiledRenderer();
            ~TilebatchTiledRenderer();
            
            void Setup(std::shared_ptr<PandaSDL::TiledMap> tiledMap);
            void Draw(const PandaSDL::Camera2D &camera, bool below);
            
        protected:
            unsigned int _mapWidth, _mapHeight;
            unsigned int _tileWidth, _tileHeight;
            
            std::shared_ptr<PandaSDL::TiledMap> _tiledMap;
            std::shared_ptr<PandaSDL::Texture2D> _tilesheet;
            
            std::vector<std::shared_ptr<PandaSDL::TiledMapLayer>> _belowLayers;
            std::vector<std::shared_ptr<PandaSDL::TiledMapLayer>> _aboveLayers;
    };
}

#endif