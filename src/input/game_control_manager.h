#ifndef __pandasdl_gamecontrolmanager_h__
#define __pandasdl_gamecontrolmanager_h__

#include "../general/game.h"
#include "input_manager.h"
#include "../general/extensions.h"

namespace PandaSDL
{
    struct KeyboardGameControl
    {
        std::string Name;
        std::vector<std::vector<std::string>> ControlKeys;
        PandaSDL::eKeyboardButtonState State;
    };
    
    class HandleGameControls
    {
        public:
            HandleGameControls() {}
            virtual ~HandleGameControls() {}
            
            virtual void HandleGameControl(const std::string &controlName, const PandaSDL::KeyboardState &keyboardState, const PandaSDL::MouseState &mouseState, const Timer& gameTimer) = 0;
    };
    
    class GameControlManager : public PandaSDL::HandleKeyboardInput, public PandaSDL::HandleMouseInput
    {
        public:
            GameControlManager();
            ~GameControlManager();
            
            void Setup(std::string settingsSection);
            
            void RegisterGameControlHandler(HandleGameControls* gameControlHandler);
            void RemoveGameControlHandler(HandleGameControls* gameControlHandler);
            
            virtual void HandleKeyPressed(const SDL_Keycode &keyCode, const std::string &keyString, const PandaSDL::KeyboardState &keyboardState, const Timer& gameTimer) override;
            virtual void HandleKeyReleased(const SDL_Keycode &keyCode, const std::string &keyString, const PandaSDL::KeyboardState &keyboardState, const Timer& gameTimer) override;
            virtual void HandleKeyDown(const SDL_Keycode &keyCode, const std::string &keyString, const PandaSDL::KeyboardState &keyboardState, const Timer& gameTimer) override;
            
            virtual void HandleMouseMotion(int xMotion, int yMotion, const PandaSDL::MouseState &mouseState, const Timer& gameTimer) override;
            virtual void HandleMouseButtonPressed(PandaSDL::eMouseButton button, const PandaSDL::MouseState &mouseState, const Timer& gameTimer) override;
            virtual void HandleMouseButtonReleased(PandaSDL::eMouseButton button, const PandaSDL::MouseState &mouseState, const Timer& gameTimer) override;
            virtual void HandleMouseButtonDown(PandaSDL::eMouseButton button, const PandaSDL::MouseState &mouseState, const Timer& gameTimer) override;
            virtual void HandleMouseWheel(PandaSDL::eMouseScrollDirection direction, const PandaSDL::MouseState &mouseState, const Timer& gameTimer) override;
            
        protected:
            bool _inputRegistered;
            std::map<std::string, PandaSDL::KeyboardGameControl> _keyboardControls;
            std::vector<HandleGameControls*> _gameControlHandlers;
            
            void CheckKeyboardControl(const PandaSDL::KeyboardGameControl &keyboardControl, const std::string &keyString, const PandaSDL::KeyboardState &keyboardState, const Timer& gameTimer);
            void TriggerGameControl(const std::string &controlName, const Timer& gameTimer);
    };
}

#endif