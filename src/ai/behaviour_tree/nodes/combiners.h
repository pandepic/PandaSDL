#ifndef __pandasdl_behaviourtree_combiners_h__
#define __pandasdl_behaviourtree_combiners_h__

#include "base.h"

namespace PandaSDL::BehaviourTree
{
    /*
        Run all children, waiting on each to complete, regardless of their success
    */
    class Combiner : public BehaviourTreeNode
    {
        public:
            Combiner();
            ~Combiner();
            
            virtual eNodeStatus Run() override;
        
        protected:
            unsigned int _nodeIndex;
            std::vector<std::shared_ptr<BehaviourTreeNode>> _children;
    };
    
    /*
        Run all children, waiting on each to complete, until they all finish or one returns failure
    */
    class CombinerUntilFailure : public BehaviourTreeNode
    {
        public:
            CombinerUntilFailure();
            ~CombinerUntilFailure();
            
            virtual eNodeStatus Run() override;
            
        protected:
            unsigned int _nodeIndex;
            std::vector<std::shared_ptr<BehaviourTreeNode>> _children;
    };
}

#endif