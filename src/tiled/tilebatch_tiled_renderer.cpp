#include "tilebatch_tiled_renderer.h"

PandaSDL::TilebatchTiledRenderer::TilebatchTiledRenderer()
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
    
    _tilesheet = PandaSDL::Game::AssetManager.LoadTexture2D(_tiledMap->GetCustomProperty("Tilesheet").Value);
}

void PandaSDL::TilebatchTiledRenderer::Draw(const PandaSDL::Camera2D &camera, bool below)
{
}
