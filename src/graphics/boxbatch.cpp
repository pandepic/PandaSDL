#include "boxbatch.h"

std::shared_ptr<PandaSDL::Shader> PandaSDL::Boxbatch::DefaultBoxShader = nullptr;
std::string PandaSDL::Boxbatch::DefaultBoxShaderVertexCode =
"#version 330 core\n"
"layout (location = 0) in vec3 vPosition;\n"
"layout (location = 1) in vec2 vTexCoords;\n"
"layout (location = 2) in vec4 vColour;\n"
"layout (location = 3) in mat4 mTransform;\n"

"out vec2 fTexCoords;\n"
"out vec4 fColour;\n"

"uniform mat4 mProjectionView;\n"

"void main()\n"
"{\n"
"    fTexCoords = vTexCoords;\n"
"    fColour = vColour;\n"
"    gl_Position = mProjectionView * mTransform * vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);\n"
"}\n";

std::string PandaSDL::Boxbatch::DefaultBoxShaderFragmentCode =
"#version 330 core\n"
"in vec2 fTexCoords;\n"
"in vec4 fColour;\n"

"out vec4 color;\n"

"uniform sampler2D image;\n"

"void main()\n"
"{\n"
"    color = texture(image, fTexCoords) * fColour;\n"
"}\n";

PandaSDL::Boxbatch::Boxbatch()
{
}

PandaSDL::Boxbatch::~Boxbatch()
{
}
