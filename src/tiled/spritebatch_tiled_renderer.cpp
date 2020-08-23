#include "spritebatch_tiled_renderer.h"

PandaSDL::SpritebatchTiledRenderer::SpritebatchTiledRenderer()
{
}

PandaSDL::SpritebatchTiledRenderer::~SpritebatchTiledRenderer()
{
}

void PandaSDL::SpritebatchTiledRenderer::Setup(std::shared_ptr<PandaSDL::TiledMap> tiledMap, std::shared_ptr<PandaSDL::SpriteBatch> spriteBatch)
{
    _mapWidth = tiledMap->GetMapWidth();
    _mapHeight = tiledMap->GetMapHeight();
    _tileWidth = tiledMap->GetTileWidth();
    _tileHeight = tiledMap->GetTileHeight();
    
    _tiledMap = tiledMap;
    _spriteBatch = spriteBatch;
    
    _belowLayers = _tiledMap->GetLayersByCustomProperty("Below", "true");
    _aboveLayers = _tiledMap->GetLayersByCustomProperty("Below", "false");
    
    _tilesheet = PandaSDL::Game::AssetManager.LoadTexture2D(_tiledMap->GetCustomProperty("Tilesheet").Value);
}

void PandaSDL::SpritebatchTiledRenderer::Draw(const PandaSDL::Camera2D &camera, bool below, PandaSDL::Vector2 worldOffset)
{
    auto drawLayers = below ? &_belowLayers : &_aboveLayers;
    auto cameraView = camera.GetViewRect() - worldOffset;
    
    auto cameraWidth = cameraView.Width / camera.Zoom;
    auto cameraHeight = cameraView.Height / camera.Zoom;
    
    int loopX = cameraView.X / (int)_tileWidth;
    int loopY = cameraView.Y / (int)_tileHeight;
    int loopWidth = (cameraView.X + cameraWidth) / (int)_tileWidth;
    int loopHeight = (cameraView.Y + cameraHeight) / (int)_tileHeight;
    
    int padding = 2;
    
    loopX -= padding;
    loopY -= padding;
    loopWidth += padding * 2;
    loopHeight += padding * 2;
    
    auto tilesheetWidth = _tilesheet->GetWidth();
    
    auto drawnTiles = 0;
    
    for (const auto &layer : *drawLayers)
    {
        PandaSDL::Vector2 tilePos;
        
        for (int y = loopY; y < loopHeight; y++)
        {
            if (y >= _mapHeight || y < 0)
                continue;
            
            for (int x = loopX; x < loopWidth; x++)
            {
                if (x >= _mapWidth || x < 0)
                    continue;
                
                auto tileID = layer->Tiles[x + _mapWidth * y];
                
                if (tileID == 0)
                    continue;
                
                tilePos.X = x * _tileWidth;
                tilePos.Y = y * _tileHeight;
                tilePos += worldOffset;
                
                auto sheetX = ((tileID - 1) % (tilesheetWidth / _tileWidth)) * _tileWidth;
                auto sheetY = ((tileID - 1) / (tilesheetWidth / _tileWidth)) * _tileHeight;
                
                _spriteBatch->Draw(_tilesheet, PandaSDL::Rectangle(sheetX, sheetY, _tileWidth, _tileHeight), tilePos, PANDASDL_COLOR_WHITE);
            } // x
        } // y
    } // for each layer
} // Draw
