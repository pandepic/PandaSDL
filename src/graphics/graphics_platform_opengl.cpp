#include "graphics_platform_opengl.h"

PandaSDL::BufferObject::BufferObject(unsigned int target, int size, const void *data, unsigned int usage)
{
    _target = target;
    _usage = usage;
    
    glGenBuffers(1, &_id);
    Bind();
    BufferData(size, data);
    Unbind();
}

PandaSDL::BufferObject::~BufferObject()
{
    glDeleteBuffers(1, &_id);
}

void PandaSDL::BufferObject::Bind()
{
    glBindBuffer(_target, _id);
    _bound = true;
}

void PandaSDL::BufferObject::Unbind()
{
    glBindBuffer(_target, 0);
    _bound = false;
}

void PandaSDL::BufferObject::BufferData(int size, const void *data)
{
    _size = size;
    glNamedBufferData(_id, _size, data, _usage);
}

void PandaSDL::BufferObject::BufferSubData(int offset, int size, const void *data)
{
    glNamedBufferSubData(_id, offset, size, data);
}

unsigned int PandaSDL::BufferObject::GetID()
{
    return _id;
}

bool PandaSDL::BufferObject::GetBound()
{
    return _bound;
}

void PandaSDL::VertexBufferObject::DrawArrays(int first, int count, unsigned int mode)
{
    glDrawArrays(mode, first, count);
}

void PandaSDL::IndexBufferObject::DrawElements(int count, const void *indices, unsigned int mode, unsigned int type)
{
    if (!_bound)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::GRAPHICS_BUFFER, "Must bind the index buffer before drawing with it.");
    
    glDrawElements(mode, count, type, indices);
}

PandaSDL::VertexArrayObject::VertexArrayObject(bool bind)
    : _attribPtrIndex(0)
{
    glGenVertexArrays(1, &_id);
    
    if (bind)
        Bind();
}

PandaSDL::VertexArrayObject::~VertexArrayObject()
{
    glDeleteVertexArrays(1, &_id);
}

void PandaSDL::VertexArrayObject::Bind(bool bindBuffers)
{
    glBindVertexArray(_id);
    _bound = true;
    
    if (bindBuffers)
    {
        for (auto &buffer : _buffers)
            buffer->Bind();
    }
}

void PandaSDL::VertexArrayObject::Unbind(bool unbindBuffers)
{
    if (unbindBuffers)
    {
        for (auto &buffer : _buffers)
            buffer->Unbind();
    }
    
    glBindVertexArray(0);
    _bound = false;
}

void PandaSDL::VertexArrayObject::VertexAttribPtrF(int size, int stride, const void *offsetPtr, bool normalize)
{
    glVertexAttribPointer(_attribPtrIndex, size, GL_FLOAT, normalize ? GL_TRUE : GL_FALSE, stride, offsetPtr);
    glEnableVertexAttribArray(_attribPtrIndex);
    _attribPtrIndex += 1;
}

void PandaSDL::VertexArrayObject::ApplyVertexAttribTypeMapF(const std::vector<VertexAttribTypeMapFVar> &map)
{
    intptr_t offset = 0;
    unsigned int typeSize = 0;
    
    for (const auto &var : map)
    {
        typeSize += sizeof(float) * var.FloatCount;
    }
    
    for (const auto &var : map)
    {
        glVertexAttribPointer(_attribPtrIndex, var.FloatCount, GL_FLOAT, var.Normalize ? GL_TRUE : GL_FALSE, typeSize, (void *)offset);
        glEnableVertexAttribArray(_attribPtrIndex);
        _attribPtrIndex += 1;
        offset += sizeof(float) * var.FloatCount;
    }
}

std::shared_ptr<PandaSDL::VertexBufferObject> PandaSDL::VertexArrayObject::AddVertexBufferObject(int size, const void *data, unsigned int hint, bool bind)
{
    auto newVBO = std::make_shared<PandaSDL::VertexBufferObject>(size, data, hint);
    _buffers.push_back(newVBO);
    
    if (bind)
        newVBO->Bind();
    
    return newVBO;
}

std::shared_ptr<PandaSDL::IndexBufferObject> PandaSDL::VertexArrayObject::AddIndexBufferObject(int size, const void *data, unsigned int hint, bool bind)
{
    auto newIBO = std::make_shared<PandaSDL::IndexBufferObject>(size, data, hint);
    _buffers.push_back(newIBO);
    
    if (bind)
        newIBO->Bind();
    
    return newIBO;
}

unsigned int PandaSDL::VertexArrayObject::GetID()
{
    return _id;
}

bool PandaSDL::VertexArrayObject::GetBound()
{
    return _bound;
}

void PandaSDL::GraphicsPlatform::SetActiveTexture(int index)
{
    glActiveTexture(GL_TEXTURE0 + index);
}

void PandaSDL::GraphicsPlatform::Clear(unsigned int mask)
{
    glClear(mask);
}

void PandaSDL::GraphicsPlatform::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void PandaSDL::GraphicsPlatform::EnableFeature(unsigned int feature)
{
    glEnable(feature);
}

void PandaSDL::GraphicsPlatform::SetBlendFunc(unsigned int sfactor, unsigned int dfactor)
{
    glBlendFunc(sfactor, dfactor);
}

void PandaSDL::GraphicsPlatform::SetDepthFunc(unsigned int func)
{
    glDepthFunc(func);
}

void PandaSDL::GraphicsPlatform::SetViewport(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
}

void PandaSDL::GraphicsPlatform::SetPixelStoragePackAlignment(int value)
{
    glPixelStorei(GL_PACK_ALIGNMENT, value);
}

void PandaSDL::GraphicsPlatform::SetPixelStorageUnpackAlignment(int value)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, value);
}

std::string GetVersion()
{
    return std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION)));
}

void PandaSDL::GraphicsPlatform::EnableDebugOutput()
{
    EnableFeature(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLErrorMessageCallback, 0);
}

void GLAPIENTRY PandaSDL::GraphicsPlatform::GLErrorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}
