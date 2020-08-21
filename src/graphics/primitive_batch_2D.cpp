#include "primitive_batch_2D.h"

std::shared_ptr<PandaSDL::Shader> PandaSDL::PrimitiveBatch2D::DefaultPrimitiveShader = nullptr;
std::string PandaSDL::PrimitiveBatch2D::DefaultPrimitiveShaderVertexCode =
"#version 330 core\n"
"layout (location = 0) in vec2 vPosition;\n"
"layout (location = 2) in vec4 vColour;\n"

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

"out vec4 color;\n"

"void main()\n"
"{\n"
"    color = fColour;\n"
"}\n";

bool PandaSDL::PrimitiveBatch2D::DefaultShadersInitialised = false;
glm::mat4 PandaSDL::PrimitiveBatch2D::_defaultTransform = glm::mat4(1.0f);

PandaSDL::PrimitiveBatch2D::PrimitiveBatch2D()
    : _initialised(false), _begin(false)
{
    CheckDefaultShaders();
}

PandaSDL::PrimitiveBatch2D::~PrimitiveBatch2D()
{
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
    
    _vao->VertexAttribPtrF(2, sizeof(PrimitiveBatchVertex), (void *)(offsetof(PrimitiveBatchVertex, Position)));
    _vao->VertexAttribPtrF(4, sizeof(PrimitiveBatchVertex), (void *)(offsetof(PrimitiveBatchVertex, Colour)));
    
    _vao->Unbind(true);
    
    _initialised = true;
}

void PandaSDL::PrimitiveBatch2D::CheckDefaultShaders()
{
    if (PandaSDL::PrimitiveBatch2D::DefaultShadersInitialised)
        return;
    
    PandaSDL::PrimitiveBatch2D::DefaultPrimitiveShader = PandaSDL::Game::AssetManager.LoadShaderFromString("DefaultPrimitiveShader", PandaSDL::PrimitiveBatch2D::DefaultPrimitiveShaderVertexCode, PandaSDL::PrimitiveBatch2D::DefaultPrimitiveShaderFragmentCode);
    PandaSDL::PrimitiveBatch2D::DefaultShadersInitialised = true;
}
