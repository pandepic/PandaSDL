#include "game_control_manager.h"

PandaSDL::GameControlManager::GameControlManager()
    : _inputRegistered(false)
{
}

PandaSDL::GameControlManager::~GameControlManager()
{
    if (_inputRegistered)
    {
        PandaSDL::Game::InputManager.RemoveKeyboardHandler(this);
        PandaSDL::Game::InputManager.RemoveMouseHandler(this);
        _inputRegistered = false;
    }
    
    _gameControlHandlers.clear();
}

void PandaSDL::GameControlManager::Setup(std::string settingsSection)
{
    PandaSDL::Game::InputManager.RegisterKeyboardHandler(this);
    PandaSDL::Game::InputManager.RegisterMouseHandler(this);
    _inputRegistered = true;
    
    auto settings = PandaSDL::Game::SettingsManager.GetSettings(settingsSection);
    
    for (auto &setting : settings)
    {
        auto controlType = PandaSDL::StringToGameControlType[setting.Attributes["Type"]];
        
        switch (controlType)
        {
            case PandaSDL::eGameControlType::KEYBOARD:
            {
                PandaSDL::KeyboardGameControl newControl;
                
                newControl.Name = setting.Name;
                newControl.State = PandaSDL::StringToKeyboardButtonState[setting.Attributes["State"]];
                
                auto controlComboSplit = PandaSDL::SplitString(setting.Value, ",");
                
                for (const auto &controlComboInfo : controlComboSplit)
                {
                    std::vector<std::string> controlKeyList;
                    auto controlSplit = PandaSDL::SplitString(controlComboInfo, "+");
                    
                    for (const auto &controlInfo : controlSplit)
                    {
                        controlKeyList.push_back(controlInfo);
                    }
                    
                    newControl.ControlKeys.push_back(controlKeyList);
                }
                
                _keyboardControls.insert(std::pair<std::string, PandaSDL::KeyboardGameControl>(newControl.Name, newControl));
            }
            break;
            
            case PandaSDL::eGameControlType::MOUSE:
            {
                
            }
            break;
        }
    }
}

void PandaSDL::GameControlManager::RegisterGameControlHandler(HandleGameControls* gameControlHandler)
{
    if (gameControlHandler == nullptr)
        return;
    
    _gameControlHandlers.push_back(gameControlHandler);
}

void PandaSDL::GameControlManager::RemoveGameControlHandler(HandleGameControls* gameControlHandler)
{
    auto find = std::find(_gameControlHandlers.begin(), _gameControlHandlers.end(), gameControlHandler);
    if (find != _gameControlHandlers.end())
       _gameControlHandlers.erase(std::remove(_gameControlHandlers.begin(), _gameControlHandlers.end(), gameControlHandler), _gameControlHandlers.end());
}

void PandaSDL::GameControlManager::TriggerGameControl(const std::string &controlName, const Timer& gameTimer)
{
    auto keyboardState = PandaSDL::Game::InputManager.GetKeyboardState();
    auto mouseState = PandaSDL::Game::InputManager.GetMouseState();
    
    for (const auto &gameControlHandler : _gameControlHandlers)
    {
        gameControlHandler->HandleGameControl(controlName, keyboardState, mouseState, gameTimer);
    }
}

void PandaSDL::GameControlManager::HandleKeyPressed(const SDL_Keycode &keyCode, const std::string &keyString, const PandaSDL::KeyboardState &keyboardState, const Timer& gameTimer)
{
    for (const auto &[key, val] : _keyboardControls)
    {
        if (val.State == PandaSDL::eKeyboardButtonState::PRESSED)
            CheckKeyboardControl(val, keyString, keyboardState, gameTimer);
    }
}

void PandaSDL::GameControlManager::HandleKeyReleased(const SDL_Keycode &keyCode, const std::string &keyString, const PandaSDL::KeyboardState &keyboardState, const Timer& gameTimer)
{
    for (const auto &[key, val] : _keyboardControls)
    {
        if (val.State == PandaSDL::eKeyboardButtonState::RELEASED)
            CheckKeyboardControl(val, keyString, keyboardState, gameTimer);
    }
}

void PandaSDL::GameControlManager::HandleKeyDown(const SDL_Keycode &keyCode, const std::string &keyString, const PandaSDL::KeyboardState &keyboardState, const Timer& gameTimer)
{
    for (const auto &[key, val] : _keyboardControls)
    {
        if (val.State == PandaSDL::eKeyboardButtonState::DOWN)
            CheckKeyboardControl(val, keyString, keyboardState, gameTimer);
    }
}

void PandaSDL::GameControlManager::CheckKeyboardControl(const PandaSDL::KeyboardGameControl &keyboardControl, const std::string &keyString, const PandaSDL::KeyboardState &keyboardState, const Timer& gameTimer)
{
    for (const auto &controlKeyList : keyboardControl.ControlKeys)
    {
        bool mainKey = false;
        bool otherKeys = false;
        bool firstOtherKey = true;
        
        if (controlKeyList.size() == 1)
            otherKeys = true;
        
        for (const auto &controlKey : controlKeyList)
        {
            if (controlKey == keyString)
            {
                mainKey = true;
            }
            else
            {
                auto keyFound = false;
                
                auto keyCode = SDL_GetKeyFromName(controlKey.c_str());
                if (keyboardState.IsKeyDown(keyCode))
                    keyFound = true;
                
                if (firstOtherKey && keyFound)
                {
                    otherKeys = true;
                    firstOtherKey = false;
                }
                else
                {
                    otherKeys = otherKeys && keyFound;
                }
            }
        }
        
        if (mainKey && otherKeys)
            TriggerGameControl(keyboardControl.Name, gameTimer);
    }
}

void PandaSDL::GameControlManager::HandleMouseMotion(int xMotion, int yMotion, const PandaSDL::MouseState &mouseState, const Timer& gameTimer)
{
}

void PandaSDL::GameControlManager::HandleMouseButtonPressed(PandaSDL::eMouseButton button, const PandaSDL::MouseState &mouseState, const Timer& gameTimer)
{
}

void PandaSDL::GameControlManager::HandleMouseButtonReleased(PandaSDL::eMouseButton button, const PandaSDL::MouseState &mouseState, const Timer& gameTimer)
{
}

void PandaSDL::GameControlManager::HandleMouseButtonDown(PandaSDL::eMouseButton button, const PandaSDL::MouseState &mouseState, const Timer& gameTimer)
{
}

void PandaSDL::GameControlManager::HandleMouseWheel(PandaSDL::eMouseScrollDirection direction, const PandaSDL::MouseState &mouseState, const Timer& gameTimer)
{
}