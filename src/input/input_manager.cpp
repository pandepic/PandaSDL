#include "input_manager.h"

bool PandaSDL::KeyboardState::IsKeyUp(const SDL_Keycode &key) const
{
    auto find = std::find(PressedKeys.begin(), PressedKeys.end(), key);
    
    if (find != PressedKeys.end())
        return false;
    
    return true;
}

bool PandaSDL::KeyboardState::IsKeyDown(const SDL_Keycode &key) const
{
    auto find = std::find(PressedKeys.begin(), PressedKeys.end(), key);
    
    if (find != PressedKeys.end())
        return true;
    
    return false;
}

PandaSDL::InputManager::InputManager()
{
}

PandaSDL::InputManager::~InputManager()
{
    ClearAll();
}

void PandaSDL::InputManager::Update(const Timer& gameTimer)
{
    // KEYBOARD
    if (_keyboardState.PressedKeys.size() > 0)
    {
        for (const auto &key : _keyboardState.PressedKeys)
        {
            if (_prevKeyboardState.IsKeyUp(key))
            {
                TriggerKeyPressed(key, gameTimer);
            }
            else
            {
                TriggerKeyDown(key, gameTimer);
            }
        }
    }
    
    if (_prevKeyboardState.PressedKeys.size() > 0)
    {
        for (const auto &key : _prevKeyboardState.PressedKeys)
        {
            if (_keyboardState.IsKeyUp(key))
            {
                TriggerKeyReleased(key, gameTimer);
            }
        }
    }
    // END KEYBOARD
    
    // MOUSE
    if (_mouseState.ButtonLeft == PandaSDL::eMouseButtonState::PRESSED)
    {
        if (_prevMouseState.ButtonLeft == PandaSDL::eMouseButtonState::RELEASED)
            TriggerMouseButtonPressed(PandaSDL::eMouseButton::LEFT, gameTimer);
        else
            TriggerMouseButtonDown(PandaSDL::eMouseButton::LEFT, gameTimer);
    }
    else
    {
        if (_prevMouseState.ButtonLeft == PandaSDL::eMouseButtonState::PRESSED)
            TriggerMouseButtonReleased(PandaSDL::eMouseButton::LEFT, gameTimer);
    }
    
    if (_mouseState.ButtonMiddle == PandaSDL::eMouseButtonState::PRESSED)
    {
        if (_prevMouseState.ButtonMiddle == PandaSDL::eMouseButtonState::RELEASED)
            TriggerMouseButtonPressed(PandaSDL::eMouseButton::MIDDLE, gameTimer);
        else
            TriggerMouseButtonDown(PandaSDL::eMouseButton::MIDDLE, gameTimer);
    }
    else
    {
        if (_prevMouseState.ButtonMiddle == PandaSDL::eMouseButtonState::PRESSED)
            TriggerMouseButtonReleased(PandaSDL::eMouseButton::MIDDLE, gameTimer);
    }
    
    if (_mouseState.ButtonRight == PandaSDL::eMouseButtonState::PRESSED)
    {
        if (_prevMouseState.ButtonRight == PandaSDL::eMouseButtonState::RELEASED)
            TriggerMouseButtonPressed(PandaSDL::eMouseButton::RIGHT, gameTimer);
        else
            TriggerMouseButtonDown(PandaSDL::eMouseButton::RIGHT, gameTimer);
    }
    else
    {
        if (_prevMouseState.ButtonRight == PandaSDL::eMouseButtonState::PRESSED)
            TriggerMouseButtonReleased(PandaSDL::eMouseButton::RIGHT, gameTimer);
    }
    // END MOUSE
    
    _prevKeyboardState = _keyboardState;
    _prevMouseState = _mouseState;
} // Update

void PandaSDL::InputManager::HandleKeyUp(const SDL_KeyboardEvent& key, const Timer& gameTimer)
{
    auto keyCode = key.keysym.sym;
    auto keyString = std::string(SDL_GetKeyName(keyCode));
    
    auto find = std::find(_keyboardState.PressedKeys.begin(), _keyboardState.PressedKeys.end(), keyCode);
    if (find != _keyboardState.PressedKeys.end())
        _keyboardState.PressedKeys.erase(std::remove(_keyboardState.PressedKeys.begin(), _keyboardState.PressedKeys.end(), keyCode), _keyboardState.PressedKeys.end());
}

void PandaSDL::InputManager::HandleKeyDown(const SDL_KeyboardEvent& key, const Timer& gameTimer)
{
    auto keyCode = key.keysym.sym;
    auto keyString = std::string(SDL_GetKeyName(keyCode));
    
    auto find = std::find(_keyboardState.PressedKeys.begin(), _keyboardState.PressedKeys.end(), keyCode);
    if (find == _keyboardState.PressedKeys.end())
        _keyboardState.PressedKeys.push_back(keyCode);
}

void PandaSDL::InputManager::HandleMouseMotion(const SDL_MouseMotionEvent& motion, const Timer& gameTimer)
{
    _mouseState.Position.X = motion.x;
    _mouseState.Position.Y = motion.y;
    
    TriggerMouseMotion(motion.xrel, motion.yrel, gameTimer);
}

void PandaSDL::InputManager::HandleMouseButtonDown(const SDL_MouseButtonEvent& button, const Timer& gameTimer)
{
    switch (button.button)
    {
        case SDL_BUTTON_LEFT:
        {
            _mouseState.ButtonLeft = PandaSDL::eMouseButtonState::PRESSED;
        }
        break;
        
        case SDL_BUTTON_MIDDLE:
        {
            _mouseState.ButtonMiddle = PandaSDL::eMouseButtonState::PRESSED;
        }
        break;
        
        case SDL_BUTTON_RIGHT:
        {
            _mouseState.ButtonRight = PandaSDL::eMouseButtonState::PRESSED;
        }
        break;
    }
}

void PandaSDL::InputManager::HandleMouseButtonUp(const SDL_MouseButtonEvent& button, const Timer& gameTimer)
{
    switch (button.button)
    {
        case SDL_BUTTON_LEFT:
        {
            _mouseState.ButtonLeft = PandaSDL::eMouseButtonState::RELEASED;
        }
        break;
        
        case SDL_BUTTON_MIDDLE:
        {
            _mouseState.ButtonMiddle = PandaSDL::eMouseButtonState::RELEASED;
        }
        break;
        
        case SDL_BUTTON_RIGHT:
        {
            _mouseState.ButtonRight = PandaSDL::eMouseButtonState::RELEASED;
        }
        break;
    }
}

void PandaSDL::InputManager::HandleMouseWheel(const SDL_MouseWheelEvent& wheel, const Timer& gameTimer)
{
    int wheelX, wheelY;
    
    if (wheel.direction == SDL_MOUSEWHEEL_FLIPPED)
    {
        wheelX = wheel.x * -1;
        wheelY = wheel.y * -1;
    }
    else
    {
        wheelX = wheel.x;
        wheelY = wheel.y;
    }
    
    if (wheelX > 0)
    {
        TriggerMouseWheel(PandaSDL::eMouseScrollDirection::RIGHT, gameTimer);
    }
    else if (wheelX < 0)
    {
        TriggerMouseWheel(PandaSDL::eMouseScrollDirection::LEFT, gameTimer);
    }
    
    if (wheelY > 0)
    {
        TriggerMouseWheel(PandaSDL::eMouseScrollDirection::UP, gameTimer);
    }
    else if (wheelY < 0)
    {
        TriggerMouseWheel(PandaSDL::eMouseScrollDirection::DOWN, gameTimer);
    }
} // HandleMouseWheel

void PandaSDL::InputManager::RegisterKeyboardHandler(HandleKeyboardInput* keyboardHandler)
{
    if (keyboardHandler == nullptr)
        return;
    
    _keyboardHandlers.push_back(keyboardHandler);
}

void PandaSDL::InputManager::RegisterMouseHandler(HandleMouseInput* mouseHandler)
{
    if (mouseHandler == nullptr)
        return;
    
    _mouseHandlers.push_back(mouseHandler);
}

void PandaSDL::InputManager::RemoveKeyboardHandler(HandleKeyboardInput* keyboardHandler)
{
    auto find = std::find(_keyboardHandlers.begin(), _keyboardHandlers.end(), keyboardHandler);
    if (find != _keyboardHandlers.end())
       _keyboardHandlers.erase(std::remove(_keyboardHandlers.begin(), _keyboardHandlers.end(), keyboardHandler), _keyboardHandlers.end());
}

void PandaSDL::InputManager::RemoveMouseHandler(HandleMouseInput* mouseHandler)
{
    auto find = std::find(_mouseHandlers.begin(), _mouseHandlers.end(), mouseHandler);
    if (find != _mouseHandlers.end())
       _mouseHandlers.erase(std::remove(_mouseHandlers.begin(), _mouseHandlers.end(), mouseHandler), _mouseHandlers.end());
}

void PandaSDL::InputManager::ClearAll()
{
    ClearKeyboardHandlers();
    ClearMouseHandlers();
}

void PandaSDL::InputManager::ClearKeyboardHandlers()
{
    _keyboardHandlers.clear();
}

void PandaSDL::InputManager::ClearMouseHandlers()
{
    _mouseHandlers.clear();
}

void PandaSDL::InputManager::TriggerKeyPressed(const SDL_Keycode &key, const Timer& gameTimer)
{
    auto keyString = std::string(SDL_GetKeyName(key));
    
    for (auto keyboardHandler : _keyboardHandlers)
    {
        keyboardHandler->HandleKeyPressed(key, keyString, _keyboardState, gameTimer);
    }
}

void PandaSDL::InputManager::TriggerKeyReleased(const SDL_Keycode &key, const Timer& gameTimer)
{
    auto keyString = std::string(SDL_GetKeyName(key));
    
    for (auto keyboardHandler : _keyboardHandlers)
    {
        keyboardHandler->HandleKeyReleased(key, keyString, _keyboardState, gameTimer);
    }
}

void PandaSDL::InputManager::TriggerKeyDown(const SDL_Keycode &key, const Timer& gameTimer)
{
    auto keyString = std::string(SDL_GetKeyName(key));
    
    for (auto keyboardHandler : _keyboardHandlers)
    {
        keyboardHandler->HandleKeyDown(key, keyString, _keyboardState, gameTimer);
    }
}

void PandaSDL::InputManager::TriggerMouseButtonPressed(eMouseButton button, const Timer& gameTimer)
{
    for (auto mouseHandler : _mouseHandlers)
    {
        mouseHandler->HandleMouseButtonPressed(button, _mouseState, gameTimer);
    }
}

void PandaSDL::InputManager::TriggerMouseButtonReleased(eMouseButton button, const Timer& gameTimer)
{
    for (auto mouseHandler : _mouseHandlers)
    {
        mouseHandler->HandleMouseButtonReleased(button, _mouseState, gameTimer);
    }
}

void PandaSDL::InputManager::TriggerMouseButtonDown(eMouseButton button, const Timer& gameTimer)
{
    for (auto mouseHandler : _mouseHandlers)
    {
        mouseHandler->HandleMouseButtonDown(button, _mouseState, gameTimer);
    }
}

void PandaSDL::InputManager::TriggerMouseMotion(int xMotion, int yMotion, const Timer& gameTimer)
{
    for (auto mouseHandler : _mouseHandlers)
    {
        mouseHandler->HandleMouseMotion(xMotion, yMotion, _mouseState, gameTimer);
    }
}

void PandaSDL::InputManager::TriggerMouseWheel(eMouseScrollDirection direction, const Timer& gameTimer)
{
    for (auto mouseHandler : _mouseHandlers)
    {
        mouseHandler->HandleMouseWheel(direction, _mouseState, gameTimer);
    }
}

PandaSDL::KeyboardState PandaSDL::InputManager::GetKeyboardState()
{
    return _keyboardState;
}

PandaSDL::KeyboardState PandaSDL::InputManager::GetPrevKeyboardState()
{
    return _prevKeyboardState;
}

PandaSDL::MouseState PandaSDL::InputManager::GetMouseState()
{
    return _mouseState;
}

PandaSDL::MouseState PandaSDL::InputManager::GetPrevMouseState()
{
    return _prevMouseState;
}
