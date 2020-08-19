#ifndef __pandasdl_tilebatchtiledrenderer_h__
#define __pandasdl_tilebatchtiledrenderer_h__

#include "../general/game.h"
#include "../general/camera_2D.h"
#include "../graphics/tile_batch.h"
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
            
            std::string GetTilesheetName();
            
        protected:
            std::string _tilesheetName;
            unsigned int _mapWidth, _mapHeight;
            unsigned int _tileWidth, _tileHeight;
            
            std::shared_ptr<PandaSDL::TileBatch> _tilebatch;
            std::shared_ptr<PandaSDL::TiledMap> _tiledMap;
            std::shared_ptr<PandaSDL::Texture2D> _tilesheet;
            
            std::vector<std::shared_ptr<PandaSDL::TiledMapLayer>> _belowLayers;
            std::vector<std::shared_ptr<PandaSDL::TiledMapLayer>> _aboveLayers;
            
            void BuildTilebatchLayer(const std::shared_ptr<PandaSDL::TiledMapLayer> &layer, bool below);
    };
}

#endif