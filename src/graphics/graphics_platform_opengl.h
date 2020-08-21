#ifndef __pandasdl_graphicstypes_h__
#define __pandasdl_graphicstypes_h__

#include "../../lib_include/GLEW/glew.h"
#include <GL/gl.h>

#include "../../lib_include/glm/glm.hpp"
#include "../../lib_include/glm/gtc/type_ptr.hpp"

#include "../general/globals.h"

namespace PandaSDL
{
    // used to map data from a struct to vertex attrib pointer where all variables in the struct are floats
    // eg. static std::vector<VertexAttribTypeMapFVar> VertexAttribMap() { return { 2, 2, 4 }; } for a struct with vec2, vec2, vec4
    struct VertexAttribTypeMapFVar
    {
        VertexAttribTypeMapFVar(unsigned int floatCount, bool normalize = false) : FloatCount(floatCount), Normalize(normalize) {}
        
        unsigned int FloatCount;
        bool Normalize;
    };
    
    class BufferObject
    {
        public:
            BufferObject(unsigned int target, int size, const void *data = nullptr, unsigned int usage = GL_STATIC_DRAW);
            virtual ~BufferObject();
            
            void Bind();
            void Unbind();
            void BufferData(int size, const void* data);
            void BufferSubData(int offset, int size, const void *data);
            
            unsigned int GetID();
            bool GetBound();
            
        protected:
            unsigned int _id;
            unsigned int _target, _usage;
            int _size;
            bool _bound;
    };
    
    class VertexBufferObject : public BufferObject
    {
        public:
            VertexBufferObject(int size, const void *data = nullptr, unsigned int hint = GL_STATIC_DRAW) : BufferObject(GL_ARRAY_BUFFER, size, data, hint) {}
            ~VertexBufferObject() {}
            
            void DrawArrays(int first, int count, unsigned int mode = GL_TRIANGLES);
    };
    
    class IndexBufferObject : public BufferObject
    {
        public:
            IndexBufferObject(int size, const void* data = nullptr, unsigned int hint = GL_STATIC_DRAW) : BufferObject(GL_ELEMENT_ARRAY_BUFFER, size, data, hint) {}
            ~IndexBufferObject() {}
            
            void DrawElements(int count, const void *indices = nullptr, unsigned int mode = GL_TRIANGLES, unsigned int type = GL_UNSIGNED_INT);
    };
    
    class VertexArrayObject
    {
        public:
            VertexArrayObject(bool bind = false);
            ~VertexArrayObject();
            
            void Bind(bool bindBuffers = false);
            void Unbind(bool unbindBuffers = false);
            void VertexAttribPtrF(int size, int stride, const void *offsetPtr, bool normalize = false);
            void ApplyVertexAttribTypeMapF(const std::vector<VertexAttribTypeMapFVar> &map); // map a struct where all variables are floats
            
            std::shared_ptr<PandaSDL::VertexBufferObject> AddVertexBufferObject(int size, const void *data = nullptr, unsigned int hint = GL_STATIC_DRAW, bool bind = false);
            std::shared_ptr<PandaSDL::IndexBufferObject> AddIndexBufferObject(int size, const void *data = nullptr, unsigned int hint = GL_STATIC_DRAW, bool bind = false);
            
            unsigned int GetID();
            bool GetBound();
            
        protected:
            unsigned int _id;
            unsigned int _attribPtrIndex;
            bool _bound;
            
            std::vector<std::shared_ptr<BufferObject>> _buffers;
    };
    
    class GraphicsPlatform
    {
        public:
            static void SetActiveTexture(int index);
            static void Clear(unsigned int mask);
            static void SetClearColor(float r, float g, float b, float a);
            static void EnableFeature(unsigned int feature);
            static void SetBlendFunc(unsigned int sfactor, unsigned int dfactor);
            static void SetDepthFunc(unsigned int func);
            static void SetViewport(int x, int y, int width, int height);
            static void SetPixelStoragePackAlignment(int value);
            static void SetPixelStorageUnpackAlignment(int value);
            static std::string GetVersion();
            
            static void EnableDebugOutput();
            static void GLAPIENTRY GLErrorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
    };
}

#endif