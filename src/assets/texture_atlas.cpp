#include "texture_atlas.h"

PandaSDL::TextureAtlasSprite::TextureAtlasSprite()
    : Name(""), SourceRect(PandaSDL::Rectangle::Empty())
{
}

PandaSDL::TextureAtlas::TextureAtlas()
{
}

PandaSDL::TextureAtlas::~TextureAtlas()
{
}

void PandaSDL::TextureAtlas::SetTexture(std::shared_ptr<Texture2D> texture)
{
    _texture = texture;
}

void PandaSDL::TextureAtlas::AddSpriteData(std::string name, Rectangle sourceRect)
{
    TextureAtlasSprite newSprite;
    newSprite.Name = name;
    newSprite.SourceRect = sourceRect;
    
    _sprites.insert({ name, newSprite });
}

PandaSDL::TextureAtlasSprite PandaSDL::TextureAtlas::GetSpriteData(std::string name)
{
    return _sprites[name];
}
