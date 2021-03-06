#ifndef __pandasdl_asset_manager_h__
#define __pandasdl_asset_manager_h__

#include "../../lib_include/pugixml/pugixml.hpp"

#include "../../lib_include/glm/glm.hpp"
#include "../../lib_include/glm/gtc/type_ptr.hpp"

#include "../general/globals.h"
#include "../graphics/texture_2D.h"
#include "../graphics/shader.h"
#include "../graphics/sprite_font.h"
#include "../tiled/tiled_map.h"
#include "../graphics/frame_buffer.h"

namespace PandaSDL
{
    // forward declarations
    class SpriteFont;
    
    struct Asset
    {
        std::string Name, Filepath;
    };

    class AssetManager
    {
        public:
            AssetManager();
            ~AssetManager();

            void LoadAssets(std::string assetsFile);

            std::string GetAssetPath(std::string asset);

            std::shared_ptr<Texture2D> LoadTexture2D(std::string asset, float scale = 1.0f, bool flip = false);
            void UnloadTexture2D(std::string asset);

            std::shared_ptr<Shader> LoadShader(std::string name, std::string vertexAsset, std::string fragmentAsset, std::string geometryAsset = "");
            std::shared_ptr<Shader> LoadShaderFromString(std::string name, std::string vertexCode, std::string fragmentCode, std::string geometryCode = "");
            void UnloadShader(std::string name);

            std::shared_ptr<SpriteFont> LoadSpritefont(std::string asset);
            void UnloadSpritefont(std::string asset);

            std::shared_ptr<TiledMap> LoadTiledMap(std::string asset);
            void UnloadTiledMap(std::string asset);

            std::shared_ptr<Texture2D> GetTexture2D(std::string asset);
            std::shared_ptr<Shader> GetShader(std::string asset);
            std::shared_ptr<SpriteFont> GetSpritefont(std::string asset);
            std::shared_ptr<TiledMap> GetTiledMap(std::string asset);

            void Clear();

        protected:
            std::map<std::string, Asset> _assets;
            std::map<std::string, std::shared_ptr<Texture2D>> _textureCache;
            std::map<std::string, std::shared_ptr<Shader>> _shaderCache;
            std::map<std::string, std::shared_ptr<SpriteFont>> _fontCache;
            std::map<std::string, std::shared_ptr<TiledMap>> _tiledCache;
    };
}

#endif
