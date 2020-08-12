#include "tilebatch.h"

//https://blog.tojicode.com/2012/07/sprite-tile-maps-on-gpu.html
//http://media.tojicode.com/webgl-samples/js/webgl-tilemap.js

std::shared_ptr<PandaSDL::Shader> PandaSDL::Tilebatch::DefaultTileShader = nullptr;
bool PandaSDL::Tilebatch::DefaultShaderInitialised = false;

std::string PandaSDL::Tilebatch::DefaultTileShaderVertexCode =
"#version 330 core\n"
"precision mediump float;\n"

"attribute vec2 position;\n"
"attribute vec2 texture;\n"

"varying vec2 pixelCoord;\n"
"varying vec2 texCoord;\n"

"uniform vec2 viewOffset;\n"
"uniform vec2 viewportSize;\n"
"uniform vec2 inverseTileTextureSize;\n"
"uniform float inverseTileSize;\n"

"void main()\n"
"{\n"
"   pixelCoord = (texture * viewportSize) + viewOffset;\n"
"   texCoord = pixelCoord * inverseTileTextureSize * inverseTileSize;\n"
"   gl_Position = vec4(position, 0.0, 1.0);\n"
"}\n";

std::string PandaSDL::Tilebatch::DefaultTileShaderFragmentCode =
"#version 330 core\n"
"precision mediump float;\n"

"varying vec2 pixelCoord;\n"
"varying vec2 texCoord;\n"

"uniform sampler2D dataImage;\n"
"uniform sampler2D atlasImage;\n"

"uniform vec2 inverseTileTextureSize;\n"
"uniform vec2 inverseSpriteTextureSize;\n"
"uniform vec2 tileSize;\n"

"void main()\n"
"{\n"
"   vec4 tile = texture(dataImage, texCoord);\n"
"   if(tile.x == 1.0 && tile.y == 1.0) { discard; }\n"
"   vec2 spriteOffset = floor(tile.xy * 256.0) * tileSize;\n"
"   vec2 spriteCoord = mod(pixelCoord, tileSize);\n"
"   gl_FragColor = texture2D(atlasImage, (spriteOffset + spriteCoord) * inverseSpriteTextureSize);\n"
"}\n";

PandaSDL::Tilebatch::Tilebatch()
    : _dataAllocated(false), _dataArray(nullptr), _width(0), _height(0), _currentLayerEnded(true)
{
    float vertices[] = {
        // triangle 1
        // x, y, u, v
        -1.0f, -1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f,

        // triangle 2
        -1.0f, -1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 0.0f
    };

    for (auto i = 0; i < PANDASDL_TILEBATCH_QUAD_VERTEX_FLOAT_COUNT; i++)
    {
        _templateVertexBuffer[i] = vertices[i];
    }
}

PandaSDL::Tilebatch::~Tilebatch()
{
    ClearData();
}

void PandaSDL::Tilebatch::ClearData()
{
    if (_dataAllocated)
    {
        delete [] _dataArray;
        _dataArray = nullptr;
        _dataAllocated = false;
    }
}

void PandaSDL::Tilebatch::BeginBuild(unsigned int width, unsigned int height, unsigned int tileWidth, unsigned int tileHeight, std::shared_ptr<PandaSDL::Texture2D> atlasTexture)
{
    CheckDefaultShader();
    
    _width = width;
    _height = height;
    _tileWidth = tileWidth;
    _tileHeight = tileHeight;
    AtlasTexture = atlasTexture;
    
    _dataArray = new TileTextureData[_width * _height];
    _dataAllocated = true;
    _currentLayerEnded = false;
}

void PandaSDL::Tilebatch::SetTileAtPosition(unsigned int posx, unsigned int posy, unsigned char x, unsigned char y)
{
    auto index = posx + _width * posy;
    SetTileAtIndex(index, x, y);
}

void PandaSDL::Tilebatch::SetTileAtPosition(unsigned int posx, unsigned int posy, unsigned int tileIndex)
{
    unsigned char x = tileIndex % _width;
    unsigned char y  = tileIndex / _width;
    
    SetTileAtPosition(posx, posy, x, y);
}

void PandaSDL::Tilebatch::SetTileAtIndex(unsigned int index, unsigned char x, unsigned char y)
{
    _dataArray[index].X = x;
    _dataArray[index].Y = y;
    
    _currentLayerEnded = false;
}

void PandaSDL::Tilebatch::SetTileAtIndex(unsigned int index, unsigned int tileIndex)
{
    unsigned char x = tileIndex % _width;
    unsigned char y  = tileIndex / _width;
    
    SetTileAtIndex(index, x, y);
}

void PandaSDL::Tilebatch::EndLayer(bool below)
{
    if (!_dataAllocated)
        return; // throw exception
    
    PandaSDL::TilebatchLayer newLayer;
    newLayer.IsBelow = below;
    
    unsigned char* textureBuffer = new unsigned char[(_width * 2) * _height];
    unsigned int bufferIndex = 0;
    
    for (unsigned int y = 0; y < _height; y++)
    {
        for (unsigned int x = 0; x < _width; x++)
        {
            unsigned int index = x + _width * y;
            
            textureBuffer[bufferIndex] = _dataArray[index].X;
            textureBuffer[bufferIndex + 1] = _dataArray[index].Y;
            
            bufferIndex += 2;
            
            _dataArray[index].X = 0;
            _dataArray[index].Y = 0;
        }
    }
    
    newLayer.DataTexture = std::make_shared<PandaSDL::Texture2D>(GL_RG, GL_NEAREST, GL_CLAMP_TO_EDGE);
    newLayer.DataTexture->Create(_width, _height, textureBuffer);
    Layers.push_back(newLayer);
    
    delete [] textureBuffer;
    _currentLayerEnded = true;
}

void PandaSDL::Tilebatch::EndBuild(bool below)
{
    if (!_currentLayerEnded)
        EndLayer(below);
    
    ClearData();
}

void PandaSDL::Tilebatch::Draw(PandaSDL::Vector2 position, bool below)
{
    for (const auto &layer : Layers)
    {
        if (layer.IsBelow == below)
        {
            //draw layer
        }
    }
}

unsigned int PandaSDL::Tilebatch::GetWidth()
{
    return _width;
}

unsigned int PandaSDL::Tilebatch::GetHeight()
{
    return _height;
}

void PandaSDL::Tilebatch::CheckDefaultShader()
{
    if (PandaSDL::Tilebatch::DefaultShaderInitialised)
        return;
    
    PandaSDL::Tilebatch::DefaultTileShader = PandaSDL::Game::AssetManager.LoadShaderFromString("DefaultTileShader", PandaSDL::Tilebatch::DefaultTileShaderVertexCode, PandaSDL::Tilebatch::DefaultTileShaderFragmentCode);
    PandaSDL::Tilebatch::DefaultShaderInitialised = true;
}
