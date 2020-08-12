#ifndef __pandasdl_tilebatch_h__
#define __pandasdl_tilebatch_h__

#include <iostream>
#include <memory>
#include <vector>

#include "../../lib_include/GLEW/glew.h"
#include <GL/gl.h>

#include "../../lib_include/glm/glm.hpp"
#include "../../lib_include/glm/gtc/type_ptr.hpp"

#include "../general/game.h"
#include "texture_2D.h"
#include "shader.h"
#include "../math/vector2.h"

namespace PandaSDL
{
    struct TileTextureData
    {
        unsigned char X;
        unsigned char Y;
    };
    
    struct TilebatchLayer
    {
        bool IsBelow;
        std::shared_ptr<PandaSDL::Texture2D> DataTexture;
    };
    
    class Tilebatch
    {
        public:
            Tilebatch();
            ~Tilebatch();
            
            std::vector<PandaSDL::TilebatchLayer> Layers;
            std::shared_ptr<PandaSDL::Texture2D> AtlasTexture;
            
            void BeginBuild(unsigned int width, unsigned int height, unsigned int tileWidth, unsigned int tileHeight, std::shared_ptr<PandaSDL::Texture2D> atlasTexture);
            void SetTileAtPosition(unsigned int posx, unsigned int posy, unsigned char x, unsigned char y);
            void SetTileAtPosition(unsigned int posx, unsigned int posy, unsigned int tileIndex);
            void SetTileAtIndex(unsigned int index, unsigned char x, unsigned char y);
            void SetTileAtIndex(unsigned int index, unsigned int tileIndex);
            void EndLayer(bool below);
            void EndBuild(bool below = true);
            
            void Draw(PandaSDL::Vector2 position, bool below);
            
            unsigned int GetWidth();
            unsigned int GetHeight();
            
            static std::shared_ptr<PandaSDL::Shader> DefaultTileShader;
            static std::string DefaultTileShaderVertexCode;
            static std::string DefaultTileShaderFragmentCode;
            static bool DefaultShaderInitialised;
            
        protected:
            bool _currentLayerEnded;
            unsigned int _width, _height;
            unsigned int _tileWidth, _tileHeight;
            
            GLuint _VAO, _VBO;
            
            glm::vec2 _inverseTileSize;
            glm::vec2 _scaledViewportSize;
            
            TileTextureData* _dataArray;
            bool _dataAllocated;
            
            float _templateVertexBuffer[PANDASDL_QUAD_VERTEX_FLOAT_COUNT];
            
            void CheckDefaultShader();
            void ClearData();
    };
}

#endif