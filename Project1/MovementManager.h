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

	sf::Vector2f useDirectMovement(sf::Vector2f t_position, sf::Vector2f t_velocity, sf::Vector2f t_destination, sf::Vector2f t_cell_location);
	sf::Vector2f repulsion(sf::Vector2f t_position, sf::Vector2f t_position_2, float t_radius_1, float t_radius_2);
	sf::Vector2f applyAlignmentAndCohesion(sf::Vector2f t_self_position, sf::Vector2f t_self_velocity, std::vector<sf::Vector2f> t_velocities, std::vector<sf::Vector2f> t_positions);
	bool isDestinationReached(sf::Vector2f t_position, FlowField* t_flowfield);
	bool isNearDestination(sf::Vector2f t_position, sf::Vector2f t_destination, float t_radius);
	
	bool destinationReached{ false };
	UnitStats* stats;
	MoveType moveType{FlowFieldMovement};

private:
	float length(const sf::Vector2f& t_vector);
	sf::Vector2f normalize(const sf::Vector2f& vector);
};

#endif
