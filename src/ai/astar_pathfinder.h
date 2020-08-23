#ifndef __pandasdl_astarpathfinder_h__
#define __pandasdl_astarpathfinder_h__

#include "../math/vector2.h"

#define PANDASDL_ASTAR_MOVEMENTBLOCKEDTHRESHOLD -1.0f

namespace PandaSDL
{
    struct AStarNode
    {
        AStarNode() {}
        
        PandaSDL::Vector2 GraphPosition;
        PandaSDL::Vector2 WorldPosition;
        std::vector<AStarNode*> Edges;
        float MovementCost;
        
        // temp values that are reset after path generation
        AStarNode *Parent;
        bool Open, Closed;
        float G, H, F;
    };
    
    struct AStarPathData
    {
        AStarPathData() {}
        AStarPathData(PandaSDL::Vector2 graphPosition, PandaSDL::Vector2 worldPosition) : GraphPosition(graphPosition), WorldPosition(worldPosition) {}
        
        PandaSDL::Vector2 GraphPosition;
        PandaSDL::Vector2 WorldPosition;
    };
    
    struct AStarPathResult
    {
        AStarPathResult() {}
        
        AStarPathResult(AStarNode *node, bool start = false, bool end = false) : Start(start), End(end), HasParent(false)
        {
            ResultData = { node->GraphPosition, node->WorldPosition };
            
            if (node->Parent != nullptr)
            {
                HasParent = true;
                ParentData = { node->Parent->GraphPosition, node->Parent->WorldPosition };
            }
        }
        
        AStarPathResult(AStarPathData resultData, bool start = false, bool end = false)
        : ResultData(resultData), HasParent(false), Start(start), End(end) {}
        
        AStarPathResult(AStarPathData resultData, AStarPathData parentData, bool hasParent = true, bool start = false, bool end = false)
        : ResultData(resultData), ParentData(parentData), HasParent(hasParent), Start(start), End(end) {}
        
        AStarPathData ResultData;
        AStarPathData ParentData;
        bool HasParent;
        bool Start, End;
    };
    
    class AStarPathfinder
    {
        public:
            AStarPathfinder();
            virtual ~AStarPathfinder();
            
            float MovementBlockedThreshold; // movement cost values below this will be impassable, defaults to PANDASDL_ASTAR_MOVEMENTBLOCKEDTHRESHOLD
            
            std::vector<PandaSDL::AStarPathResult> GeneratePathFromWorldPos(const PandaSDL::Vector2 &start, const PandaSDL::Vector2 &end, bool addStart = false);
            std::vector<PandaSDL::AStarPathResult> GeneratePathFromGraphPos(const PandaSDL::Vector2 &start, const PandaSDL::Vector2 &end, bool addStart = false);
            std::vector<PandaSDL::AStarPathResult> GeneratePath(AStarNode *startNode, AStarNode *endNode, bool addStart = false);
            
            void Generate4EdgeGrid(unsigned int mapWidth, unsigned int mapHeight, unsigned int cellWidth, unsigned int cellHeight, bool lazyLoad = false);
            void Generate8EdgeGrid(unsigned int mapWidth, unsigned int mapHeight, unsigned int cellWidth, unsigned int cellHeight, bool lazyLoad = false);
            
            void SetGraph(AStarNode *graph, unsigned int size);
            void SetGraph(std::vector<AStarNode> graph);
            
            PandaSDL::AStarNode* GetNodeByGraphPosition(const PandaSDL::Vector2 &graphPosition);
            PandaSDL::AStarNode* GetNodeByWorldPosition(const PandaSDL::Vector2 &worldPosition);
            
            virtual float GetNodeCost(AStarNode *node);
            virtual float GetNodeHeuristic(AStarNode *node, AStarNode *end);
            virtual bool IsNodeBlocked(AStarNode *node);
            
            void Reset();
            void Clear();
            
        protected:
            AStarNode *_graph;
            unsigned int _graphSize;
            std::function<bool(PandaSDL::AStarNode*)> _nodeCostFunction;
            
            bool _isGrid;
            unsigned int _gridWidth, _gridHeight;
            unsigned int _gridCellWidth, _gridCellHeight;
            std::vector<PandaSDL::Vector2> _gridEdgeOffsets;
            
            void GenerateGrid(unsigned int mapWidth, unsigned int mapHeight, unsigned int cellWidth, unsigned int cellHeight, const std::vector<PandaSDL::Vector2> &edgeOffsets, bool lazyLoad = false);
            bool AddNodeToOpenList(std::vector<AStarNode*> &openList, AStarNode *node, AStarNode *parentNode, AStarNode *endNode);
            void CalculateNode(AStarNode *node);
    };
}

#endif