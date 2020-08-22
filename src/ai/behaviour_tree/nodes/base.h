#ifndef __pandasdl_behaviourtree_nodes_base_h__
#define __pandasdl_behaviourtree_nodes_base_h__

namespace PandaSDL::BehaviourTree
{
    enum class eNodeStatus
    {
        RUNNING,
        SUCCESS,
        FAILURE
    };
    
    // base tree node class, does nothing
    class BehaviourTreeNode
    {
        public:
            BehaviourTreeNode() {}
            virtual ~BehaviourTreeNode() {}
            
            virtual eNodeStatus Run() = 0;
    };
}

#endif