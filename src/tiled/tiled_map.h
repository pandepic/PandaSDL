#ifndef __pandasdl_tiledmap_h__
#define __pandasdl_tiledmap_h__

/*
    Import data from a CSV type Tiled TMX.
*/

#include "../../lib_include/pugixml/pugixml.hpp"

#include <iostream>
#include <vector>
#include <memory>

#include "../general/globals.h"
#include "../general/extensions.h"

namespace PandaSDL
{
    struct TiledCustomProperty
    {
        TiledCustomProperty();
        
        std::string Name, Type, Value;
        bool IsEmpty();
    };
    
    struct TiledMapLayer
    {
        TiledMapLayer();
        ~TiledMapLayer();
        
        std::string Name;
        std::vector<TiledCustomProperty> CustomProperties;
        unsigned int *Tiles; // tile index 0 = empty
        
        TiledCustomProperty GetCustomProperty(std::string name);
    };
    
    class TiledMap
    {
        public:
            TiledMap();
            ~TiledMap();
            
            void Load(std::string filepath);
            TiledCustomProperty GetCustomProperty(std::string name);
            
            unsigned int GetTileWidth();
            unsigned int GetTileHeight();
            unsigned int GetMapWidth();
            unsigned int GetMapHeight();
            
            std::vector<std::shared_ptr<TiledMapLayer>> GetLayers();
            std::vector<std::shared_ptr<TiledMapLayer>> GetLayersByCustomProperty(std::string property, std::string value);
            
        protected:
            unsigned int _tileWidth, _tileHeight;
            unsigned int _mapWidth, _mapHeight;
            
            std::vector<std::shared_ptr<TiledMapLayer>> _layers;
            
            std::vector<TiledCustomProperty> _customProperties;
    };
}

#endif