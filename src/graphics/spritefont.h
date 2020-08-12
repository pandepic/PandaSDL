#ifndef __pandasdl_spritefont_h__
#define __pandasdl_spritefont_h__

#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../general/game.h"
#include "texture_2D.h"
#include "../math/rectangle.h"
#include "../math/vector2.h"
#include "framebuffer.h"

#define PANDASDL_FONT_TEXTURE_MAXWIDTH 2000
#define PANDASDL_FONT_TEXTURE_MAXHEIGHT 1000

namespace PandaSDL
{
    struct SpritefontCharacterData
    {
        bool Whitespace;
        
        unsigned char Character;
        PandaSDL::Vector2 Size;
        PandaSDL::Vector2 Bearing;
        unsigned int Advance;
        
        PandaSDL::Rectangle SourceRect;
        std::shared_ptr<Texture2D> Texture;
    };

    struct SpritefontSizeData
    {
        SpritefontSizeData() {}
        
        ~SpritefontSizeData()
        {
            FT_Done_Face(FontFace);
        }
        
        unsigned int Size;
        PandaSDL::Vector2 NextCharPosition;
        std::shared_ptr<Texture2D> Texture;
        std::map<unsigned char, std::shared_ptr<SpritefontCharacterData>> CharacterCache;
        
        FT_Face FontFace;
    };

    class Spritefont
    {
        public:
            Spritefont();
            ~Spritefont();

            void Create(std::string filepath);
            PandaSDL::Vector2 MeasureText(std::string text, unsigned int size);

            std::shared_ptr<PandaSDL::SpritefontCharacterData> GetCharacterData(unsigned char character, unsigned int size);

            void Clear();

        protected:
            std::string _filepath;
            std::map<unsigned int, std::shared_ptr<SpritefontSizeData>> _sizeCache;
            std::vector<unsigned int> _textureIDs;

            static void InitFT();
            static FT_Library _ftLibrary;
            static bool _ftInit;
    };
}

#endif