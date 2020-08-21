#ifndef __pandasdl_globals_h__
#define __pandasdl_globals_h__

#define PANDASDL_UNLIMITED_FPS -1
#define PANDASDL_VSYNC_ON true
#define PANDASDL_VSYNC_OFF false

#define PANDASDL_DEFAULT_BATCH_SIZE 1000
#define PANDASDL_QUAD_VERTEX_COUNT 4
#define PANDASDL_PRIMITIVE_QUAD_VERTEX_COUNT 6
#define PANDASDL_QUAD_INDEX_COUNT 6
#define PANDASDL_QUAD_VERTEX_FLOAT_COUNT (PANDASDL_QUAD_VERTEX_COUNT * 2)
#define PANDASDL_TILEBATCH_QUAD_VERTEX_FLOAT_COUNT 24

namespace PandaSDL
{
    enum class eGameControlType
    {
        KEYBOARD,
        MOUSE
    };
    
    static std::map<std::string, eGameControlType> StringToGameControlType =
    {
        { "KEYBOARD", eGameControlType::KEYBOARD },
        { "MOUSE", eGameControlType::MOUSE },
    };
    
    enum class eMouseScrollDirection
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    
    enum class eMouseButtonState
    {
        RELEASED,
        PRESSED,
    };
    
    enum class eMouseButton
    {
        LEFT,
        MIDDLE,
        RIGHT,
    };
    
    enum class eKeyboardButtonState
    {
        PRESSED,
        RELEASED,
        DOWN,
    };
    
    static std::map<std::string, eKeyboardButtonState> StringToKeyboardButtonState =
    {
        { "PRESSED", eKeyboardButtonState::PRESSED },
        { "RELEASED", eKeyboardButtonState::RELEASED },
        { "DOWN", eKeyboardButtonState::DOWN },
    };
    
    static std::map<eKeyboardButtonState, std::string> KeyboardButtonStateToString =
    {
        { eKeyboardButtonState::PRESSED, "PRESSED" },
        { eKeyboardButtonState::RELEASED, "RELEASED" },
        { eKeyboardButtonState::DOWN, "DOWN" },
    };
    
    enum class eSpriteFlip
    {
        NONE,
        HORIZONTAL,
        VERTICAL,
        BOTH,
    };

    static std::map<std::string, eSpriteFlip> StringToSpriteFlip =
    {
        { "NONE", eSpriteFlip::NONE },
        { "HORIZONTAL", eSpriteFlip::HORIZONTAL },
        { "VERTICAL", eSpriteFlip::VERTICAL },
        { "BOTH", eSpriteFlip::BOTH }
    };
    
    enum class ePandaSDLException
    {
        UNKNOWN = -1,
        SDL_INIT_FAILED,
        ASSET_PATH_NOT_FOUND,
        LOADING_ASSETS_FAILED,
        GLEW_INIT_FAILED,
        SPRITEBATCH_SETUP,
        SPRITEBATCH_BEGIN,
        SPRITEBATCH_DRAW,
        SPRITEBATCH_FLUSH,
        FRAMEBUFFER_START_FAILED,
        DYNAMIC_TEXTURE2D_INVALID_ID,
        ANIMATION_MANAGER_MISSING,
        LOADING_ANIMATIONS_FAILED,
        FREETYPE_INIT_FAILED,
        FREETYPE_LOADFONT_FAILED,
        FREETYPE_LOADGLYPH_FAILED,
        SPRITEFONT_MAX_TEXTURE_SIZE_REACHED,
        LOADING_SETTINGS_FAILED,
        LOADING_TILEDMAP_FAILED,
        PRIMITIVEBATCH_SETUP,
        PRIMITIVEBATCH_BEGIN,
        PRIMITIVEBATCH_DRAW,
        GRAPHICS_BUFFER,
    };

    static std::map<ePandaSDLException, std::string> PandaSDLExceptionNames =
    {
        { ePandaSDLException::UNKNOWN, "UNKNOWN" },
        { ePandaSDLException::SDL_INIT_FAILED, "SDL_INIT_FAILED" },
        { ePandaSDLException::ASSET_PATH_NOT_FOUND, "ASSET_PATH_NOT_FOUND" },
        { ePandaSDLException::LOADING_ASSETS_FAILED, "LOADING_ASSETS_FAILED" },
        { ePandaSDLException::GLEW_INIT_FAILED, "GLEW_INIT_FAILED" },
        { ePandaSDLException::SPRITEBATCH_SETUP, "SPRITEBATCH_SETUP" },
        { ePandaSDLException::SPRITEBATCH_BEGIN, "SPRITEBATCH_BEGIN" },
        { ePandaSDLException::SPRITEBATCH_DRAW, "SPRITEBATCH_DRAW" },
        { ePandaSDLException::SPRITEBATCH_FLUSH, "SPRITEBATCH_FLUSH" },
        { ePandaSDLException::FRAMEBUFFER_START_FAILED, "FRAMEBUFFER_START_FAILED" },
        { ePandaSDLException::DYNAMIC_TEXTURE2D_INVALID_ID, "DYNAMIC_TEXTURE2D_INVALID_ID" },
        { ePandaSDLException::ANIMATION_MANAGER_MISSING, "ANIMATION_MANAGER_MISSING" },
        { ePandaSDLException::LOADING_ANIMATIONS_FAILED, "LOADING_ANIMATIONS_FAILED" },
        { ePandaSDLException::FREETYPE_INIT_FAILED, "FREETYPE_INIT_FAILED" },
        { ePandaSDLException::FREETYPE_LOADFONT_FAILED, "FREETYPE_LOADFONT_FAILED" },
        { ePandaSDLException::FREETYPE_LOADGLYPH_FAILED, "FREETYPE_LOADGLYPH_FAILED" },
        { ePandaSDLException::SPRITEFONT_MAX_TEXTURE_SIZE_REACHED, "SPRITEFONT_MAX_TEXTURE_SIZE_REACHED" },
        { ePandaSDLException::LOADING_SETTINGS_FAILED, "LOADING_SETTINGS_FAILED" },
        { ePandaSDLException::LOADING_TILEDMAP_FAILED, "LOADING_TILEDMAP_FAILED" },
        { ePandaSDLException::PRIMITIVEBATCH_SETUP, "PRIMITIVEBATCH_SETUP" },
        { ePandaSDLException::PRIMITIVEBATCH_BEGIN, "PRIMITIVEBATCH_BEGIN" },
        { ePandaSDLException::PRIMITIVEBATCH_DRAW, "PRIMITIVEBATCH_DRAW" },
        { ePandaSDLException::GRAPHICS_BUFFER, "GRAPHICS_BUFFER" }
    };

    static void ThrowException(ePandaSDLException type, std::string message = "")
    {
        throw std::runtime_error(PandaSDLExceptionNames[type] + " - " + message);
    }
    
    template<typename E>
    constexpr auto ToUnderlyingType(E e) 
    {
        return static_cast<typename std::underlying_type<E>::type>(e);
    }
}

#endif
