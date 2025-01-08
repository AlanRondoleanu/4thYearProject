#ifndef MOVEMENTMANAGER_H
#define MOVEMENTMANAGER_H

#include <SFML/Graphics.hpp>
#include "FlowField.h"
#include "UnitStats.h"
#include <cmath>
#include <iostream>

class MovementManager
{
public:
	enum MoveType {
		FlowFieldMovement,
		CellMovement
	};

	sf::Vector2f applyFlowFieldDirection(sf::Vector2f t_position, FlowField* t_flowfield);
	sf::Vector2f repulsion(sf::Vector2f t_position, sf::Vector2f t_position_2, float t_radius_1, float t_radius_2);
	bool isDestinationReached(sf::Vector2f t_position, FlowField* t_flowfield);
	bool isNearDestination(sf::Vector2f t_position, FlowField* t_flowfield, float t_radius);


	UnitStats* stats;
	MoveType moveType{FlowFieldMovement};

private:
	sf::Vector2f normalize(const sf::Vector2f& vector);
};

#endif
