#include "simple_state_machine.h"

void PandaSDL::SimpleStateTimedBase::Update(const PandaSDL::Timer &gameTimer)
{
    Duration -= gameTimer.GetTicks();
    
    if (Duration <= 0)
        EndDuration();
}

PandaSDL::SimpleStateMachine::SimpleStateMachine()
    : _currentState(nullptr), _nextState(nullptr), _prevState(nullptr)
{
}

PandaSDL::SimpleStateMachine::~SimpleStateMachine()
{
    _states.clear();
}

void PandaSDL::SimpleStateMachine::RegisterState(std::shared_ptr<PandaSDL::SimpleStateBase> state)
{
    if (_states.count(state->Name) > 0)
        return;
    
    state->Parent = this;
    _states.insert(std::pair<std::string, std::shared_ptr<PandaSDL::SimpleStateBase>>(state->Name, state));
}

void PandaSDL::SimpleStateMachine::RemoveState(std::string name)
{
    if (_states.count(name) == 0)
        return;
    
    _states.erase(name);
}

void PandaSDL::SimpleStateMachine::RemoveState(std::shared_ptr<PandaSDL::SimpleStateBase> state)
{
    std::string name = "";
    
    for (const auto &[key, val] : _states)
    {
        if (val == state)
            name = key;
    }
    
    if (name.length() > 0)
        _states.erase(name);
}

void PandaSDL::SimpleStateMachine::SetState(std::string name, bool allowSame)
{
    if (_states.count(name) == 0)
        return;
    
    if (allowSame || _currentState == nullptr || (_currentState->Name != name))
        SetCurrentStateInternal(_states[name]);
}

void PandaSDL::SimpleStateMachine::SetState(std::shared_ptr<PandaSDL::SimpleStateBase> state, bool allowSame)
{
    std::string name = "";
    
    for (const auto &[key, val] : _states)
    {
        if (val == state)
            name = key;
    }
    
    if (name.length() > 0)
    {
        if (allowSame || _currentState == nullptr || (_currentState->Name != state->Name))
            SetCurrentStateInternal(_states[name]);
    }
}

void PandaSDL::SimpleStateMachine::SetNextState(std::string name, bool allowSame)
{
    if (_states.count(name) == 0)
        return;
    
    if (allowSame || _nextState == nullptr || (_nextState->Name != name))
        _nextState = _states[name];
}

void PandaSDL::SimpleStateMachine::SetNextState(std::shared_ptr<PandaSDL::SimpleStateBase> state, bool allowSame)
{
    std::string name = "";
    
    for (const auto &[key, val] : _states)
    {
        if (val == state)
            name = key;
    }
    
    if (name.length() > 0)
    {
        if (allowSame || _nextState == nullptr || (_nextState->Name != state->Name))
            _nextState = _states[name];
    }
}

void PandaSDL::SimpleStateMachine::Start(std::string initialState)
{
    SetCurrentStateInternal(_states[initialState]);
}

void PandaSDL::SimpleStateMachine::Update(const PandaSDL::Timer &gameTimer)
{
    _currentState->Update(gameTimer);
    
    if (_nextState != nullptr)
    {
        SetCurrentStateInternal(_nextState);
        _nextState = nullptr;
    }
}

void PandaSDL::SimpleStateMachine::SetCurrentStateInternal(std::shared_ptr<SimpleStateBase> state)
{
    if (_currentState != nullptr)
    {
        _currentState->End();
        _prevState = _currentState;
    }
    
    _currentState = state;
    _currentState->Begin();
}
