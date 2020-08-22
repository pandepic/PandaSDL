#include "conditionals.h"

PandaSDL::BehaviourTree::FilterWhenTrue::FilterWhenTrue(std::function<bool()> condition)
    : _condition(condition)
{
}

PandaSDL::BehaviourTree::FilterWhenTrue::~FilterWhenTrue()
{
}

PandaSDL::BehaviourTree::eNodeStatus PandaSDL::BehaviourTree::FilterWhenTrue::Run()
{
    auto result = _condition();
    
    if (result)
    {
        _child->Run();
        return eNodeStatus::SUCCESS;
    }
    else
    {
        return eNodeStatus::FAILURE;
    }
}
