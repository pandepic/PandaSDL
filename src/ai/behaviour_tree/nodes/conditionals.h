#ifndef __pandasdl_behaviourtree_conditionals_h__
#define __pandasdl_behaviourtree_conditionals_h__

#include "base.h"

namespace PandaSDL::BehaviourTree
{
    class FilterWhenTrue : public BehaviourTreeNode
    {
        public:
            FilterWhenTrue(std::function<bool()> condition);
            ~FilterWhenTrue();
            
            virtual eNodeStatus Run() override;
            
        protected:
            std::function<bool()> _condition;
            std::shared_ptr<BehaviourTreeNode> _child;
    };
}

#endif