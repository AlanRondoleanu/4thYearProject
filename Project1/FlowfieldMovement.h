#ifndef FLOWFIELDMOVEMENT_H
#define FLOWFIELDMOVEMENT_H

#include "MovementStrategy.h"
#include <SFML/Graphics.hpp>
#include "FlowField.h"

class FlowfieldMovement : public MovementStrategy
{
public:
    // Constructor: Takes a reference to a FlowField
    FlowfieldMovement(FlowField& flowField) : flowField(flowField) {}

    // MoveTo implementation for FlowField movement
    sf::Vector2f MoveTo(sf::Vector2f currentPosition) override;

    // Optional: Override ComputePath if needed
    void ComputePath(sf::Vector2f t_position) override;

private:
    FlowField& flowField; // Reference to the flow field
};

#endif