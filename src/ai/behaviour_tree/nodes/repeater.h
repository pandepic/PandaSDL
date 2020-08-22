#ifndef __pandasdl_behaviourtree_repeater_h__
#define __pandasdl_behaviourtree_repeater_h__

#include "base.h"

namespace PandaSDL::BehaviourTree
{
    /*
        Repeats child regardless of the returned status and never finishes.
        Intended for use as the root node of a tree.
    */
    class Repeater : public BehaviourTreeNode
    {
        public:
            Repeater();
            ~Repeater();
            
            virtual eNodeStatus Run() override;
            
        protected:
            std::shared_ptr<BehaviourTreeNode> _child;
    };
    
    /*
        Repeats child until it fails.
    */
    class RepeatUntilFailure : public BehaviourTreeNode
    {
        public:
            RepeatUntilFailure();
            ~RepeatUntilFailure();
            
            virtual eNodeStatus Run() override;
            
        protected:
            std::shared_ptr<BehaviourTreeNode> _child;
    };
}

#endif