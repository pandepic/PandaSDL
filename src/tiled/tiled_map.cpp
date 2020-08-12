#include "tiled_map.h"

PandaSDL::TiledCustomProperty::TiledCustomProperty()
    : Name(""), Type(""), Value("")
{
}

bool PandaSDL::TiledCustomProperty::IsEmpty()
{
    if (Name.length() == 0 && Type.length() == 0 && Value.length() == 0)
        return true;
    
    return false;
}

PandaSDL::TiledMapLayer::TiledMapLayer()
    : Tiles(nullptr)
{
}

PandaSDL::TiledMapLayer::~TiledMapLayer()
{
    if (Tiles != nullptr)
    {
        delete [] Tiles;
        Tiles = nullptr;
    }
}

PandaSDL::TiledCustomProperty PandaSDL::TiledMapLayer::GetCustomProperty(std::string name)
{
    for (auto property : CustomProperties)
    {
        if (property.Name == name)
            return property;
    }
    
    TiledCustomProperty emptyProperty;
    return emptyProperty;
}

PandaSDL::TiledMap::TiledMap()
    : _mapWidth(0), _mapHeight(0), _tileWidth(0), _tileHeight(0)
{
}

PandaSDL::TiledMap::~TiledMap()
{
}

void PandaSDL::TiledMap::Load(std::string filepath)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filepath.c_str());
    
    if (!result)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::LOADING_TILEDMAP_FAILED, result.description());
    
    auto mapRoot = doc.child("map");
    auto mapCustomProperties = mapRoot.child("properties");
    
    _tileWidth = mapRoot.attribute("tilewidth").as_uint();
    _tileHeight = mapRoot.attribute("tileheight").as_uint();
    _mapWidth = mapRoot.attribute("width").as_uint();
    _mapHeight = mapRoot.attribute("height").as_uint();
    
    if (mapCustomProperties)
    {
        for (auto property : mapCustomProperties.children("property"))
        {
            TiledCustomProperty newProperty;
            newProperty.Name = property.attribute("name").value();
            newProperty.Type = property.attribute("type").value();
            newProperty.Value = property.attribute("value").value();
            
            _customProperties.push_back(newProperty);
        }
    }
    
    for (auto layer : mapRoot.children("layer"))
    {
        auto newLayer = std::make_shared<TiledMapLayer>();
        newLayer->Name = layer.attribute("name").value();
        newLayer->Tiles = new unsigned int[_mapWidth * _mapHeight];
        
        auto layerCustomProperties = layer.child("properties");
        
        if (layerCustomProperties)
        {
            for (auto property : layerCustomProperties.children("property"))
            {
                TiledCustomProperty newProperty;
                newProperty.Name = property.attribute("name").value();
                newProperty.Type = property.attribute("type").value();
                newProperty.Value = property.attribute("value").value();
                
                newLayer->CustomProperties.push_back(newProperty);
            }
        }
        
        unsigned int tileArrayIndex = 0;
        std::string layerTileData = layer.child("data").text().as_string();
        auto tileDataRows = PandaSDL::SplitString(layerTileData, "\n");
        
        for (const auto &row : tileDataRows)
        {
            if (row.length() == 0)
                continue;
            
            auto rowTiles = PandaSDL::SplitString(row, ",");
            
            for (const auto &tile : rowTiles)
            {
                if (tile.length() == 0)
                    continue;
                
                newLayer->Tiles[tileArrayIndex] = std::stoi(tile);
                tileArrayIndex += 1;
            }
        }
        
        _layers.push_back(newLayer);
    }
}

PandaSDL::TiledCustomProperty PandaSDL::TiledMap::GetCustomProperty(std::string name)
{
    for (auto property : _customProperties)
    {
        if (property.Name == name)
            return property;
    }
    
    TiledCustomProperty emptyProperty;
    return emptyProperty;
}

unsigned int PandaSDL::TiledMap::GetTileWidth()
{
    return _tileWidth;
}

unsigned int PandaSDL::TiledMap::GetTileHeight()
{
    return _tileHeight;
}

unsigned int PandaSDL::TiledMap::GetMapWidth()
{
    return _mapWidth;
}

unsigned int PandaSDL::TiledMap::GetMapHeight()
{
    return _mapHeight;
}

std::vector<std::shared_ptr<PandaSDL::TiledMapLayer>> PandaSDL::TiledMap::GetLayers()
{
    return _layers;
}

std::vector<std::shared_ptr<PandaSDL::TiledMapLayer>> PandaSDL::TiledMap::GetLayersByCustomProperty(std::string name, std::string value)
{
    std::vector<std::shared_ptr<PandaSDL::TiledMapLayer>> layers;
    
    for (const auto &layer : _layers)
    {
        bool addLayer = false;
        
        for (const auto &property : layer->CustomProperties)
        {
            if (property.Name == name && property.Value == value)
                addLayer = true;
        }
        
        if (addLayer)
            layers.push_back(layer);
    }
    
    return layers;
}
