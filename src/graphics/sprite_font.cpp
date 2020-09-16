#include "sprite_font.h"

FT_Library PandaSDL::SpriteFont::_ftLibrary = FT_Library();
bool PandaSDL::SpriteFont::_ftInit = false;

PandaSDL::SpriteFont::SpriteFont()
{
}

PandaSDL::SpriteFont::~SpriteFont()
{
    Clear();
}

void PandaSDL::SpriteFont::Clear()
{
}

void PandaSDL::SpriteFont::Create(std::string filepath)
{
    PandaSDL::SpriteFont::InitFT();
    
    _filepath = filepath;
}

void PandaSDL::SpriteFont::InitFT()
{
    if (PandaSDL::SpriteFont::_ftInit)
        return;
    
    if (FT_Init_FreeType(&PandaSDL::SpriteFont::_ftLibrary))
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::FREETYPE_INIT_FAILED);
    
    PandaSDL::SpriteFont::_ftInit = true;
}

std::shared_ptr<PandaSDL::SpriteFontCharacterData> PandaSDL::SpriteFont::GetCharacterData(unsigned char character, unsigned int size)
{
    if (_sizeCache.count(size) == 0)
    {
        auto newSize = std::make_shared<PandaSDL::SpriteFontSizeData>();
        newSize->Size = size;
        newSize->NextCharPosition = PandaSDL::Vector2::Zero();
        newSize->Texture = PandaSDL::Game::DynamicTextureManager.CreateTexture2D(PANDASDL_FONT_TEXTURE_MAXWIDTH, PANDASDL_FONT_TEXTURE_MAXHEIGHT);
        
        if (FT_New_Face(PandaSDL::SpriteFont::_ftLibrary, _filepath.c_str(), 0, &newSize->FontFace))
            PandaSDL::ThrowException(PandaSDL::ePandaSDLException::FREETYPE_LOADFONT_FAILED);
        
        FT_Set_Pixel_Sizes(newSize->FontFace, 0, size);
        
        _sizeCache.insert({ size, newSize });
    }
    
    auto sizeData = _sizeCache[size];
    
    if (sizeData->CharacterCache.count(character) == 0)
    {
        if (FT_Load_Char(sizeData->FontFace, character, FT_LOAD_RENDER))
            PandaSDL::ThrowException(PandaSDL::ePandaSDLException::FREETYPE_LOADGLYPH_FAILED);
        
        auto tempTexture = std::make_shared<PandaSDL::Texture2D>(GL_RED, GL_LINEAR, GL_CLAMP_TO_EDGE);
        tempTexture->Create(sizeData->FontFace->glyph->bitmap.width, sizeData->FontFace->glyph->bitmap.rows, sizeData->FontFace->glyph->bitmap.buffer);
        
        if ((sizeData->NextCharPosition.X + tempTexture->GetWidth()) > PANDASDL_FONT_TEXTURE_MAXWIDTH)
        {
            sizeData->NextCharPosition.X = 0;
            sizeData->NextCharPosition.Y += size;
            
            // todo : handle the case where there are too many glyphs for the texture size
            if ((sizeData->NextCharPosition.Y + size) > PANDASDL_FONT_TEXTURE_MAXHEIGHT)
                PandaSDL::ThrowException(PandaSDL::ePandaSDLException::SPRITEFONT_MAX_TEXTURE_SIZE_REACHED);
        }
        
        auto whitespace = true;
        
        if (tempTexture->GetWidth() != 0 && tempTexture->GetHeight() != 0)
        {
            PandaSDL::SpriteBatch spriteBatch;
            spriteBatch.Setup(sizeData->Texture->GetWidth(), sizeData->Texture->GetHeight(), true, PandaSDL::SpriteBatch::DefaultFontShader);
            
            PandaSDL::FrameBuffer frameBuffer;
            
            frameBuffer.Start(PandaSDL::Game::GameInstance, sizeData->Texture);
            if (sizeData->NextCharPosition.IsZero())
                PandaSDL::Game::GameInstance->Clear(PANDASDL_COLOR_TRANSPARENT);
            
            spriteBatch.Begin();
            
            spriteBatch.Draw(tempTexture, sizeData->NextCharPosition);
            
            spriteBatch.End();
            frameBuffer.End();
            
            whitespace = false;
        }
        
        auto newCharacter = std::make_shared<SpriteFontCharacterData>();
        newCharacter->Character = character;
        newCharacter->Size = PandaSDL::Vector2(sizeData->FontFace->glyph->bitmap.width, sizeData->FontFace->glyph->bitmap.rows);
        newCharacter->Bearing = PandaSDL::Vector2(sizeData->FontFace->glyph->bitmap_left, sizeData->FontFace->glyph->bitmap_top);
        newCharacter->Advance = sizeData->FontFace->glyph->advance.x;
        newCharacter->Texture = sizeData->Texture;
        newCharacter->SourceRect = PandaSDL::Rectangle(sizeData->NextCharPosition.X, sizeData->NextCharPosition.Y, tempTexture->GetWidth(), tempTexture->GetHeight());
        newCharacter->Whitespace = whitespace;
        
        sizeData->CharacterCache.insert({ character, newCharacter });
        sizeData->NextCharPosition.X += tempTexture->GetWidth();
    }
    
    return sizeData->CharacterCache[character];
}

PandaSDL::Vector2 PandaSDL::SpriteFont::MeasureText(std::string text, unsigned int size)
{
    auto stringSize = PandaSDL::Vector2::Zero();
    
    for (const char& c : text)
    {
        auto charData = GetCharacterData(c, size);
        
        if (!charData->Whitespace)
        {
            if (charData->Size.Y > stringSize.Y)
                stringSize.Y = charData->Size.Y;
        }
        
        stringSize.X += (charData->Advance >> 6);
    }
    
    return stringSize;
}
