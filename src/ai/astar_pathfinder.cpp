#include "astar_pathfinder.h"

PandaSDL::AStarPathfinder::AStarPathfinder()
    : _graph(nullptr), _graphSize(0),
    _isGrid(false), _gridWidth(0), _gridHeight(0), _gridCellWidth(0), _gridCellHeight(0),
    MovementBlockedThreshold(PANDASDL_ASTAR_MOVEMENTBLOCKEDTHRESHOLD)
{
}

PandaSDL::AStarPathfinder::~AStarPathfinder()
{
    Clear();
}

void PandaSDL::AStarPathfinder::Clear()
{
    if (_graph != nullptr)
        delete [] _graph;
    
    _graph = nullptr;
    _graphSize = 0;
}

std::vector<PandaSDL::AStarPathResult> PandaSDL::AStarPathfinder::GeneratePathFromWorldPos(const PandaSDL::Vector2 &start, const PandaSDL::Vector2 &end, bool addStart)
{
    if (_graph == nullptr)
        return std::vector<PandaSDL::AStarPathResult>();
    
    auto startNode = GetNodeByWorldPosition(start);
    auto endNode = GetNodeByWorldPosition(end);
    
    if (startNode == nullptr || endNode == nullptr)
        return std::vector<PandaSDL::AStarPathResult>();
    
    return GeneratePath(startNode, endNode, addStart);
}

std::vector<PandaSDL::AStarPathResult> PandaSDL::AStarPathfinder::GeneratePathFromGraphPos(const PandaSDL::Vector2 &start, const PandaSDL::Vector2 &end, bool addStart)
{
    if (_graph == nullptr)
        return std::vector<PandaSDL::AStarPathResult>();
    
    auto startNode = GetNodeByGraphPosition(start);
    auto endNode = GetNodeByGraphPosition(end);
    
    if (startNode == nullptr || endNode == nullptr)
        return std::vector<PandaSDL::AStarPathResult>();
    
    return GeneratePath(startNode, endNode, addStart);
}

std::vector<PandaSDL::AStarPathResult> PandaSDL::AStarPathfinder::GeneratePath(AStarNode *startNode, AStarNode *endNode, bool addStart)
{
    Reset();
    
    if (_graph == nullptr)
        return std::vector<PandaSDL::AStarPathResult>();
    if (startNode == nullptr || endNode == nullptr)
        return std::vector<PandaSDL::AStarPathResult>();
    if (startNode == endNode)
        return std::vector<PandaSDL::AStarPathResult>();
    if (IsNodeBlocked(endNode))
        return std::vector<PandaSDL::AStarPathResult>();
    
    std::vector<PandaSDL::AStarPathResult> result;
    std::vector<AStarNode*> openList;
    std::vector<AStarNode*> closedList;
    
    bool pathFound = false;
    
    startNode->G = 0.0f;
    startNode->H = GetNodeHeuristic(startNode, endNode);
    startNode->Parent = nullptr;
    CalculateNode(startNode);
    
    openList.push_back(startNode);
    
    while (!pathFound)
    {
        if (openList.size() == 0)
            return std::vector<PandaSDL::AStarPathResult>();
        
        AStarNode *currentNode = openList.back();
        
        if (currentNode == nullptr)
            return std::vector<PandaSDL::AStarPathResult>();
        
        openList.pop_back();
        
        AStarNode *tempNode;
        
        for (auto edgeNode : currentNode->Edges)
        {
            if (AddNodeToOpenList(openList, edgeNode, currentNode, endNode))
            {
                pathFound = true;
                tempNode = edgeNode;
            }
        }
        
        currentNode->Open = false;
        currentNode->Closed = true;
        
        if (pathFound)
        {
            tempNode->Parent = currentNode;
            bool resultEnd = true;
            
            while (tempNode->Parent != nullptr)
            {
                bool resultStart = (tempNode->Parent == nullptr || tempNode->Parent->Parent == nullptr);
                if (addStart)
                    resultStart = false;
                
                result.push_back({ tempNode, resultStart, resultEnd });
                
                tempNode = tempNode->Parent;
                resultEnd = false;
            }
            
            if (addStart)
            {
                result.push_back({ startNode, true, false });
            }
        }
    }
    
    return result;
}

bool PandaSDL::AStarPathfinder::AddNodeToOpenList(std::vector<AStarNode*> &openList, AStarNode *node, AStarNode *parentNode, AStarNode *endNode)
{
    if (node == endNode)
        return true;
    
    if (node->Open || node->Closed)
        return false;
    
    if (IsNodeBlocked(node))
        return false;
    
    auto movementCost = GetNodeCost(node);
    
    node->G = parentNode->G + 1.0f + movementCost;
    node->H = GetNodeHeuristic(node, endNode);
    node->Parent = parentNode;
    node->Open = true;
    CalculateNode(node);
    
    // sort open list
    for (int i = 0; i < openList.size(); i++)
    {
        if (openList[i]->F < node->F)
        {
            openList.insert(openList.begin() + i, node);
            return false;
        }
    }
    
    openList.push_back(node);
    return false;
}

void PandaSDL::AStarPathfinder::Generate4EdgeGrid(unsigned int mapWidth, unsigned int mapHeight, unsigned int cellWidth, unsigned int cellHeight)
{
    std::vector<PandaSDL::Vector2> edgeOffsets = 
    {
        { -1, 0 }, // left
        { 1, 0 }, // right
        { 0, -1 }, // top
        { 0, 1 }, // bottom
    };
    
    GenerateGrid(mapWidth, mapHeight, cellWidth, cellHeight, edgeOffsets);
}

void PandaSDL::AStarPathfinder::Generate8EdgeGrid(unsigned int mapWidth, unsigned int mapHeight, unsigned int cellWidth, unsigned int cellHeight)
{
    std::vector<PandaSDL::Vector2> edgeOffsets = 
    {
        { -1, 0 }, // left
        { 1, 0 }, // right
        { 0, -1 }, // top
        { 0, 1 }, // bottom
        { -1, -1 }, // top left
        { 1, -1 }, // top right
        { -1, 1 }, // bottom left
        { 1, 1 }, // bottom right
    };
    
    GenerateGrid(mapWidth, mapHeight, cellWidth, cellHeight, edgeOffsets);
}

void PandaSDL::AStarPathfinder::GenerateGrid(unsigned int mapWidth, unsigned int mapHeight, unsigned int cellWidth, unsigned int cellHeight, const std::vector<PandaSDL::Vector2> &edgeOffsets)
{
    Clear();
    
    _graph = new AStarNode[mapWidth * mapHeight];
    _graphSize = mapWidth * mapHeight;
    
    _isGrid = true;
    _gridWidth = mapWidth;
    _gridHeight = mapHeight;
    _gridCellWidth = cellWidth;
    _gridCellHeight = cellHeight;
    
    // init grid based graph
    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            _graph[x + mapWidth * y].GraphPosition = { x, y };
            _graph[x + mapWidth * y].WorldPosition = { x * cellWidth, y * cellHeight };
            _graph[x + mapWidth * y].MovementCost = 0.0f;
        }
    }
    
    // set edges on grid based graph
    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            for (const auto &offset : edgeOffsets)
            {
                auto offsetPos = PandaSDL::Vector2(x, y) + offset;
                
                if (offsetPos.X >= 0 && offsetPos.X < mapWidth && offsetPos.Y >= 0 && offsetPos.Y < mapHeight)
                    _graph[x + mapWidth * y].Edges.push_back(&_graph[(int)(offsetPos.X + mapWidth * offsetPos.Y)]);
            }
        }
    }
    
    Reset();
}

void PandaSDL::AStarPathfinder::SetGraph(AStarNode *graph, unsigned int size)
{
    Clear();
    
    _graph = graph;
    _graphSize = size;
    
    Reset();
}

void PandaSDL::AStarPathfinder::SetGraph(std::vector<AStarNode> graph)
{
    SetGraph(&graph[0], graph.size());
}

PandaSDL::AStarNode* PandaSDL::AStarPathfinder::GetNodeByGraphPosition(const PandaSDL::Vector2 &graphPosition)
{
    if (_isGrid)
    {
        return &_graph[(int)(graphPosition.X + _gridWidth * graphPosition.Y)];
    }
    else
    {
        for (int i = 0; i < _graphSize; i++)
        {
            if (_graph[i].GraphPosition == graphPosition)
                return &_graph[i];
        }
    }
    
    return nullptr;
}

PandaSDL::AStarNode* PandaSDL::AStarPathfinder::GetNodeByWorldPosition(const PandaSDL::Vector2 &worldPosition)
{
    if (_isGrid)
    {
        auto graphPosition = PandaSDL::Vector2(worldPosition.X / _gridCellWidth, worldPosition.Y / _gridCellHeight);
        return GetNodeByGraphPosition(graphPosition);
    }
    else
    {
        for (int i = 0; i < _graphSize; i++)
        {
            if (_graph[i].WorldPosition == worldPosition)
                return &_graph[i];
        }
    }
    
    return nullptr;
}


float PandaSDL::AStarPathfinder::GetNodeCost(AStarNode *node)
{
    return node->MovementCost;
}

float PandaSDL::AStarPathfinder::GetNodeHeuristic(AStarNode *node, AStarNode *end)
{
    return node->WorldPosition.Distance(end->WorldPosition);
}

bool PandaSDL::AStarPathfinder::IsNodeBlocked(AStarNode *node)
{
    return (node->MovementCost <= MovementBlockedThreshold);
}

void PandaSDL::AStarPathfinder::CalculateNode(AStarNode *node)
{
    node->F = node->G + node->H;
}

void PandaSDL::AStarPathfinder::Reset()
{
    for (int i = 0; i < _graphSize; i++)
    {
        _graph[i].Parent = nullptr;
        _graph[i].Open = false;
        _graph[i].Closed = false;
        _graph[i].G = 0.0f;
        _graph[i].H = 0.0f;
        _graph[i].F = 0.0f;
    }
}
