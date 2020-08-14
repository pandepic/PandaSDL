#include "spritebatch.h"

std::shared_ptr<PandaSDL::Shader> PandaSDL::Spritebatch::DefaultSpriteShader = nullptr;
std::string PandaSDL::Spritebatch::DefaultSpriteShaderVertexCode =
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

std::string PandaSDL::Spritebatch::DefaultSpriteShaderFragmentCode =
"#version 330 core\n"
"in vec2 fTexCoords;\n"
"in vec4 fColour;\n"

"out vec4 color;\n"

"uniform sampler2D image;\n"

"void main()\n"
"{\n"
"    color = texture(image, fTexCoords) * fColour;\n"
"}\n";

std::shared_ptr<PandaSDL::Shader> PandaSDL::Spritebatch::DefaultFontShader = nullptr;
std::string PandaSDL::Spritebatch::DefaultFontShaderVertexCode = PandaSDL::Spritebatch::DefaultSpriteShaderVertexCode;

std::string PandaSDL::Spritebatch::DefaultFontShaderFragmentCode =
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

glm::mat4 PandaSDL::Spritebatch::_defaultTransform = glm::mat4(1.0f);

PandaSDL::Spritebatch::Spritebatch()
: _begin(false), _initialised(false)
{
}

PandaSDL::Spritebatch::~Spritebatch()
{
    glDeleteVertexArrays(1, &_VAO);
    glDeleteBuffers(1, &_VBO);
}

void PandaSDL::Spritebatch::Setup(int screenWidth, int screenHeight, bool invertY, std::shared_ptr<Shader> spriteShader, unsigned int maxBatchSize)
{
    if (_initialised)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::SPRITEBATCH_SETUP, "Setup can only be called once per Spritebatch.");
    
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;
    _spriteShader = spriteShader;
    _maxBatchSize = maxBatchSize;

    if (invertY)
        _projection = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight, -1.0f, 1.0f);
    else
        _projection = glm::ortho(0.0f, (float)_screenWidth, (float)_screenHeight, 0.0f, -1.0f, 1.0f);

    _spriteShader->Use().SetInteger("image", 0);

    glActiveTexture(GL_TEXTURE0);

    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteBatchVertex), (void *)(offsetof(SpriteBatchVertex, Position)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteBatchVertex), (void *)(offsetof(SpriteBatchVertex, TexCoords)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteBatchVertex), (void *)(offsetof(SpriteBatchVertex, Colour)));
    glEnableVertexAttribArray(2);

    glBufferData(GL_ARRAY_BUFFER, sizeof(SpriteBatchVertex) * _maxBatchSize * PANDASDL_QUAD_VERTEX_COUNT, &_batchVertices[0], GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    float vertices[] = {
        // triangle 1
        0.0f, 1.0f, // bottom left
        1.0f, 0.0f, // top right
        0.0f, 0.0f, // top left

        // triangle 2
        0.0f, 1.0f, // bottom left
        1.0f, 1.0f, // bottom right
        1.0f, 0.0f, // top right
    };

    // bottom left, bottom right, top right, top left
    // 0, 2, 3, 0, 1, 2

    for (auto i = 0; i < PANDASDL_QUAD_VERTEX_FLOAT_COUNT; i++)
    {
        _tempVertexBuffer[i] = vertices[i];
    }

    _initialised = true;
}

void PandaSDL::Spritebatch::Begin(glm::mat4 transform)
{
    if (!_initialised)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::SPRITEBATCH_BEGIN, "Must call Setup before drawing.");
    if (_begin)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::SPRITEBATCH_BEGIN, "Call End on the current batch before using Begin again.");
    
    _currentBatchTransform = transform;
    _begin = true;
}

void PandaSDL::Spritebatch::Draw(std::shared_ptr<PandaSDL::Texture2D> texture, PandaSDL::Vector2 position, PandaSDL::Color color, Vector2 scale, float rotation, eSpriteFlip flip)
{
    Draw(texture, PandaSDL::Rectangle::Empty(), position, color, scale, rotation, flip);
}

void PandaSDL::Spritebatch::Draw(std::shared_ptr<PandaSDL::Texture2D> texture, PandaSDL::Rectangle sourceRect, PandaSDL::Vector2 position, PandaSDL::Color color, Vector2 scale, float rotation, eSpriteFlip flip)
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

    _currentBatch.push_back(batchItem);
}

void PandaSDL::Spritebatch::DrawText(std::shared_ptr<Spritefont> font, std::string text, unsigned int size, Vector2 position, Color color, bool alignPosition, Vector2 scale, float rotation, eSpriteFlip flip)
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

void PandaSDL::Spritebatch::AddQuadVertices(const SpriteBatchItem &item)
{
    auto texture = item.Texture;
    auto source = item.SourceRect;
    auto position = item.Position;
    auto color = item.Color.ToGLMVec4();
    auto rotation = item.Rotation;

    auto texelWidth = texture->GetTexelWidth();
    auto texelHeight = texture->GetTexelHeight();

    glm::vec2 scale;
    scale.x = item.Scale.X * source.Width;
    scale.y = item.Scale.Y * source.Height;

    glm::mat4 model = glm::mat4(1.0f);

    if (rotation != 0.0f)
    {
        model = glm::translate(model, glm::vec3(position.X, position.Y, 1.0f));
        model = glm::translate(model, glm::vec3(0.5f * scale.x, 0.5f * scale.y, 0.0f));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f));
        model = glm::scale(model, glm::vec3(scale, 1.0f));
    }

    for (auto i = 0; i < 6; i++)
    {
        glm::vec2 vertPosition;
        auto startIndex = i * 2;

        auto x = _tempVertexBuffer[startIndex];
        auto y = _tempVertexBuffer[startIndex + 1];

        glm::vec2 uv;

        if (i == QuadVertexIndex::TOP_LEFT)
        {
            if (item.Flip == PandaSDL::eSpriteFlip::HORIZONTAL || item.Flip == PandaSDL::eSpriteFlip::BOTH)
                uv.x = (source.X + source.Width) * texelWidth;
            else
                uv.x = source.X * texelWidth;

            if (item.Flip == PandaSDL::eSpriteFlip::VERTICAL || item.Flip == PandaSDL::eSpriteFlip::BOTH)
                uv.y = (source.Y + source.Height) * texelHeight;
            else
                uv.y = source.Y * texelHeight;

            vertPosition.x = position.X;
            vertPosition.y = position.Y;
        }
        else if (i == QuadVertexIndex::TOP_RIGHT || i == QuadVertexIndex::TOP_RIGHT_2)
        {
            if (item.Flip == PandaSDL::eSpriteFlip::HORIZONTAL || item.Flip == PandaSDL::eSpriteFlip::BOTH)
                uv.x = source.X * texelWidth;
            else
                uv.x = (source.X + source.Width) * texelWidth;

            if (item.Flip == PandaSDL::eSpriteFlip::VERTICAL || item.Flip == PandaSDL::eSpriteFlip::BOTH)
                uv.y = (source.Y + source.Height) * texelHeight;
            else
                uv.y = source.Y * texelHeight;

            vertPosition.x = position.X + (scale.x * x);
            vertPosition.y = position.Y;
        }
        else if (i == QuadVertexIndex::BOTTOM_LEFT || i == QuadVertexIndex::BOTTOM_LEFT_2)
        {
            if (item.Flip == PandaSDL::eSpriteFlip::HORIZONTAL || item.Flip == PandaSDL::eSpriteFlip::BOTH)
                uv.x = (source.X + source.Width) * texelWidth;
            else
                uv.x = source.X * texelWidth;

            if (item.Flip == PandaSDL::eSpriteFlip::VERTICAL || item.Flip == PandaSDL::eSpriteFlip::BOTH)
                uv.y = source.Y * texelHeight;
            else
                uv.y = (source.Y + source.Height) * texelHeight;

            vertPosition.x = position.X;
            vertPosition.y = position.Y + (scale.y * y);
        }
        else if (i == QuadVertexIndex::BOTTOM_RIGHT)
        {
            if (item.Flip == PandaSDL::eSpriteFlip::HORIZONTAL || item.Flip == PandaSDL::eSpriteFlip::BOTH)
                uv.x = source.X * texelWidth;
            else
                uv.x = (source.X + source.Width) * texelWidth;

            if (item.Flip == PandaSDL::eSpriteFlip::VERTICAL || item.Flip == PandaSDL::eSpriteFlip::BOTH)
                uv.y = source.Y * texelHeight;
            else
                uv.y = (source.Y + source.Height) * texelHeight;

            vertPosition.x = position.X + (scale.x * x);
            vertPosition.y = position.Y + (scale.y * y);
        }

        if (rotation != 0.0f)
        {
            auto transformVec = glm::vec4(x, y, 0, 1);
            transformVec = model * transformVec;
            vertPosition.x = transformVec.x;
            vertPosition.y = transformVec.y;
        }

        SpriteBatchVertex vertex;
        vertex.Position = vertPosition;
        vertex.TexCoords = uv;
        vertex.Colour = color;

        _batchVertices.push_back(vertex);
    }
}

void PandaSDL::Spritebatch::End()
{
    if (!_begin)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::SPRITEBATCH_BEGIN, "Begin must be called before End.");

    bool firstTexture = true;
    auto prevTextureID = 0;
    auto textureID = 0;

    _spriteShader->Use();
    _spriteShader->SetMatrix4("mProjectionView", _projection * _currentBatchTransform);

    for (const auto &batchItem : _currentBatch)
    {
        auto texture = batchItem.Texture;

        if (firstTexture || texture->GetTextureID() != textureID)
        {
            prevTextureID = textureID;
            textureID = texture->GetTextureID();

            Flush(prevTextureID);

            firstTexture = false;
        }

        AddQuadVertices(batchItem);

        if ((_batchVertices.size() / PANDASDL_QUAD_VERTEX_COUNT) >= _maxBatchSize)
            Flush(textureID);
    }

    Flush(textureID);
    Clear();
}

void PandaSDL::Spritebatch::Flush(int texture)
{
    if (_batchVertices.size() <= 0)
        return;

    glBindVertexArray(_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(SpriteBatchVertex) * _batchVertices.size(), &_batchVertices[0]);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, _batchVertices.size());

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    _batchVertices.clear();
}

void PandaSDL::Spritebatch::Clear()
{
    _currentBatch.clear();
    _batchVertices.clear();
    _begin = false;
}
