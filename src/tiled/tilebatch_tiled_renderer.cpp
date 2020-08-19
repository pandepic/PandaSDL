#include "tilebatch_tiled_renderer.h"

PandaSDL::TilebatchTiledRenderer::TilebatchTiledRenderer()
    : _mapWidth(0), _mapHeight(0),
    _tileWidth(0), _tileHeight(0)
{
}

PandaSDL::TilebatchTiledRenderer::~TilebatchTiledRenderer()
{
}

void PandaSDL::TilebatchTiledRenderer::Setup(std::shared_ptr<PandaSDL::TiledMap> tiledMap)
{
    _mapWidth = tiledMap->GetMapWidth();
    _mapHeight = tiledMap->GetMapHeight();
    _tileWidth = tiledMap->GetTileWidth();
    _tileHeight = tiledMap->GetTileHeight();
    
    _tiledMap = tiledMap;
    
    _belowLayers = _tiledMap->GetLayersByCustomProperty("Below", "true");
    _aboveLayers = _tiledMap->GetLayersByCustomProperty("Below", "false");
    
    _tilesheetName = _tiledMap->GetCustomProperty("Tilesheet").Value;
    _tilesheet = PandaSDL::Game::AssetManager.LoadTexture2D(_tilesheetName);
    
    _tilebatch = std::make_shared<PandaSDL::TileBatch>();
    
    _tilebatch->BeginBuild(_mapWidth, _mapHeight, _tileWidth, _tileHeight, _tilesheet);
    
    for (const auto &layer : _belowLayers)
    {
        BuildTilebatchLayer(layer, true);
    }
    
    for (const auto &layer : _aboveLayers)
    {
        BuildTilebatchLayer(layer, false);
    }
    
    _tilebatch->EndBuild();
}

void PandaSDL::TilebatchTiledRenderer::Draw(const PandaSDL::Camera2D &camera, bool below)
{
    _tilebatch->Draw({ camera.GetPosition().X, camera.GetPosition().Y }, below, camera.Zoom);
}

void PandaSDL::TilebatchTiledRenderer::BuildTilebatchLayer(const std::shared_ptr<PandaSDL::TiledMapLayer> &layer, bool below)
{
    for (auto y = 0; y < _mapHeight; y++)
    {
        for (auto x = 0; x < _mapWidth; x++)
        {
            auto tileID = layer->Tiles[x + _mapWidth * y];
            
            if (tileID == 0)
                continue;
            
            _tilebatch->SetTileAtPosition(x, y, tileID - 1);
        }
    }
    
    _tilebatch->EndLayer(below);
}

std::string PandaSDL::TilebatchTiledRenderer::GetTilesheetName()
{
    return _tilesheetName;
}