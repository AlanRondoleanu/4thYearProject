#include "Astar.h"
#include <iostream>


Astar::Astar()
{
}

std::vector<sf::Vector2f> Astar::FindPath(sf::Vector2f start, float pathfindingUnitSize)
{
    // Convert world positions to grid coordinates
    sf::Vector2i startGrid = WorldToGrid(start);
    sf::Vector2i targetGrid = WorldToGrid(target);

    // Priority queue for open nodes
    std::priority_queue<Node> openSet;

    // Map to store node information
    std::unordered_map<int, Node> allNodes;

    // Add the start node to the open set
    Node startNode = { startGrid, 0, Heuristic(startGrid, targetGrid), nullptr };
    openSet.push(startNode);
    allNodes[GridToIndex(startGrid)] = startNode;

    // If target cell is blocked, find the nearest valid cell
    if (!IsWalkable(targetGrid, unitPositions, pathfindingUnitSize))
    {
        targetGrid = FindNearestWalkableCell(targetGrid, 5, pathfindingUnitSize); // 5 is a reasonable radius

        if (targetGrid == sf::Vector2i(-1, -1))
        {
            // Still no valid cell found after searching
            return {};
        }
    }

    while (!openSet.empty()) 
    {
        Node currentNode = openSet.top();
        openSet.pop();

        // Check if we've reached the target
        if (currentNode.position == targetGrid) {
            return ReconstructPath(currentNode);
        }

        // Explore neighbors (including diagonals)
        for (const auto& direction : directions) 
        {
            sf::Vector2i neighborPos = currentNode.position + direction;

            // Skip if out of bounds or blocked by an obstacle
            if (!IsWalkable(neighborPos, unitPositions, pathfindingUnitSize)) continue;

            // Calculate tentative gCost
            float movementCost = (direction.x != 0 && direction.y != 0) ? 1.414f : 1.0f; // Diagonal cost = sqrt(2)
            float tentativeGCost = currentNode.gCost + movementCost;

            // Check if this node is already in allNodes
            int neighborIndex = GridToIndex(neighborPos);
            if (allNodes.find(neighborIndex) == allNodes.end() || tentativeGCost < allNodes[neighborIndex].gCost) 
            {
                Node neighborNode = {
                    neighborPos,
                    tentativeGCost,
                    Heuristic(neighborPos, targetGrid),
                    &allNodes[GridToIndex(currentNode.position)]
                };

                openSet.push(neighborNode);
                allNodes[neighborIndex] = neighborNode;
            }
        }
    }

    return {}; // No path found
}

void Astar::setTarget(sf::Vector2f t_position)
{
    target = t_position;
}

sf::Vector2i Astar::WorldToGrid(sf::Vector2f worldPos)
{
    return sf::Vector2i(static_cast<int>(worldPos.x / cellSize), static_cast<int>(worldPos.y / cellSize));
}

int Astar::GridToIndex(sf::Vector2i gridPos)
{
    return gridPos.y * gridSize.x + gridPos.x;
}

float Astar::Heuristic(sf::Vector2i a, sf::Vector2i b)
{
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

bool Astar::IsWalkable(sf::Vector2i gridPos, const std::vector<AstarUnit>& units, float pathfindingUnitSize)
{
    // Check bounds
    if (gridPos.x < 0 || gridPos.y < 0 || gridPos.x >= gridSize.x || gridPos.y >= gridSize.y) 
    {
        std::cout << "out" << std::endl;
        return false;
    }

    float pathfindingUnitGridSize = pathfindingUnitSize / cellSize;
    float pathfindingUnitRadius = pathfindingUnitGridSize / 2;
    float pathfindingUnitRadiusSq = pathfindingUnitRadius * pathfindingUnitRadius; // Precompute squared radius

    // Check if the unit's area overlaps with any other unit
    for (const auto& unit : units)
    {
        sf::Vector2i unitGridPos = WorldToGrid(unit.position);
        float unitGridSize = unit.size / cellSize;
        float unitRadius = unitGridSize / 2;
        float radiusSumSq = (unitRadius + pathfindingUnitRadius) * (unitRadius + pathfindingUnitRadius); // Squared sum of radii

        // Calculate squared distance (avoids sqrt)
        float dx = gridPos.x - unitGridPos.x;
        float dy = gridPos.y - unitGridPos.y;
        float distSq = dx * dx + dy * dy;

        // If squared distance is less than squared radius sum, the cell is blocked
        if (distSq < radiusSumSq)
            return false;
    }

    return true;
}

sf::Vector2i Astar::FindNearestWalkableCell(sf::Vector2i blockedCell, int maxSearchRadius, float pathfindingUnitSize)
{
    for (int radius = 1; radius <= maxSearchRadius; ++radius)
    {
        for (int dx = -radius; dx <= radius; ++dx)
        {
            for (int dy = -radius; dy <= radius; ++dy)
            {
                // Skip center cell
                if (dx == 0 && dy == 0) continue;

                sf::Vector2i neighbor = blockedCell + sf::Vector2i(dx, dy);

                if (IsWalkable(neighbor, unitPositions, pathfindingUnitSize))
                {
                    return neighbor;
                }
            }
        }
    }

    // No valid cell found
    return sf::Vector2i(-1, -1); // Sentinel: invalid cell
}

std::vector<sf::Vector2f> Astar::ReconstructPath(Node node)
{
    std::vector<sf::Vector2f> path;

    while (node.parent) 
    {
        path.push_back(GridToWorld(node.position));
        node = *node.parent;
    }
    std::reverse(path.begin(), path.end());

    return path;
}

sf::Vector2f Astar::GridToWorld(sf::Vector2i gridPos)
{
    return sf::Vector2f(gridPos.x * cellSize, gridPos.y * cellSize);
}
