#ifndef __pandasdl_inputmanager_h__
#define __pandasdl_inputmanager_h__

#include "../general/globals.h"
#include "../general/timer.h"
#include "../math/vector2.h"

namespace PandaSDL
{
    struct MouseState
    {
        PandaSDL::Vector2 Position;
        eMouseButtonState ButtonLeft;
        eMouseButtonState ButtonMiddle;
        eMouseButtonState ButtonRight;
    };
    
    struct KeyboardState
    {
        std::vector<SDL_Keycode> PressedKeys;
        bool IsKeyUp(const SDL_Keycode &key) const;
        bool IsKeyDown(const SDL_Keycode &key) const;
    };
    
    // input base classes
    class HandleKeyboardInput
    {
        public:
            HandleKeyboardInput() {}
            virtual ~HandleKeyboardInput() {}
            
            virtual void HandleKeyPressed(const SDL_Keycode &keyCode, const std::string &keyString, const KeyboardState &keyboardState, const Timer &gameTimer) {}
            virtual void HandleKeyReleased(const SDL_Keycode &keyCode, const std::string &keyString, const KeyboardState &keyboardState, const Timer &gameTimer) {}
            virtual void HandleKeyDown(const SDL_Keycode &keyCode, const std::string &keyString, const KeyboardState &keyboardState, const Timer &gameTimer) {}
    };
    
    class HandleMouseInput
    {
        public:
            HandleMouseInput() {}
            virtual ~HandleMouseInput() {}
            
            virtual void HandleMouseMotion(int xMotion, int yMotion, const MouseState &mouseState, const Timer &gameTimer) {}
            virtual void HandleMouseButtonPressed(eMouseButton button, const MouseState &mouseState, const Timer &gameTimer) {}
            virtual void HandleMouseButtonReleased(eMouseButton button, const MouseState &mouseState, const Timer &gameTimer) {}
            virtual void HandleMouseButtonDown(eMouseButton button, const MouseState &mouseState, const Timer &gameTimer) {}
            virtual void HandleMouseWheel(eMouseScrollDirection direction, const MouseState &mouseState, const Timer &gameTimer) {}
    };
    // end input base classes
    
    class InputManager
    {
        public:
            InputManager();
            ~InputManager();

            void Update(const Timer &gameTimer);
            
            void HandleKeyUp(const SDL_KeyboardEvent& key, const Timer &gameTimer);
            void HandleKeyDown(const SDL_KeyboardEvent& key, const Timer &gameTimer);
            void HandleMouseMotion(const SDL_MouseMotionEvent& motion, const Timer &gameTimer);
            void HandleMouseButtonDown(const SDL_MouseButtonEvent& button, const Timer &gameTimer);
            void HandleMouseButtonUp(const SDL_MouseButtonEvent& button, const Timer &gameTimer);
            void HandleMouseWheel(const SDL_MouseWheelEvent& wheel, const Timer &gameTimer);
            
            void RegisterKeyboardHandler(HandleKeyboardInput* keyboardHandler);
            void RegisterMouseHandler(HandleMouseInput* mouseHandler);
            void RemoveKeyboardHandler(HandleKeyboardInput* keyboardHandler);
            void RemoveMouseHandler(HandleMouseInput* mouseHandler);
            
            void ClearAll();
            void ClearKeyboardHandlers();
            void ClearMouseHandlers();
            
            KeyboardState GetKeyboardState();
            KeyboardState GetPrevKeyboardState();
            MouseState GetMouseState();
            MouseState GetPrevMouseState();

        protected:
            std::vector<HandleKeyboardInput*> _keyboardHandlers;
            std::vector<HandleMouseInput*> _mouseHandlers;
            
            KeyboardState _keyboardState;
            KeyboardState _prevKeyboardState;
            
            MouseState _mouseState;
            MouseState _prevMouseState;
            
            void TriggerKeyPressed(const SDL_Keycode &key, const Timer &gameTimer);
            void TriggerKeyReleased(const SDL_Keycode &key, const Timer &gameTimer);
            void TriggerKeyDown(const SDL_Keycode &key, const Timer &gameTimer);
            
            void TriggerMouseButtonPressed(eMouseButton button, const Timer &gameTimer);
            void TriggerMouseButtonReleased(eMouseButton button, const Timer &gameTimer);
            void TriggerMouseButtonDown(eMouseButton button, const Timer &gameTimer);
            void TriggerMouseMotion(int xMotion, int yMotion, const Timer &gameTimer);
            void TriggerMouseWheel(eMouseScrollDirection direction, const Timer &gameTimer);
    };
}

#endif
