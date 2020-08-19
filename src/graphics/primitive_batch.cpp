#include "primitive_batch.h"

std::shared_ptr<PandaSDL::Shader> PandaSDL::PrimitiveBatch::DefaultPrimitiveShader = nullptr;
std::string PandaSDL::PrimitiveBatch::DefaultPrimitiveShaderVertexCode =
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

std::string PandaSDL::PrimitiveBatch::DefaultPrimitiveShaderFragmentCode =
"#version 330 core\n"
"in vec4 fColour;\n"

"out vec4 color;\n"

"void main()\n"
"{\n"
"    color = fColour;\n"
"}\n";

PandaSDL::PrimitiveBatch::PrimitiveBatch()
{
}

PandaSDL::PrimitiveBatch::~PrimitiveBatch()
{
}
