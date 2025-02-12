#include "FlowfieldMovement.h"

// MoveTo implementation for FlowField movement
sf::Vector2f FlowfieldMovement::MoveTo(sf::Vector2f currentPosition)
{
    // Get the direction from the flow field at the current position
    sf::Vector2f direction = flowField.getDirection(currentPosition);

    return direction;
}

void FlowfieldMovement::ComputePath(sf::Vector2f t_position)
{
    flowField.computePath(t_position);
}