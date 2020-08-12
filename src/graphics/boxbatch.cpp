#include "boxbatch.h"

std::shared_ptr<PandaSDL::Shader> PandaSDL::Boxbatch::DefaultBoxShader = nullptr;
std::string PandaSDL::Boxbatch::DefaultBoxShaderVertexCode = "";

std::string PandaSDL::Boxbatch::DefaultBoxShaderFragmentCode = "";

PandaSDL::Boxbatch::Boxbatch()
{
}

PandaSDL::Boxbatch::~Boxbatch()
{
}
