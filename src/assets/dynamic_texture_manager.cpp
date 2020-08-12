#include "dynamic_texture_manager.h"

PandaSDL::DynamicTextureManager::DynamicTextureManager()
    : _texture2DIndexCounter(0)
{
}

PandaSDL::DynamicTextureManager::~DynamicTextureManager()
{
    Clear();
}

std::shared_ptr<PandaSDL::Texture2D> PandaSDL::DynamicTextureManager::CreateTexture2D(unsigned int width, unsigned int height)
{
    unsigned int id = 0;
    return CreateTexture2D(width, height, id);
}

std::shared_ptr<PandaSDL::Texture2D> PandaSDL::DynamicTextureManager::CreateTexture2D(unsigned int width, unsigned int height, unsigned int &id)
{
    auto newTexture = std::make_shared<PandaSDL::Texture2D>();
    newTexture->Create(width, height);

    id = _texture2DIndexCounter;
    _texture2DCache.insert(std::pair<unsigned int, std::shared_ptr<PandaSDL::Texture2D>>(_texture2DIndexCounter, newTexture));
    _texture2DIndexCounter += 1;

    return newTexture;
}

std::shared_ptr<PandaSDL::Texture2D> PandaSDL::DynamicTextureManager::GetTexture2D(unsigned int id)
{
    auto iter = _texture2DCache.find(id);

    if (iter == _texture2DCache.end())
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::DYNAMIC_TEXTURE2D_INVALID_ID);
    
    return iter->second;
}

void PandaSDL::DynamicTextureManager::FreeTexture2D(unsigned int id)
{
    _texture2DCache.erase(id);
}

void PandaSDL::DynamicTextureManager::Clear()
{
    _texture2DCache.clear();
}