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
    glBufferData(_target, _size, data, _usage);
}

void PandaSDL::BufferObject::BufferSubData(int offset, int size, const void *data)
{
    glBufferSubData(_target, offset, size, data);
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