#ifndef __pandasdl_spritebatch_h__
#define __pandasdl_spritebatch_h__

#include "../../lib_include/GLEW/glew.h"
#include <GL/gl.h>

#include "../../lib_include/glm/glm.hpp"
#include "../../lib_include/glm/gtc/type_ptr.hpp"

#include "primitive_batch_types.h"
#include "../general/globals.h"
#include "../math/rectangle.h"
#include "../math/vector2.h"
#include "shader.h"
#include "color.h"

namespace PandaSDL
{
    #pragma pack(push, 0)
    
    struct PrimitiveBatchVertex
    {
        glm::vec2 Position;
        glm::vec4 Colour;
    };
    
    #pragma pack(pop)
    
    class PrimitiveBatch
    {
        public:
            PrimitiveBatch();
            ~PrimitiveBatch();
            
            static std::shared_ptr<Shader> DefaultPrimitiveShader;
            
            static std::string DefaultPrimitiveShaderVertexCode;
            static std::string DefaultPrimitiveShaderFragmentCode;
            
        protected:
            unsigned int _maxBatchSize;
            bool _initialised, _begin;
            GLuint _VAO, _VBO, _IBO;
    };
}

#endif