#include "layered_sprite.h"

PandaSDL::LayeredSprite::LayeredSprite()
{
}

PandaSDL::LayeredSprite::~LayeredSprite()
{
}

void PandaSDL::LayeredSprite::Create(std::vector<std::string> layerAssets, int frameWidth, int frameHeight, int defaultFrame)
{
    if (layerAssets.size() == 0)
        return;
    
    std::vector<std::shared_ptr<Texture2D>> layerTextures;

    for (const auto& asset: layerAssets)
    {
        layerTextures.push_back(PandaSDL::Game::AssetManager.LoadTexture2D(asset));
    }

    auto w = layerTextures[0]->GetWidth();
    auto h = layerTextures[0]->GetHeight();

    auto newTexture = PandaSDL::Game::DynamicTextureManager.CreateTexture2D(layerTextures[0]->GetWidth(), layerTextures[0]->GetHeight());

    PandaSDL::SpriteBatch spriteBatch;
    spriteBatch.Setup(layerTextures[0]->GetWidth(), layerTextures[0]->GetHeight(), true);

    PandaSDL::FrameBuffer frameBuffer;

    frameBuffer.Start(PandaSDL::Game::GameInstance, newTexture);
    PandaSDL::Game::GameInstance->Clear(PANDASDL_COLOR_TRANSPARENT);
    spriteBatch.Begin();
    for (const auto& texture: layerTextures)
    {
        spriteBatch.Draw(texture, PandaSDL::Vector2::Zero());
    }
    spriteBatch.End();
    frameBuffer.End();

    Init(newTexture, frameWidth, frameHeight, defaultFrame);
}