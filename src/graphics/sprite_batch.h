#ifndef __pandasdl_spritebatch_h__
#define __pandasdl_spritebatch_h__

#include "../../lib_include/GLEW/glew.h"
#include <GL/gl.h>

#include "../../lib_include/glm/glm.hpp"
#include "../../lib_include/glm/gtc/type_ptr.hpp"

#include "../general/globals.h"
#include "texture_2D.h"
#include "../math/rectangle.h"
#include "../math/vector2.h"
#include "shader.h"
#include "color.h"
#include "sprite_font.h"
#include "graphics_platform_opengl.h"

namespace PandaSDL
{
    enum class QuadVertexIndex
    {
        BOTTOM_LEFT = 0,
        TOP_RIGHT,
        TOP_LEFT,
        BOTTOM_RIGHT
    };

    #pragma pack(push, 0)

    struct SpriteBatchVertex
    {
        glm::vec2 Position;
        glm::vec2 TexCoords;
        glm::vec4 Colour;
        
        static std::vector<VertexAttribTypeMapFVar> VertexAttribMap() { return { 2, 2, 4 }; }
    };

    #pragma pack(pop)

    struct SpriteBatchItem
    {
        std::shared_ptr<PandaSDL::Texture2D> Texture;
        Rectangle SourceRect;
        Vector2 Position;
        Color Color;
        Vector2 Scale;
        Vector2 Origin;
        float Rotation;
        PandaSDL::eSpriteFlip Flip;
    };
    
    // forward declarations
    class SpriteFont;

    class SpriteBatch
    {
        public:
            SpriteBatch();
            ~SpriteBatch();

            void Setup(
                int                         screenWidth,
                int                         screenHeight,
                bool                        invertY =       false,
                std::shared_ptr<Shader>     spriteShader =  DefaultSpriteShader,
                unsigned int                maxBatchSize =  PANDASDL_DEFAULT_BATCH_SIZE);

            void Begin(glm::mat4 transform = _defaultTransform);
            
            void Draw(
                std::shared_ptr<Texture2D>  texture,
                Vector2                     position,
                Color                       color =     PANDASDL_COLOR_WHITE,
                Vector2                     scale =     Vector2(1.0f, 1.0f),
                float                       rotation =  0.0f, // in degrees
                eSpriteFlip                 flip =      eSpriteFlip::NONE,
                Vector2                     origin =    Vector2(0.0f, 0.0f));
            
            void Draw(
                std::shared_ptr<Texture2D>  texture,
                Rectangle                   sourceRect,
                Vector2                     position,
                Color                       color =     PANDASDL_COLOR_WHITE,
                Vector2                     scale =     Vector2(1.0f, 1.0f),
                float                       rotation =  0.0f, // in degrees
                eSpriteFlip                 flip =      eSpriteFlip::NONE,
                Vector2                     origin =    Vector2(0.0f, 0.0f));
            
            void DrawText(
                std::shared_ptr<SpriteFont> font,
                std::string                 text,
                unsigned int                size,
                Vector2                     position,
                Color                       color =         PANDASDL_COLOR_WHITE,
                bool                        alignPosition = false,
                Vector2                     scale =         Vector2(1.0f, 1.0f),
                float                       rotation =      0.0f, // in degrees
                eSpriteFlip                 flip =          eSpriteFlip::NONE,
                Vector2                     origin =        Vector2(0.0f, 0.0f));
            
            void End();
            void Clear();

            static std::shared_ptr<Shader> DefaultSpriteShader;
            static std::shared_ptr<Shader> DefaultFontShader;
            static std::string DefaultSpriteShaderVertexCode;
            static std::string DefaultSpriteShaderFragmentCode;
            static std::string DefaultFontShaderVertexCode;
            static std::string DefaultFontShaderFragmentCode;
            static bool DefaultShadersInitialised;

        protected:
            unsigned int _maxBatchSize;
            bool _initialised, _begin;
            std::unique_ptr<VertexArrayObject> _vao;
            std::shared_ptr<VertexBufferObject> _vbo;
            std::shared_ptr<IndexBufferObject> _ibo;
            std::shared_ptr<Shader> _spriteShader;
            glm::mat4 _projection;
            
            int _screenWidth, _screenHeight;
            static glm::mat4 _defaultTransform;

            glm::mat4 _currentBatchTransform;
            std::vector<SpriteBatchItem> _currentBatch; // todo : test if a C array is faster here
            float _tempVertexBuffer[PANDASDL_QUAD_VERTEX_FLOAT_COUNT];
            std::vector<SpriteBatchVertex> _batchVertices;

            void AddQuadVertices(const SpriteBatchItem &item);
            void Flush(std::shared_ptr<PandaSDL::Texture2D> texture);
            void CheckDefaultShaders();
    };
}

#endif
