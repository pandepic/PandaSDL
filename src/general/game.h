#ifndef __pandasdl_game_h__
#define __pandasdl_game_h__

#include "../../lib_include/GLEW/glew.h"
#include <GL/gl.h>

#include "../../lib_include/SDL2/SDL.h"
#include "../../lib_include/SDL2/SDL_opengl.h"

#include "globals.h"
#include "util.h"
#include "../math/rectangle.h"
#include "../graphics/color.h"
#include "timer.h"
#include "../assets/dynamic_texture_manager.h"
#include "../assets/asset_manager.h"
#include "../sprites/animation_manager.h"
#include "../graphics/sprite_batch.h"
#include "../input/input_manager.h"
#include "gamestate.h"
#include "settings_manager.h"
#include "../input/game_control_manager.h"
#include "../graphics/graphics_platform_opengl.h"

namespace PandaSDL
{
    // forward declarations
    class AssetManager;
    class Gamestate;
    class GameControlManager;
    
    class Game
    {
        public:
            Game();
            virtual ~Game();

            void Setup(PandaSDL::Game* gameInstance, std::string windowName, PandaSDL::Rectangle windowRect, std::string assetsPath, std::string animationsAssetName = "Animations", bool vsync = true, bool glDebug = false);
            int Run();
            void ResetViewport();
            void SetClearColor(PandaSDL::Color color);
            void Clear();
            void Clear(PandaSDL::Color color);
            
            void EnableBlend(GLenum sfactor = GL_SRC_ALPHA, GLenum dfactor = GL_ONE_MINUS_SRC_ALPHA);
            void EnableDepth(GLenum func = GL_LESS);
            
            PandaSDL::Rectangle GetWindowRect();
            bool GetDepthEnabled();
            bool GetBlendEnabled();
            
            virtual void Load() = 0;
            virtual void Update(const PandaSDL::Timer &gameTimer) = 0;
            virtual void Draw(const PandaSDL::Timer &gameTimer) = 0;
            
            void SetGamestate(PandaSDL::Gamestate *NewState);
            void SetNextGamestate(PandaSDL::Gamestate *NewState);
            
            void Quit();

            static PandaSDL::Game* GameInstance;
            static PandaSDL::DynamicTextureManager DynamicTextureManager;
            static PandaSDL::AssetManager AssetManager;
            static PandaSDL::AnimationManager AnimationManager;
            static PandaSDL::InputManager InputManager;
            static PandaSDL::SettingsManager SettingsManager;
            static PandaSDL::GameControlManager GameControlManager;

        protected:
            bool _quit;
            
            bool _blendEnabled;
            bool _depthEnabled;
            unsigned int _clearFlags;

            // manage framerate
            unsigned int _frameCounter = 0;
            unsigned int _frameCounterTicks = 0;
            int _targetFPS = PANDASDL_UNLIMITED_FPS;
            PandaSDL::Timer _frameTimer;
            PandaSDL::Timer _gameTimer;

            // window
            std::string _windowName = "";
            PandaSDL::Rectangle _windowRect;
            Uint32 _windowFlags;
            std::shared_ptr<SDL_Window> _window = NULL;

            // graphics
            SDL_GLContext _glContext = NULL;
            PandaSDL::Color _windowClearColor;
            
            // game
            PandaSDL::Gamestate* CurrentGamestate = nullptr;
            PandaSDL::Gamestate* NextGamestate = nullptr;
    };
}

#endif
