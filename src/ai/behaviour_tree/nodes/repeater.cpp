#include "repeater.h"

PandaSDL::BehaviourTree::Repeater::Repeater()
{
}

PandaSDL::BehaviourTree::Repeater::~Repeater()
{
}

PandaSDL::BehaviourTree::eNodeStatus PandaSDL::BehaviourTree::Repeater::Run()
{
    auto result = _child->Run();
    
    if (result == eNodeStatus::RUNNING)
        return eNodeStatus::RUNNING;
    else
        return eNodeStatus::SUCCESS;
}

PandaSDL::BehaviourTree::RepeatUntilFailure::RepeatUntilFailure()
{
}

PandaSDL::BehaviourTree::RepeatUntilFailure::~RepeatUntilFailure()
{
}

PandaSDL::BehaviourTree::eNodeStatus PandaSDL::BehaviourTree::RepeatUntilFailure::Run()
{
    auto result = _child->Run();
    
    if (result == eNodeStatus::RUNNING)
        return eNodeStatus::RUNNING;
    else if (result == eNodeStatus::SUCCESS)
        return eNodeStatus::RUNNING;
    else
        return eNodeStatus::SUCCESS;
}
