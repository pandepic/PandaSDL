#include "tile_batch.h"

std::shared_ptr<PandaSDL::Shader> PandaSDL::TileBatch::DefaultTileShader = nullptr;
bool PandaSDL::TileBatch::DefaultShaderInitialised = false;

std::string PandaSDL::TileBatch::DefaultTileShaderVertexCode =
"#version 330 core\n"
"precision mediump float;\n"

"attribute vec2 position;\n"
"attribute vec2 texture;\n"

"varying vec2 pixelCoord;\n"
"varying vec2 texCoord;\n"

"uniform vec2 viewOffset;\n"
"uniform vec2 viewportSize;\n"
"uniform vec2 inverseTileTextureSize;\n"
"uniform vec2 inverseTileSize;\n"

"void main()\n"
"{\n"
"   pixelCoord = (texture * viewportSize) + viewOffset;\n"
"   texCoord = pixelCoord * inverseTileTextureSize * inverseTileSize;\n"
"   gl_Position = vec4(position, -1.0, 1.0);\n"
"}\n";

std::string PandaSDL::TileBatch::DefaultTileShaderFragmentCode =
"#version 330 core\n"
"precision mediump float;\n"

"varying vec2 pixelCoord;\n"
"varying vec2 texCoord;\n"

"uniform sampler2D dataImage;\n"
"uniform sampler2D atlasImage;\n"

"uniform vec2 inverseTileTextureSize;\n"
"uniform vec2 inverseSpriteTextureSize;\n"
"uniform vec2 tileSize;\n"
//"uniform uint animationTimer;\n" todo : global timer or timer array?

"void main()\n"
"{\n"
"   if(texCoord.x < 0 || texCoord.y < 0 || texCoord.x > 1 || texCoord.y > 1) { discard; }\n" // discard if outside data texture/outside tilemap

"   vec4 tile = texture(dataImage, texCoord);\n"
"   if(tile.x == 1.0 && tile.y == 1.0) { discard; }\n"

"   vec2 spriteOffset = floor(tile.xy * 256.0) * tileSize;\n" // todo : add tile animation frame offset here
"   vec2 spriteCoord = mod(pixelCoord, tileSize);\n"

"   gl_FragColor = texture2D(atlasImage, (spriteOffset + spriteCoord) * inverseSpriteTextureSize);\n"
"}\n";

PandaSDL::TileBatch::TileBatch()
    : _dataAllocated(false), _dataArray(nullptr), _width(0), _height(0), _currentLayerEnded(true), _setup(false), _tileScale(1.0f),
    _tileSize(0, 0), _inverseTileSize(0, 0)
{
}

PandaSDL::TileBatch::~TileBatch()
{
    ClearData();
}

void PandaSDL::TileBatch::ClearData()
{
    if (_dataAllocated)
    {
        delete [] _dataArray;
        _dataArray = nullptr;
        _dataAllocated = false;
    }
}

void PandaSDL::TileBatch::Setup()
{
    if (_setup)
        return;
    
    auto windowRect = PandaSDL::Game::GameInstance->GetWindowRect();
    _viewportSize = { windowRect.Width, windowRect.Height };
    _scaledViewportSize = _viewportSize; // todo : handle camera zoom for scale
    
    _tileSize = { _tileWidth, _tileHeight };
    _inverseTileSize = { 1.0f / _tileWidth, 1.0f / _tileHeight };
    
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

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    
    auto attribSize = sizeof(float) * 2;
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *)(attribSize * 0));
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *)(attribSize * 1));
    glEnableVertexAttribArray(1);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * PANDASDL_TILEBATCH_QUAD_VERTEX_FLOAT_COUNT, vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    _setup = true;
}

void PandaSDL::TileBatch::BeginBuild(unsigned int width, unsigned int height, unsigned int tileWidth, unsigned int tileHeight, std::shared_ptr<PandaSDL::Texture2D> atlasTexture, std::shared_ptr<Shader> tileShader)
{
    CheckDefaultShader();
    
    _width = width;
    _height = height;
    _tileWidth = tileWidth;
    _tileHeight = tileHeight;
    AtlasTexture = atlasTexture;
    
    _tilesheetTilesWidth = AtlasTexture->GetWidth() / _tileWidth;
    _tilesheetTilesHeight = AtlasTexture->GetHeight() / _tileHeight;
    
    _inverseSpriteTextureSize = { AtlasTexture->GetTexelWidth(), AtlasTexture->GetTexelHeight() };
    
    _tileShader = DefaultTileShader;
    
    _dataArray = new TileTextureData[_width * _height];
    _dataAllocated = true;
    _currentLayerEnded = false;
    
    for (unsigned int y = 0; y < _height; y++)
    {
        for (unsigned int x = 0; x < _width; x++)
        {
            auto index = x + _width * y;
            _dataArray[index].X = 255;
            _dataArray[index].Y = 255;
        }
    }
    
    if (!_setup)
        Setup();
}

void PandaSDL::TileBatch::SetTileAtPosition(unsigned int posx, unsigned int posy, unsigned char x, unsigned char y)
{
    auto index = posx + _width * posy;
    SetTileAtIndex(index, x, y);
}

void PandaSDL::TileBatch::SetTileAtPosition(unsigned int posx, unsigned int posy, unsigned int tileIndex)
{
    unsigned char x = tileIndex % _tilesheetTilesWidth;
    unsigned char y = tileIndex / _tilesheetTilesWidth;
    
    SetTileAtPosition(posx, posy, x, y);
}

void PandaSDL::TileBatch::SetTileAtIndex(unsigned int index, unsigned char x, unsigned char y)
{
    _dataArray[index].X = x;
    _dataArray[index].Y = y;
    
    _currentLayerEnded = false;
}

void PandaSDL::TileBatch::SetTileAtIndex(unsigned int index, unsigned int tileIndex)
{
    unsigned char x = tileIndex % _width;
    unsigned char y  = tileIndex / _width;
    
    SetTileAtIndex(index, x, y);
}

void PandaSDL::TileBatch::EndLayer(bool below)
{
    if (!_dataAllocated)
        return; // todo : throw exception?
    
    PandaSDL::TileBatchLayer newLayer;
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
            
            _dataArray[index].X = 255;
            _dataArray[index].Y = 255;
        }
    }
    
    newLayer.DataTexture = std::make_shared<PandaSDL::Texture2D>(GL_RG, GL_NEAREST, GL_CLAMP_TO_EDGE);
    newLayer.DataTexture->Create(_width, _height, textureBuffer);
    Layers.push_back(newLayer);
    
    delete [] textureBuffer;
    _currentLayerEnded = true;
}

void PandaSDL::TileBatch::EndBuild(bool below)
{
    if (!_currentLayerEnded)
        EndLayer(below);
    
    ClearData();
}

void PandaSDL::TileBatch::Draw(PandaSDL::Vector2 position, bool below, float scale)
{
    _tileShader->Use();
    
    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    
    _scaledViewportSize = { _viewportSize.x / scale, _viewportSize.y / scale };
    
    _tileShader->SetVector2f("viewportSize", _scaledViewportSize);
    _tileShader->SetVector2f("inverseSpriteTextureSize", _inverseSpriteTextureSize);
    _tileShader->SetVector2f("tileSize", _tileSize);
    _tileShader->SetVector2f("inverseTileSize", _inverseTileSize);
    _tileShader->SetVector2f("viewOffset", { floor(position.X * _tileScale / scale), floor(position.Y * _tileScale / scale) });
    _tileShader->SetInteger("atlasImage", 0);
    _tileShader->SetInteger("dataImage", 1);
    
    glActiveTexture(GL_TEXTURE0);
    AtlasTexture->Bind();
    
    glActiveTexture(GL_TEXTURE1);
    
    for (const auto &layer : Layers)
    {
        if (layer.IsBelow == below)
        {
            glm::vec2 inverseTileTextureSize = { layer.DataTexture->GetTexelWidth(), layer.DataTexture->GetTexelHeight() };
            _tileShader->SetVector2f("inverseTileTextureSize", inverseTileTextureSize);
            
            layer.DataTexture->Bind();
            
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }
    
    glActiveTexture(GL_TEXTURE0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

unsigned int PandaSDL::TileBatch::GetWidth()
{
    return _width;
}

unsigned int PandaSDL::TileBatch::GetHeight()
{
    return _height;
}

void PandaSDL::TileBatch::CheckDefaultShader()
{
    if (PandaSDL::TileBatch::DefaultShaderInitialised)
        return;
    
    PandaSDL::TileBatch::DefaultTileShader = PandaSDL::Game::AssetManager.LoadShaderFromString("DefaultTileShader", PandaSDL::TileBatch::DefaultTileShaderVertexCode, PandaSDL::TileBatch::DefaultTileShaderFragmentCode);
    PandaSDL::TileBatch::DefaultShaderInitialised = true;
}
