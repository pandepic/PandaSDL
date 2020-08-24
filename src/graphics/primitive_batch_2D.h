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
        CIRCLE
    };
    
    enum class eCircleQuality
    {
        HIGH,
        LOW
    };
    
    #pragma pack(push, 0)
    
    struct PrimitiveBatchVertex
    {
        glm::vec2 Position;
        glm::vec4 Colour;
        
        static std::vector<VertexAttribTypeMapFVar> VertexAttribMap() { return { 2, 4 }; }
    };
    
    #pragma pack(pop)
    
    struct PrimitiveBase2D
    {
        PrimitiveBase2D(ePrimitive2DType type, PandaSDL::Vector2 position, PandaSDL::Color color, unsigned int drawMode)
        : Type(type), Color(color), Position(position), Centre(position), VertexCount(0), DrawMode(drawMode) {}
        
        ePrimitive2DType Type;
        PandaSDL::Color Color;
        PandaSDL::Vector2 Position;
        float Rotation;
        PandaSDL::Vector2 Centre;
        unsigned int VertexCount;
        unsigned int DrawMode;
        
        virtual void AddBatchVertices(std::vector<PrimitiveBatchVertex> &batchVertices) const {}
    };
    
    struct PrimitiveRectangle2D : public PrimitiveBase2D
    {
        enum ePrimitiveRectangle2DVertex
        {
            RECTVERT_BOTTOMLEFT,
            RECTVERT_TOPRIGHT,
            RECTVERT_TOPLEFT,
            RECTVERT_BOTTOMLEFT2,
            RECTVERT_BOTTOMRIGHT,
            RECTVERT_TOPRIGHT2,
        };
        
        PrimitiveRectangle2D(PandaSDL::Rectangle rect, PandaSDL::Color color)
        : PrimitiveBase2D(ePrimitive2DType::RECTANGLE, { rect.X, rect.Y }, color, GL_TRIANGLES), Rect(rect) {}
        
        PandaSDL::Rectangle Rect;
        
        virtual void AddBatchVertices(std::vector<PrimitiveBatchVertex> &batchVertices) const override;
    };
    
    struct PrimitiveCircle2D : public PrimitiveBase2D
    {
        PrimitiveCircle2D(PandaSDL::Vector2 position, float radius, PandaSDL::Color color)
        : PrimitiveBase2D(ePrimitive2DType::CIRCLE, position, color, GL_TRIANGLE_FAN), Filled(false), Radius(radius) {}
        
        bool Filled;
        float Radius;
        
        virtual void AddBatchVertices(std::vector<PrimitiveBatchVertex> &batchVertices) const override;
    };
    
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
                unsigned int                maxBatchSize =      PANDASDL_DEFAULT_PRIMITIVEBATCH_SIZE);
            
            void Begin(glm::mat4 transform = _defaultTransform);
            
            void DrawRectangle(PandaSDL::Rectangle rect, PandaSDL::Color color, float rotation = 0.0f, bool outline = false, unsigned int outlineSize = 1, PandaSDL::Color outlineColor = PANDASDL_COLOR_WHITE);
            void DrawFilledRectangle(PandaSDL::Rectangle rect, PandaSDL::Color color, float rotation = 0.0f);
            void DrawFilledRectangle(PandaSDL::Rectangle rect, PandaSDL::Color color, PandaSDL::Vector2 centre, float rotation = 0.0f);
            void DrawEmptyRectangle(PandaSDL::Rectangle rect, PandaSDL::Color color, unsigned int lineSize, float rotation = 0.0f);
            
            void DrawEmptyCircle(PandaSDL::Vector2 position, float radius, PandaSDL::Color color, eCircleQuality quality = eCircleQuality::HIGH);
            void DrawFilledCircle(PandaSDL::Vector2 position, float radius, PandaSDL::Color color, eCircleQuality quality = eCircleQuality::HIGH);
            
            void End();
            void Clear();
            
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
            
            glm::mat4 _currentBatchTransform;
            std::vector<PrimitiveBase2D*> _currentBatch;
            std::vector<PrimitiveBatchVertex> _batchVertices;
            unsigned int _batchSize;
            
            void Flush(unsigned int drawMode);
            void CheckDefaultShaders();
    };
}

#endif