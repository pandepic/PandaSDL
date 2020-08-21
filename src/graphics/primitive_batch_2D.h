#ifndef __pandasdl_primitivebatch_h__
#define __pandasdl_primitivebatch_h__

#include "../../lib_include/GLEW/glew.h"
#include <GL/gl.h>

#include "../../lib_include/glm/glm.hpp"
#include "../../lib_include/glm/gtc/type_ptr.hpp"

#include "../general/globals.h"
#include "../math/rectangle.h"
#include "../math/vector2.h"
#include "shader.h"
#include "color.h"
#include "../general/game.h"
#include "graphics_platform_opengl.h"

namespace PandaSDL
{
    enum class ePrimitive2DType
    {
        RECTANGLE,
    };
    
    struct PrimitiveBase2D
    {
        PrimitiveBase2D(ePrimitive2DType type, PandaSDL::Color color, PandaSDL::Vector2 position) : Type(type), Color(color), Position(position) {}
        
        ePrimitive2DType Type;
        PandaSDL::Color Color;
        PandaSDL::Vector2 Position;
    };
    
    struct PrimitiveRectangle2D : public PrimitiveBase2D
    {
        PrimitiveRectangle2D(PandaSDL::Color color, PandaSDL::Vector2 position, PandaSDL::Rectangle rect) : PrimitiveBase2D(ePrimitive2DType::RECTANGLE, color, position), Rect(rect) {}
        
        PandaSDL::Rectangle Rect;
    };
    
    #pragma pack(push, 0)
    
    struct PrimitiveBatchVertex
    {
        glm::vec2 Position;
        glm::vec4 Colour;
    };
    
    #pragma pack(pop)
    
    class PrimitiveBatch2D
    {
        public:
            PrimitiveBatch2D();
            ~PrimitiveBatch2D();
            
            void Setup(
                int                         screenWidth,
                int                         screenHeight,
                bool                        invertY =           false,
                std::shared_ptr<Shader>     primitiveShader =   DefaultPrimitiveShader,
                unsigned int                maxBatchSize =      PANDASDL_DEFAULT_BATCH_SIZE);
            
            static std::shared_ptr<Shader> DefaultPrimitiveShader;
            static std::string DefaultPrimitiveShaderVertexCode;
            static std::string DefaultPrimitiveShaderFragmentCode;
            static bool DefaultShadersInitialised;
            
        protected:
            unsigned int _maxBatchSize;
            bool _initialised, _begin;
            std::unique_ptr<VertexArrayObject> _vao;
            std::shared_ptr<VertexBufferObject> _vbo;
            std::shared_ptr<Shader> _primitiveShader;
            glm::mat4 _projection;
            
            int _screenWidth, _screenHeight;
            static glm::mat4 _defaultTransform;
            
            void CheckDefaultShaders();
    };
}

#endif