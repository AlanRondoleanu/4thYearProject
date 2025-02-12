#ifndef MOVEMENTSTRATEGY_H
#define MOVEMENTSTRATEGY_H

#include <SFML/Graphics.hpp>
#include <vector>

class MovementStrategy
{
public:
    virtual ~MovementStrategy() = default;

    // Pure virtual function for moving a unit towards a target position
    virtual sf::Vector2f MoveTo(sf::Vector2f currentPosition) = 0;

    // Optional: Function to compute a path (if needed)
    virtual void ComputePath(sf::Vector2f t_position);
};

#endif
