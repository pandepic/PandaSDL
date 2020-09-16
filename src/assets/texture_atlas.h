#ifndef __pandasdl_textureatlas_h__
#define __pandasdl_textureatlas_h__

#include "../graphics/texture_2D.h"
#include "../math/rectangle.h"

namespace PandaSDL
{
    struct TextureAtlasSprite
    {
        TextureAtlasSprite();
        
        std::string Name;
        Rectangle SourceRect;
    };
    
    class TextureAtlas
    {
        public:
            TextureAtlas();
            ~TextureAtlas();
            
            void SetTexture(std::shared_ptr<Texture2D> texture);
            void AddSpriteData(std::string name, Rectangle sourceRect);
            PandaSDL::TextureAtlasSprite GetSpriteData(std::string name);
            
        protected:
            std::shared_ptr<Texture2D> _texture;
            std::map<std::string, TextureAtlasSprite> _sprites;
    };
}

#endif
