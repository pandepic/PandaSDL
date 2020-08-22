#include "combiners.h"

PandaSDL::BehaviourTree::Combiner::Combiner()
    : _nodeIndex(0)
{
}

PandaSDL::BehaviourTree::Combiner::~Combiner()
{
}

PandaSDL::BehaviourTree::eNodeStatus PandaSDL::BehaviourTree::Combiner::Run()
{
    for (unsigned int i = _nodeIndex; i < _children.size(); i++)
    {
        auto result = _children[i]->Run();
        
        if (result == eNodeStatus::RUNNING)
            return eNodeStatus::RUNNING;
        else
            _nodeIndex += 1;
    }
    
    return eNodeStatus::SUCCESS;
}

PandaSDL::BehaviourTree::CombinerUntilFailure::CombinerUntilFailure()
    : _nodeIndex(0)
{
}

PandaSDL::BehaviourTree::CombinerUntilFailure::~CombinerUntilFailure()
{
}

PandaSDL::BehaviourTree::eNodeStatus PandaSDL::BehaviourTree::CombinerUntilFailure::Run()
{
    for (unsigned int i = _nodeIndex; i < _children.size(); i++)
    {
        auto result = _children[i]->Run();
        
        if (result == eNodeStatus::RUNNING)
            return eNodeStatus::RUNNING;
        else if (result == eNodeStatus::FAILURE)
            return eNodeStatus::FAILURE;
        else
            _nodeIndex += 1;
    }
    
    return eNodeStatus::SUCCESS;
}
