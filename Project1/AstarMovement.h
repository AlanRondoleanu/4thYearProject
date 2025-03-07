#ifndef ASTARMOVEMENT_H
#define ASTARMOVEMENT_H

#include "Astar.h"
#include <vector>

class AStarMovement
{
public:
    AStarMovement(Astar& astar) : astar(astar) {}

    sf::Vector2f MoveTo(sf::Vector2f currentPosition, float pathfindingUnitSize);
    Astar* getAstar() { return &astar; }

private:
    Astar astar;
};

#endif