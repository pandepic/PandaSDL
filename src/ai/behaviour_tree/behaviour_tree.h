#ifndef __pandasdl_behaviourtree_h__
#define __pandasdl_behaviourtree_h__

#include "nodes/base.h"

namespace PandaSDL::BehaviourTree
{
    class BehaviourTree
    {
        public:
            BehaviourTree();
            ~BehaviourTree();
            
        protected:
            std::shared_ptr<BehaviourTreeNode> _rootNode;
    };
}

#endif