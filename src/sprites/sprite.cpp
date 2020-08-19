#include "sprite.h"

PandaSDL::Sprite::Sprite()
    : Visible(true), Scale(1.0f), Rotation(0.0f), Colour(PANDASDL_COLOR_WHITE), Flip(PandaSDL::eSpriteFlip::NONE)
{
}

PandaSDL::Sprite::~Sprite()
{
}

int PandaSDL::Sprite::GetWidth()
{
    if (Texture != nullptr)
        return Texture->GetWidth();
    else
        return 0;
}

int PandaSDL::Sprite::GetHeight()
{
    if (Texture != nullptr)
        return Texture->GetHeight();
    else
        return 0;
}

void PandaSDL::Sprite::LoadTexture(std::shared_ptr<PandaSDL::Texture2D> texture)
{
    if (texture == nullptr)
        return;
    
    Texture = texture;
    
    Center = PandaSDL::Vector2((int)Texture->GetWidth() / 2, (int)Texture->GetHeight() / 2);
    SourceRect = PandaSDL::Rectangle(0, 0, Texture->GetWidth(), Texture->GetHeight());
}

void PandaSDL::Sprite::Draw(std::shared_ptr<PandaSDL::SpriteBatch> spriteBatch, PandaSDL::Vector2 position)
{
    if (Texture == nullptr)
        return;
    
    if (!Visible)
        return;
    
    PandaSDL::Vector2 drawPosition = PandaSDL::Vector2((int)position.X, (int)position.Y);

    spriteBatch->Draw(Texture, SourceRect, drawPosition, Colour, Scale, Rotation, Flip);
}

void PandaSDL::Sprite::Update(const PandaSDL::Timer& gameTimer)
{
}
