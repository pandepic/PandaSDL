#include "primitive_batch_2D.h"

std::shared_ptr<PandaSDL::Shader> PandaSDL::PrimitiveBatch2D::DefaultPrimitiveShader = nullptr;
std::string PandaSDL::PrimitiveBatch2D::DefaultPrimitiveShaderVertexCode =
"#version 330 core\n"
"layout (location = 0) in vec2 vPosition;\n"
"layout (location = 1) in vec4 vColour;\n"

"out vec4 fColour;\n"

"uniform mat4 mProjectionView;\n"

"void main()\n"
"{\n"
"    fColour = vColour;\n"
"    gl_Position = mProjectionView * vec4(vPosition.x, vPosition.y, 0.0, 1.0);\n"
"}\n";

std::string PandaSDL::PrimitiveBatch2D::DefaultPrimitiveShaderFragmentCode =
"#version 330 core\n"
"in vec4 fColour;\n"

"out vec4 fFragColour;\n"

"void main()\n"
"{\n"
"    fFragColour = fColour;\n"
"}\n";

bool PandaSDL::PrimitiveBatch2D::DefaultShadersInitialised = false;
glm::mat4 PandaSDL::PrimitiveBatch2D::_defaultTransform = glm::mat4(1.0f);

void PandaSDL::PrimitiveRectangle2D::AddBatchVertices(std::vector<PrimitiveBatchVertex> &batchVertices) const
{
    float vertices[] = {
        0.0f, 1.0f, // bottom left (0)
        1.0f, 0.0f, // top right (1)
        0.0f, 0.0f, // top left (2)
        0.0f, 1.0f, // bottom left (0)
        1.0f, 1.0f, // bottom right (3)
        1.0f, 0.0f, // top right (1)
    };
    
    auto color = Color.ToGLMVec4();
    
    glm::vec2 scale;
    scale.x = Rect.Width;
    scale.y = Rect.Height;
    
    glm::mat4 model = glm::mat4(1.0f);

    // temporary slow rotation
    // todo : optimise this to use a mat3x2 or do rotation math directly?
    if (Rotation != 0.0f)
    {
        model = glm::translate(model, glm::vec3(Position.X, Position.Y, 1.0f));
        model = glm::translate(model, glm::vec3(Centre.X, Centre.Y, 0.0f));
        model = glm::rotate(model, glm::radians(Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-Centre.X, -Centre.Y, 0.0f));
        model = glm::scale(model, glm::vec3(scale, 1.0f));
    }
    
    for (int i = 0; i < 6; i++)
    {
        glm::vec2 vertPosition;
        auto startIndex = i * 2;

        auto x = vertices[startIndex];
        auto y = vertices[startIndex + 1];
        
        switch (i)
        {
            case ePrimitiveRectangle2DVertex::RECTVERT_TOPLEFT:
            {
                vertPosition.x = Rect.X;
                vertPosition.y = Rect.Y;
            }
            break;
            
            case ePrimitiveRectangle2DVertex::RECTVERT_TOPRIGHT:
            case ePrimitiveRectangle2DVertex::RECTVERT_TOPRIGHT2:
            {
                vertPosition.x = Rect.X + Rect.Width;
                vertPosition.y = Rect.Y;
            }
            break;
            
            case ePrimitiveRectangle2DVertex::RECTVERT_BOTTOMLEFT:
            case ePrimitiveRectangle2DVertex::RECTVERT_BOTTOMLEFT2:
            {
                vertPosition.x = Rect.X;
                vertPosition.y = Rect.Y + Rect.Height;
            }
            break;
            
            case ePrimitiveRectangle2DVertex::RECTVERT_BOTTOMRIGHT:
            {
                vertPosition.x = Rect.X + Rect.Width;
                vertPosition.y = Rect.Y + Rect.Height;
            }
            break;
        }
        
        // temporary slow rotation
        // todo : optimise this to use a mat3x2 or do rotation math directly?
        if (Rotation != 0.0f)
        {
            auto transformVec = glm::vec4(x, y, 0, 1);
            transformVec = model * transformVec;
            vertPosition.x = transformVec.x;
            vertPosition.y = transformVec.y;
        }
        
        PrimitiveBatchVertex vertex;
        vertex.Position = vertPosition;
        vertex.Colour = color;
        
        batchVertices.push_back(vertex);
    }
}

void PandaSDL::PrimitiveCircle2D::AddBatchVertices(std::vector<PrimitiveBatchVertex> &batchVertices) const
{
    auto color = Color.ToGLMVec4();
    auto numVertices = VertexCount;
    
    for(double i = 0; i < 2 * M_PI; i += 2 * M_PI / numVertices){
        PrimitiveBatchVertex vertex;
        vertex.Position = { cos(i) * Radius + Position.X, sin(i) * Radius + Position.Y };
        vertex.Colour = color;
        
        batchVertices.push_back(vertex);
    }
}

PandaSDL::PrimitiveBatch2D::PrimitiveBatch2D()
    : _initialised(false), _begin(false), _batchSize(0)
{
    CheckDefaultShaders();
}

PandaSDL::PrimitiveBatch2D::~PrimitiveBatch2D()
{
    if (_initialised)
        Clear();
}

void PandaSDL::PrimitiveBatch2D::Setup(int screenWidth, int screenHeight, bool invertY, std::shared_ptr<Shader> primitiveShader, unsigned int maxBatchSize)
{
    if (_initialised)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::PRIMITIVEBATCH_SETUP, "Setup can only be called once per PrimitiveBatch.");
    
    _screenWidth = screenWidth;
    _screenHeight = screenHeight;
    _primitiveShader = primitiveShader;
    _maxBatchSize = maxBatchSize;
    
    if (invertY)
        _projection = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight, -1.0f, 1.0f);
    else
        _projection = glm::ortho(0.0f, (float)_screenWidth, (float)_screenHeight, 0.0f, -1.0f, 1.0f);
    
    _vao = std::make_unique<VertexArrayObject>(true);
    _vbo = _vao->AddVertexBufferObject(sizeof(PrimitiveBatchVertex) * _maxBatchSize * PANDASDL_PRIMITIVE_QUAD_VERTEX_COUNT, nullptr, GL_DYNAMIC_DRAW, true);
    _vao->ApplyVertexAttribTypeMapF(PrimitiveBatchVertex::VertexAttribMap());
    _vao->Unbind(true);
    
    _initialised = true;
}

void PandaSDL::PrimitiveBatch2D::Begin(glm::mat4 transform)
{
    if (!_initialised)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::PRIMITIVEBATCH_BEGIN, "Must call Setup before drawing.");
    if (_begin)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::PRIMITIVEBATCH_BEGIN, "Call End on the current batch before using Begin again.");
    
    _currentBatchTransform = transform;
    _begin = true;
}

void PandaSDL::PrimitiveBatch2D::DrawRectangle(PandaSDL::Rectangle rect, PandaSDL::Color color, float rotation, bool outline, unsigned int outlineSize, PandaSDL::Color outlineColor)
{
    if (!_begin)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::PRIMITIVEBATCH_DRAW, "Begin must be called before DrawRectangle.");
    
    DrawFilledRectangle(rect, color, rotation);
    
    if (outline)
        DrawEmptyRectangle(rect, outlineColor, outlineSize, rotation);
}

void PandaSDL::PrimitiveBatch2D::DrawFilledRectangle(PandaSDL::Rectangle rect, PandaSDL::Color color, float rotation)
{
    DrawFilledRectangle(rect, color, { (float)rect.Width * 0.5f, (float)rect.Height * 0.5f }, rotation);
}

void PandaSDL::PrimitiveBatch2D::DrawFilledRectangle(PandaSDL::Rectangle rect, PandaSDL::Color color, PandaSDL::Vector2 centre, float rotation)
{
    if (!_begin)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::PRIMITIVEBATCH_DRAW, "Begin must be called before DrawFilledRectangle.");
    
    auto newItem = new PrimitiveRectangle2D(rect, color);
    newItem->VertexCount = 6;
    newItem->Rotation = rotation;
    newItem->Centre = centre;
    
    _currentBatch.push_back(newItem);
}

void PandaSDL::PrimitiveBatch2D::DrawEmptyRectangle(PandaSDL::Rectangle rect, PandaSDL::Color color, unsigned int lineSize, float rotation)
{
    if (!_begin)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::PRIMITIVEBATCH_DRAW, "Begin must be called before DrawEmptyRectangle.");
    
    auto centre = PandaSDL::Vector2((float)rect.Width * 0.5f, (float)rect.Height * 0.5f);
    
    // top
    DrawFilledRectangle(PandaSDL::Rectangle(rect.X, rect.Y, rect.Width, lineSize), color, centre, rotation);
    // bottom
    DrawFilledRectangle(PandaSDL::Rectangle(rect.X, rect.Y + (rect.Height - lineSize), rect.Width, lineSize), color, centre, rotation);
    // left
    DrawFilledRectangle(PandaSDL::Rectangle(rect.X, rect.Y + lineSize, lineSize, (rect.Height - lineSize * 2)), color, centre, rotation);
    // right
    DrawFilledRectangle(PandaSDL::Rectangle(rect.X + (rect.Width - lineSize), rect.Y + lineSize, lineSize, (rect.Height - lineSize * 2)), color, centre, rotation);
}

void PandaSDL::PrimitiveBatch2D::DrawEmptyCircle(PandaSDL::Vector2 position, float radius, PandaSDL::Color color, eCircleQuality quality)
{
    if (!_begin)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::PRIMITIVEBATCH_DRAW, "Begin must be called before DrawFilledCircle.");
    
    auto qualityFactor = quality == eCircleQuality::HIGH ? 10 : 20;
    
    auto newItem = new PrimitiveCircle2D(position, radius, color);
    newItem->Centre = position;
    newItem->DrawMode = GL_LINE_LOOP;
    newItem->VertexCount = 16;
    newItem->Filled = false;
    newItem->VertexCount = (int)(radius / qualityFactor) * 8;
    
    _currentBatch.push_back(newItem);
}

void PandaSDL::PrimitiveBatch2D::DrawFilledCircle(PandaSDL::Vector2 position, float radius, PandaSDL::Color color, eCircleQuality quality)
{
    if (!_begin)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::PRIMITIVEBATCH_DRAW, "Begin must be called before DrawFilledCircle.");
    
    auto qualityFactor = quality == eCircleQuality::HIGH ? 10 : 20;
    
    auto newItem = new PrimitiveCircle2D(position, radius, color);
    newItem->Centre = position;
    newItem->VertexCount = 16;
    newItem->Filled = true;
    newItem->VertexCount = (int)(radius / qualityFactor) * 8;
    
    _currentBatch.push_back(newItem);
}

void PandaSDL::PrimitiveBatch2D::End()
{
    if (!_begin)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::PRIMITIVEBATCH_BEGIN, "Begin must be called before End.");

    _primitiveShader->Use();
    _primitiveShader->SetMatrix4("mProjectionView", _projection * _currentBatchTransform);
    
    unsigned int currentDrawMode = -1;
    
    for (const auto &batchItem : _currentBatch)
    {
        if (currentDrawMode == -1)
        {
            currentDrawMode = batchItem->DrawMode;
        }
        else
        {
            if (currentDrawMode != batchItem->DrawMode)
            {
                Flush(currentDrawMode);
                currentDrawMode = batchItem->DrawMode;
            }
        }
        
        if (_batchVertices.size() + batchItem->VertexCount >= _maxBatchSize)
            Flush(currentDrawMode);
        
        batchItem->AddBatchVertices(_batchVertices);
        _batchSize += 1;
    }
    
    Flush(currentDrawMode);
    Clear();
}

void PandaSDL::PrimitiveBatch2D::Clear()
{
    for (int i = 0; i < _currentBatch.size(); i++)
        delete _currentBatch[i];
    
    _currentBatch.clear();
    _batchVertices.clear();
    _begin = false;
    _batchSize = 0;
}

void PandaSDL::PrimitiveBatch2D::Flush(unsigned int drawMode)
{
    if (_batchVertices.size() <= 0)
        return;
    if (drawMode == -1)
        return;
    
    _vao->Bind(true);
    
    _vbo->BufferSubData(0, sizeof(PrimitiveBatchVertex) * _batchVertices.size(), &_batchVertices[0]);
    _vbo->DrawArrays(0, _batchVertices.size(), drawMode);
    
    _vao->Unbind(true);

    _batchVertices.clear();
    _batchSize = 0;
}

void PandaSDL::PrimitiveBatch2D::CheckDefaultShaders()
{
    if (PandaSDL::PrimitiveBatch2D::DefaultShadersInitialised)
        return;
    
    PandaSDL::PrimitiveBatch2D::DefaultPrimitiveShader = PandaSDL::Game::AssetManager.LoadShaderFromString("DefaultPrimitiveShader", PandaSDL::PrimitiveBatch2D::DefaultPrimitiveShaderVertexCode, PandaSDL::PrimitiveBatch2D::DefaultPrimitiveShaderFragmentCode);
    PandaSDL::PrimitiveBatch2D::DefaultShadersInitialised = true;
}
