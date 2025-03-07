#ifndef ASTAR_H
#define ASTAR_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>

struct Node 
{
    sf::Vector2i position; // Grid position
    float gCost;           // Cost from start to this node
    float hCost;           // Heuristic cost to goal
    float fCost() const { return gCost + hCost; } // Total cost
    Node* parent;          // Parent node for path reconstruction

    bool operator<(const Node& other) const 
    {
        return fCost() > other.fCost(); // For priority queue (lower fCost has higher priority)
    }
};

class AstarUnit
{
public:
    sf::Vector2f position;
    float size;
};

class Astar
{
public:
    Astar();

    // Compute a path from start to target
    std::vector<sf::Vector2f> FindPath(sf::Vector2f start, float pathfindingUnitSize);
    void setTarget(sf::Vector2f t_position);
    void setUnitPositions(std::vector<AstarUnit> t_positions) { unitPositions = t_positions; }

private:
    sf::Vector2u gridSize{ 50,50 };
    int cellSize = 25;
    sf::Vector2f target;
    std::vector<AstarUnit> unitPositions;

    std::vector<sf::Vector2i> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}, // Straight
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1} // Diagonal
    };

    // Convert world position to grid coordinates
    sf::Vector2i WorldToGrid(sf::Vector2f worldPos);

    // Convert grid coordinates to index
    int GridToIndex(sf::Vector2i gridPos);

    // Heuristic function (Manhattan distance)
    float Heuristic(sf::Vector2i a, sf::Vector2i b);

    // Check if a grid position is walkable
    bool IsWalkable(sf::Vector2i gridPos, const std::vector<AstarUnit>& units, float pathfindingUnitSize);

    // Reconstruct the path from the goal node
    std::vector<sf::Vector2f> ReconstructPath(Node node);

    // Convert grid coordinates to world position
    sf::Vector2f GridToWorld(sf::Vector2i gridPos);
};

#endif