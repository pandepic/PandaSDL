#ifndef __pandasdl_spritefont_h__
#define __pandasdl_spritefont_h__

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../general/game.h"
#include "texture_2D.h"
#include "../math/rectangle.h"
#include "../math/vector2.h"
#include "frame_buffer.h"
#include "sprite_batch.h"

#define PANDASDL_FONT_TEXTURE_MAXWIDTH 2000
#define PANDASDL_FONT_TEXTURE_MAXHEIGHT 1000

namespace PandaSDL
{
    struct SpriteFontCharacterData
    {
        bool Whitespace;
        
        unsigned char Character;
        PandaSDL::Vector2 Size;
        PandaSDL::Vector2 Bearing;
        unsigned int Advance;
        
        PandaSDL::Rectangle SourceRect;
        std::shared_ptr<Texture2D> Texture;
    };

    struct SpriteFontSizeData
    {
        SpriteFontSizeData() {}
        
        ~SpriteFontSizeData()
        {
            FT_Done_Face(FontFace);
        }
        
        unsigned int Size;
        PandaSDL::Vector2 NextCharPosition;
        std::shared_ptr<Texture2D> Texture;
        std::map<unsigned char, std::shared_ptr<SpriteFontCharacterData>> CharacterCache;
        
        FT_Face FontFace;
    };

    class SpriteFont
    {
        public:
            SpriteFont();
            ~SpriteFont();

            void Create(std::string filepath);
            PandaSDL::Vector2 MeasureText(std::string text, unsigned int size);

            std::shared_ptr<PandaSDL::SpriteFontCharacterData> GetCharacterData(unsigned char character, unsigned int size);

            void Clear();

        protected:
            std::string _filepath;
            std::map<unsigned int, std::shared_ptr<SpriteFontSizeData>> _sizeCache;
            std::vector<unsigned int> _textureIDs;

            static void InitFT();
            static FT_Library _ftLibrary;
            static bool _ftInit;
    };
}

#endif