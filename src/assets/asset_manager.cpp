#include "asset_manager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../lib_include/stb/stb_image.h"

PandaSDL::AssetManager::AssetManager()
{
}

PandaSDL::AssetManager::~AssetManager()
{
    Clear();
}

void PandaSDL::AssetManager::Clear()
{
    _textureCache.clear();
    _shaderCache.clear();
    _fontCache.clear();
    _tiledCache.clear();
}

void PandaSDL::AssetManager::LoadAssets(std::string assetsFile)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(assetsFile.c_str());

    if (!result)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::LOADING_ASSETS_FAILED, result.description());

    for (auto node : doc.child("Assets").children("Asset"))
    {
        PandaSDL::Asset asset;
        asset.Name = node.attribute("Name").value();
        asset.Filepath = node.attribute("Filepath").value();

        _assets.insert(std::pair<std::string, PandaSDL::Asset>(asset.Name, asset));

        //std::cout << "Loaded asset " << asset.Name << " - " << asset.Filepath << std::endl;
    }
}

std::shared_ptr<PandaSDL::Texture2D> PandaSDL::AssetManager::LoadTexture2D(std::string asset, float scale)
{
    // todo: figure out if we should provide flip on loading or not
    // stbi_set_flip_vertically_on_load(true);
    
    auto texture = GetTexture2D(asset);

    if (texture != nullptr)
        return texture;

    auto filepath = GetAssetPath(asset);
    int width, height, channels;

    auto data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    
    auto newTexture = std::make_shared<Texture2D>();
    newTexture->Create(width, height, data, asset);

    stbi_image_free(data);
    
    // if a scale is passed auto scale the new texture
    if (scale != 1.0f)
    {
        auto tempTexture = std::make_shared<PandaSDL::Texture2D>();
        tempTexture->Create(width * scale, height * scale);
        
        PandaSDL::Framebuffer framebuffer;
        PandaSDL::Spritebatch spritebatch;
        spritebatch.Setup(tempTexture->GetWidth(), tempTexture->GetHeight(), true);
        
        glm::mat4 m(1.0f);
        
        framebuffer.Start(PandaSDL::Game::GameInstance, tempTexture);
        PandaSDL::Game::GameInstance->Clear(PANDASDL_COLOR_TRANSPARENT);
        spritebatch.Begin(glm::scale(m, { scale, scale, 1.0f }));
        spritebatch.Draw(newTexture, { 0, 0 });
        spritebatch.End();
        framebuffer.End();
        
        newTexture = nullptr;
        newTexture = tempTexture;
    }

    _textureCache.insert(std::pair<std::string, std::shared_ptr<Texture2D>>(asset, newTexture));

    //std::cout << "Loaded Texture2D " << asset << std::endl;

    return newTexture;
}

void PandaSDL::AssetManager::UnloadTexture2D(std::string asset)
{
    auto texture = GetTexture2D(asset);

    if (texture == NULL)
        return;

    _textureCache.erase(asset);

    //std::cout << "Unloaded Texture2D " << asset << std::endl;
}

std::shared_ptr<PandaSDL::Shader> PandaSDL::AssetManager::LoadShader(std::string name, std::string vertexAsset, std::string fragmentAsset, std::string geometryAsset)
{
    auto shader = GetShader(name);

    if (shader != nullptr)
        return shader;

    auto filepathVertex = GetAssetPath(vertexAsset);
    auto filepathFragment = GetAssetPath(fragmentAsset);
    auto filepathGeometry = geometryAsset.length() > 0 ? GetAssetPath(geometryAsset) : "";

    std::string vertexCode = "";
    std::string fragmentCode = "";
    std::string geometryCode = "";

    std::ifstream vertexShaderFile(filepathVertex.c_str());
    std::ifstream fragmentShaderFile(filepathFragment.c_str());
    std::stringstream vShaderStream, fShaderStream;

    vShaderStream << vertexShaderFile.rdbuf();
    fShaderStream << fragmentShaderFile.rdbuf();

    vertexShaderFile.close();
    fragmentShaderFile.close();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();

    // if geometry shader path is present, also load a geometry shader
    if (filepathGeometry.length() > 0)
    {
        std::ifstream geometryShaderFile(filepathGeometry.c_str());
        std::stringstream gShaderStream;
        gShaderStream << geometryShaderFile.rdbuf();
        geometryShaderFile.close();
        geometryCode = gShaderStream.str();
    }

    return LoadShaderFromString(name, vertexCode, fragmentCode, geometryCode);
}

std::shared_ptr<PandaSDL::Shader> PandaSDL::AssetManager::LoadShaderFromString(std::string name, std::string vertexCode, std::string fragmentCode, std::string geometryCode)
{
    auto shader = GetShader(name);

    if (shader != nullptr)
        return shader;
    
    auto newShader = std::make_shared<PandaSDL::Shader>();
    newShader->Compile(vertexCode.c_str(), fragmentCode.c_str(), geometryCode.length() > 0 ? geometryCode.c_str() : nullptr);
    
    _shaderCache.insert(std::pair<std::string, std::shared_ptr<PandaSDL::Shader>>(name, newShader));

    //std::cout << "Loaded Shader " << name << std::endl;
    return newShader;
}

void PandaSDL::AssetManager::UnloadShader(std::string asset)
{
    auto shader = GetShader(asset);

    if (shader == nullptr)
        return;
    
    _shaderCache.erase(asset);

    //std::cout << "Unloaded Shader " << asset << std::endl;
}

std::shared_ptr<PandaSDL::Spritefont> PandaSDL::AssetManager::LoadSpritefont(std::string asset)
{
    auto font = GetSpritefont(asset);

    if (font != nullptr)
        return font;
    
    auto filepath = GetAssetPath(asset);

    auto newFont = std::make_shared<PandaSDL::Spritefont>();
    newFont->Create(filepath);
    
    _fontCache.insert(std::pair<std::string, std::shared_ptr<PandaSDL::Spritefont>>(asset, newFont));
    
    //std::cout << "Loaded Spritefont " << asset << std::endl;
    return newFont;
}

void PandaSDL::AssetManager::UnloadSpritefont(std::string asset)
{
    auto font = GetSpritefont(asset);

    if (font == nullptr)
        return;
    
    _fontCache.erase(asset);

    //std::cout << "Unloaded Spritefont " << asset << std::endl;
}

std::shared_ptr<PandaSDL::TiledMap> PandaSDL::AssetManager::LoadTiledMap(std::string asset)
{
    auto tmx = GetTiledMap(asset);
    
    if (tmx != nullptr)
        return tmx;
    
    auto filepath = GetAssetPath(asset);
    
    auto newTMX = std::make_shared<PandaSDL::TiledMap>();
    newTMX->Load(filepath);
    
    _tiledCache.insert(std::pair<std::string, std::shared_ptr<PandaSDL::TiledMap>>(asset, newTMX));
    
    //std::cout << "Loaded Tiled Map " << asset << std::endl;
    return newTMX;
}

void PandaSDL::AssetManager::UnloadTiledMap(std::string asset)
{
    auto tmx = GetTiledMap(asset);
    
    if (tmx == nullptr)
        return;
    
    _tiledCache.erase(asset);
    
    //std::cout << "Unloaded Tiled Map " << asset << std::endl;
}

std::shared_ptr<PandaSDL::Texture2D> PandaSDL::AssetManager::GetTexture2D(std::string asset)
{
    if (_textureCache.count(asset) == 0)
        return nullptr;

    return _textureCache[asset];
}

std::shared_ptr<PandaSDL::Shader> PandaSDL::AssetManager::GetShader(std::string asset)
{
    if (_shaderCache.count(asset) == 0)
        return nullptr;

    return _shaderCache[asset];
}

std::shared_ptr<PandaSDL::Spritefont> PandaSDL::AssetManager::GetSpritefont(std::string asset)
{
    if (_fontCache.count(asset) == 0)
        return nullptr;

    return _fontCache[asset];
}

std::shared_ptr<PandaSDL::TiledMap> PandaSDL::AssetManager::GetTiledMap(std::string asset)
{
    if (_tiledCache.count(asset) == 0)
        return nullptr;

    return _tiledCache[asset];
}

std::string PandaSDL::AssetManager::GetAssetPath(std::string asset)
{
    if (_assets.count(asset) == 0)
        PandaSDL::ThrowException(PandaSDL::ePandaSDLException::ASSET_PATH_NOT_FOUND, asset);

    return _assets[asset].Filepath;
}
