#ifndef FLOWFIELDMOVEMENT_H
#define FLOWFIELDMOVEMENT_H

#include <SFML/Graphics.hpp>
#include "FlowField.h"

class FlowfieldMovement
{
public:
    FlowfieldMovement(FlowField flowField) : flowField(flowField) {}

    sf::Vector2f MoveTo(sf::Vector2f currentPosition);

    void ComputePath(sf::Vector2f t_position);
    void SetFlowfield(FlowField t_flowfield);
    FlowField* getFlowfield() { return &flowField; }

private:
    FlowField flowField;
};

#endif