#include "sprite_batch.h"

std::shared_ptr<PandaSDL::Shader> PandaSDL::SpriteBatch::DefaultSpriteShader = nullptr;
std::string PandaSDL::SpriteBatch::DefaultSpriteShaderVertexCode =
"#version 330 core\n"
"layout (location = 0) in vec2 vPosition;\n"
"layout (location = 1) in vec2 vTexCoords;\n"
"layout (location = 2) in vec4 vColour;\n"

"out vec2 fTexCoords;\n"
"out vec4 fColour;\n"

"uniform mat4 mProjectionView;\n"

"void main()\n"
"{\n"
"    fTexCoords = vTexCoords;\n"
"    fColour = vColour;\n"
"    gl_Position = mProjectionView * vec4(vPosition.x, vPosition.y, 0.0, 1.0);\n"
"}\n";

std::string PandaSDL::SpriteBatch::DefaultSpriteShaderFragmentCode =
"#version 330 core\n"
"in vec2 fTexCoords;\n"
"in vec4 fColour;\n"

"out vec4 fFragColour;\n"

"uniform sampler2D image;\n" // todo : texture array?

"void main()\n"
"{\n"
"    fFragColour = texture(image, fTexCoords) * fColour;\n"
"}\n";

std::shared_ptr<PandaSDL::Shader> PandaSDL::SpriteBatch::DefaultFontShader = nullptr;
std::string PandaSDL::SpriteBatch::DefaultFontShaderVertexCode = PandaSDL::SpriteBatch::DefaultSpriteShaderVertexCode;

std::string PandaSDL::SpriteBatch::DefaultFontShaderFragmentCode =
"#version 330 core\n"
"in vec2 fTexCoords;\n"
"in vec4 fColour;\n"

"out vec4 color;\n"

"uniform sampler2D image;\n"

"void main()\n"
"{\n"
"    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(image, fTexCoords).r);\n"
"    color = fColour * sampled;\n"
"}\n";

bool PandaSDL::SpriteBatch::DefaultShadersInitialised = false;
glm::mat4 PandaSDL::SpriteBatch::_defaultTransform = glm::mat4(1.0f);

PandaSDL::SpriteBatch::SpriteBatch()
: _begin(false), _initialised(false)
{
    CheckDefaultShaders();
}

PandaSDL::SpriteBatch::~SpriteBatch()
{
}

void PandaSDL::SpriteBatch::Setup(int screenWidth, int screenHeight, bool invertY, std::shared_ptr<Shader> spriteShader, unsigned int maxBatchSize)
{
    if (_initialised)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::SPRITEBATCH_SETUP, "Setup can only be called once per SpriteBatch.");
    
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;
    _spriteShader = spriteShader;
    _maxBatchSize = maxBatchSize;

    if (invertY)
        _projection = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight, -1.0f, 1.0f);
    else
        _projection = glm::ortho(0.0f, (float)_screenWidth, (float)_screenHeight, 0.0f, -1.0f, 1.0f);

    _spriteShader->Use().SetInteger("image", 0);
    
    unsigned int indicesTemplate[PANDASDL_QUAD_INDEX_COUNT] = {
        0, 1, 2, // tri 1
        0, 3, 1 // tri 2
    };
    
    unsigned int indices[PANDASDL_QUAD_INDEX_COUNT * _maxBatchSize];
    
    // pre calculate index buffer
    for (int i = 0; i < _maxBatchSize; i++)
    {
       auto startIndex = i * PANDASDL_QUAD_INDEX_COUNT;
       auto offset = i * PANDASDL_QUAD_VERTEX_COUNT;
       
       indices[startIndex + 0] = indicesTemplate[0] + offset;
       indices[startIndex + 1] = indicesTemplate[1] + offset;
       indices[startIndex + 2] = indicesTemplate[2] + offset;
           
       indices[startIndex + 3] = indicesTemplate[3] + offset;
       indices[startIndex + 4] = indicesTemplate[4] + offset;
       indices[startIndex + 5] = indicesTemplate[5] + offset;
    }
    
    _vao = std::make_unique<VertexArrayObject>(true);
    _vbo = _vao->AddVertexBufferObject(sizeof(SpriteBatchVertex) * _maxBatchSize * PANDASDL_QUAD_VERTEX_COUNT, nullptr, GL_DYNAMIC_DRAW, true);
    _ibo = _vao->AddIndexBufferObject(sizeof(indices), indices, GL_STATIC_DRAW, true);
    _vao->ApplyVertexAttribTypeMapF(SpriteBatchVertex::VertexAttribMap());
    _vao->Unbind(true);

    float vertices[] = {
        0.0f, 1.0f, // bottom left (0)
        1.0f, 0.0f, // top right (1)
        0.0f, 0.0f, // top left (2)
        1.0f, 1.0f, // bottom right (3)
    };

    for (int i = 0; i < PANDASDL_QUAD_VERTEX_FLOAT_COUNT; i++)
    {
        _tempVertexBuffer[i] = vertices[i];
    }

    _initialised = true;
}

void PandaSDL::SpriteBatch::Begin(glm::mat4 transform)
{
    if (!_initialised)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::SPRITEBATCH_BEGIN, "Must call Setup before drawing.");
    if (_begin)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::SPRITEBATCH_BEGIN, "Call End on the current batch before using Begin again.");
    
    _currentBatchTransform = transform;
    _begin = true;
}

void PandaSDL::SpriteBatch::Draw(
    std::shared_ptr<PandaSDL::Texture2D> texture,
    PandaSDL::Vector2 position,
    PandaSDL::Color color,
    Vector2 scale,
    float rotation,
    eSpriteFlip flip,
    Vector2 origin)
{
    Draw(texture, PandaSDL::Rectangle::Empty(), position, color, scale, rotation, flip, origin);
}

void PandaSDL::SpriteBatch::Draw(
    std::shared_ptr<PandaSDL::Texture2D> texture,
    PandaSDL::Rectangle sourceRect,
    PandaSDL::Vector2 position,
    PandaSDL::Color color,
    Vector2 scale,
    float rotation,
    eSpriteFlip flip,
    Vector2 origin)
{
    if (!_begin)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::SPRITEBATCH_BEGIN, "Begin must be called before Draw.");

    if (sourceRect.Width == 0)
        sourceRect.Width = texture->GetWidth();
    if (sourceRect.Height == 0)
        sourceRect.Height = texture->GetHeight();

    if (sourceRect.X > texture->GetWidth())
        sourceRect.X = texture->GetWidth();
    if (sourceRect.Y > texture->GetHeight())
        sourceRect.Y = texture->GetHeight();

    if (sourceRect.X + sourceRect.Width > texture->GetWidth())
        sourceRect.Width = texture->GetWidth() - sourceRect.X;
    if (sourceRect.Y + sourceRect.Height > texture->GetHeight())
        sourceRect.Height = texture->GetHeight() - sourceRect.Y;

    if (sourceRect.Width <= 0 || sourceRect.Height <= 0)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::SPRITEBATCH_DRAW, "Invalid source rect " + sourceRect.ToString());

    PandaSDL::SpriteBatchItem batchItem;
    batchItem.Texture = texture;
    batchItem.SourceRect = sourceRect;
    batchItem.Position = position;
    batchItem.Color = color;
    batchItem.Scale = scale;
    batchItem.Rotation = rotation;
    batchItem.Flip = flip;
    batchItem.Origin = origin;

    _currentBatch.push_back(batchItem);
}

void PandaSDL::SpriteBatch::DrawText(std::shared_ptr<SpriteFont> font, std::string text, unsigned int size, Vector2 position, Color color, bool alignPosition, Vector2 scale, float rotation, eSpriteFlip flip, Vector2 origin)
{
    if (!_begin)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::SPRITEBATCH_BEGIN, "Begin must be called before DrawText.");
    
    auto currentPosition = position;
    
    std::vector<PandaSDL::SpriteBatchItem> textBatch;
    
    int xOffsetAll = -1;
    int yOffsetAll = -1;
    
    for (const char& c : text)
    {
        auto charData = font->GetCharacterData(c, size);
        
        if (!charData->Whitespace)
        {
            PandaSDL::SpriteBatchItem batchItem;
            batchItem.Texture = charData->Texture;
            batchItem.SourceRect = charData->SourceRect;
            batchItem.Color = color;
            batchItem.Scale = scale;
            batchItem.Rotation = rotation;
            batchItem.Flip = flip;
            batchItem.Origin = origin;
            
            auto offsetX = charData->Bearing.X * scale.X;
            auto offsetY = (size - charData->Bearing.Y) * scale.Y;
            
            if (alignPosition)
            {
                if (xOffsetAll == -1)
                    xOffsetAll = offsetX;
            
                if (offsetY > 0 && (yOffsetAll == -1 || offsetY < yOffsetAll))
                    yOffsetAll = offsetY;
            }
            
            batchItem.Position.X = currentPosition.X + offsetX;
            batchItem.Position.Y = currentPosition.Y + offsetY;
            
            if (alignPosition)
                textBatch.push_back(batchItem);
            else
                _currentBatch.push_back(batchItem);
        }
        
        currentPosition.X += (charData->Advance >> 6) * scale.X; // bitshift advance by 6 to get value in pixels (2^6 = 64)
    }
    
    if (alignPosition)
    {
        for (auto &item : textBatch)
        {
            item.Position.X -= xOffsetAll;
            item.Position.Y -= yOffsetAll;
            _currentBatch.push_back(item);
        }
    }
}

void PandaSDL::SpriteBatch::AddQuadVertices(const SpriteBatchItem &item)
{
    auto texture = item.Texture;
    auto source = item.SourceRect;
    auto position = item.Position;
    auto color = item.Color.ToGLMVec4();
    auto rotation = item.Rotation;
    auto origin = item.Origin;

    auto texelWidth = texture->GetTexelWidth();
    auto texelHeight = texture->GetTexelHeight();

    glm::vec2 scale;
    scale.x = item.Scale.X * source.Width;
    scale.y = item.Scale.Y * source.Height;
    
    auto sin = 0.0f;
    auto cos = 0.0f;
    auto nOriginX = -origin.X;
    auto nOriginY = -origin.Y;
    
    if (rotation != 0.0f)
    {
        auto radians = glm::radians(rotation);
        sin = std::sin(radians);
        cos = std::cos(radians);
    }

    for (int i = 0; i < PANDASDL_QUAD_VERTEX_COUNT; i++)
    {
        glm::vec2 vertPosition;
        auto startIndex = i * 2;

        auto x = _tempVertexBuffer[startIndex];
        auto y = _tempVertexBuffer[startIndex + 1];
        
        auto w = (scale.x * x);
        auto h = (scale.y * y);

        glm::vec2 uv;

        if (i == (int)QuadVertexIndex::TOP_LEFT)
        {
            if (item.Flip == PandaSDL::eSpriteFlip::HORIZONTAL || item.Flip == PandaSDL::eSpriteFlip::BOTH)
                uv.x = (source.X + source.Width) * texelWidth;
            else
                uv.x = source.X * texelWidth;

            if (item.Flip == PandaSDL::eSpriteFlip::VERTICAL || item.Flip == PandaSDL::eSpriteFlip::BOTH)
                uv.y = (source.Y + source.Height) * texelHeight;
            else
                uv.y = source.Y * texelHeight;

            if (rotation == 0.0f)
            {
                vertPosition.x = (position.X - origin.X);
                vertPosition.y = (position.Y - origin.Y);
            }
            else
            {
                vertPosition.x = position.X + nOriginX * cos - nOriginY * sin;
                vertPosition.y = position.Y + nOriginX * sin + nOriginY * cos;
            }
        }
        else if (i == (int)QuadVertexIndex::TOP_RIGHT)
        {
            if (item.Flip == PandaSDL::eSpriteFlip::HORIZONTAL || item.Flip == PandaSDL::eSpriteFlip::BOTH)
                uv.x = source.X * texelWidth;
            else
                uv.x = (source.X + source.Width) * texelWidth;

            if (item.Flip == PandaSDL::eSpriteFlip::VERTICAL || item.Flip == PandaSDL::eSpriteFlip::BOTH)
                uv.y = (source.Y + source.Height) * texelHeight;
            else
                uv.y = source.Y * texelHeight;
            
            if (rotation == 0.0f)
            {
                vertPosition.x = (position.X - origin.X) + w;
                vertPosition.y = (position.Y - origin.Y);
            }
            else
            {
                vertPosition.x = position.X + (nOriginX + w) * cos - nOriginY * sin;
                vertPosition.y = position.Y + (nOriginX + w) * sin + nOriginY * cos;
            }
        }
        else if (i == (int)QuadVertexIndex::BOTTOM_LEFT)
        {
            if (item.Flip == PandaSDL::eSpriteFlip::HORIZONTAL || item.Flip == PandaSDL::eSpriteFlip::BOTH)
                uv.x = (source.X + source.Width) * texelWidth;
            else
                uv.x = source.X * texelWidth;

            if (item.Flip == PandaSDL::eSpriteFlip::VERTICAL || item.Flip == PandaSDL::eSpriteFlip::BOTH)
                uv.y = source.Y * texelHeight;
            else
                uv.y = (source.Y + source.Height) * texelHeight;
            
            if (rotation == 0.0f)
            {
                vertPosition.x = (position.X - origin.X);
                vertPosition.y = (position.Y - origin.Y) + h;
            }
            else
            {
                vertPosition.x = position.X + nOriginX * cos - (nOriginY + h) * sin;
                vertPosition.y = position.Y + nOriginX * sin + (nOriginY + h) * cos;
            }
        }
        else if (i == (int)QuadVertexIndex::BOTTOM_RIGHT)
        {
            if (item.Flip == PandaSDL::eSpriteFlip::HORIZONTAL || item.Flip == PandaSDL::eSpriteFlip::BOTH)
                uv.x = source.X * texelWidth;
            else
                uv.x = (source.X + source.Width) * texelWidth;

            if (item.Flip == PandaSDL::eSpriteFlip::VERTICAL || item.Flip == PandaSDL::eSpriteFlip::BOTH)
                uv.y = source.Y * texelHeight;
            else
                uv.y = (source.Y + source.Height) * texelHeight;
            
            if (rotation == 0.0f)
            {
                vertPosition.x = (position.X - origin.X) + w;
                vertPosition.y = (position.Y - origin.Y) + h;
            }
            else
            {
                vertPosition.x = position.X + (nOriginX + w) * cos - (nOriginY + h) * sin;
                vertPosition.y = position.Y + (nOriginX + w) * sin + (nOriginY + h) * cos;
            }
        }

        SpriteBatchVertex vertex;
        vertex.Position = vertPosition;
        vertex.TexCoords = uv;
        vertex.Colour = color;

        _batchVertices.push_back(vertex);
    }
}

void PandaSDL::SpriteBatch::End()
{
    if (!_begin)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::SPRITEBATCH_BEGIN, "Begin must be called before End.");

    bool firstTexture = true;
    std::shared_ptr<PandaSDL::Texture2D> prevTexture = nullptr;
    std::shared_ptr<PandaSDL::Texture2D> currentTexture = nullptr;

    _spriteShader->Use();
    _spriteShader->SetMatrix4("mProjectionView", _projection * _currentBatchTransform);
    
    PandaSDL::GraphicsPlatform::SetActiveTexture(0); // todo : handle multiple textures per draw

    for (const auto &batchItem : _currentBatch)
    {
        auto texture = batchItem.Texture;

        if (firstTexture || texture != currentTexture)
        {
            prevTexture = currentTexture;
            currentTexture = texture;

            Flush(prevTexture);

            firstTexture = false;
        }

        AddQuadVertices(batchItem);

        if ((_batchVertices.size() / PANDASDL_QUAD_VERTEX_COUNT) >= _maxBatchSize)
            Flush(currentTexture);
    }

    Flush(currentTexture);
    Clear();
}

void PandaSDL::SpriteBatch::Flush(std::shared_ptr<PandaSDL::Texture2D> texture)
{
    if (_batchVertices.size() <= 0)
        return;
    if (texture == nullptr)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::SPRITEBATCH_FLUSH, "Can't flush the SpriteBatch with a null texture.");

    _vao->Bind(true);
    
    texture->Bind();
    _vbo->BufferSubData(0, sizeof(SpriteBatchVertex) * _batchVertices.size(), &_batchVertices[0]);
    _ibo->DrawElements((_batchVertices.size() / PANDASDL_QUAD_VERTEX_COUNT) * PANDASDL_QUAD_INDEX_COUNT);
    
    _vao->Unbind(true);

    _batchVertices.clear();
}

void PandaSDL::SpriteBatch::Clear()
{
    _currentBatch.clear();
    _batchVertices.clear();
    _begin = false;
}

void PandaSDL::SpriteBatch::CheckDefaultShaders()
{
    if (PandaSDL::SpriteBatch::DefaultShadersInitialised)
        return;
    
    PandaSDL::SpriteBatch::DefaultSpriteShader = PandaSDL::Game::AssetManager.LoadShaderFromString("DefaultSpriteShader", PandaSDL::SpriteBatch::DefaultSpriteShaderVertexCode, PandaSDL::SpriteBatch::DefaultSpriteShaderFragmentCode);
    PandaSDL::SpriteBatch::DefaultFontShader = PandaSDL::Game::AssetManager.LoadShaderFromString("DefaultFontShader", PandaSDL::SpriteBatch::DefaultFontShaderVertexCode, PandaSDL::SpriteBatch::DefaultFontShaderFragmentCode);
    
    PandaSDL::SpriteBatch::DefaultShadersInitialised = true;
}