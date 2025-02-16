#include "AstarMovement.h"

sf::Vector2f AStarMovement::MoveTo(sf::Vector2f currentPosition, float pathfindingUnitSize)
{
    // Compute the path using A*
    std::vector<sf::Vector2f> path = astar.FindPath(currentPosition, pathfindingUnitSize);

    if (!path.empty()) 
    {
        // Move towards the next point in the path
        sf::Vector2f direction = path[0] - currentPosition;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        
        if (length > 0) 
        {
            direction /= length;
        }

        return direction;
    }

    return sf::Vector2f(0, 0);
}
