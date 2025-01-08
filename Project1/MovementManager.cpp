#include "MovementManager.h"


sf::Vector2f MovementManager::applyFlowFieldDirection(sf::Vector2f t_position, FlowField* t_flowfield)
{
	sf::Vector2f pos = t_position;
	int gridX = static_cast<int>(pos.x / FlowField::GRID_WIDTH);
	int gridY = static_cast<int>(pos.y / FlowField::GRID_HEIGHT);

	Cell currentCell = t_flowfield->Grid[gridY][gridX];
	sf::Vector2f direction = currentCell.getDirection();

	// Swaps to direct movement when near destination cell
	if (isNearDestination(pos, t_flowfield, 75))
	{
		direction = t_flowfield->destinationPosition - pos;
		direction = normalize(direction);
		return direction;
	}
	return direction;
}

sf::Vector2f MovementManager::repulsion(sf::Vector2f t_position, sf::Vector2f t_position_2, float t_radius_1, float t_radius_2)
{
	const float MIN_UNIT_SPACING = (t_radius_1 + t_radius_2) / 2;
	const float MAX_REPULSION_DISTANCE = t_radius_1 + t_radius_2;
	const float REPULSION_STRENGTH = 15.0f;

	sf::Vector2f repulsion = t_position - t_position_2;
	float distance = std::sqrt(repulsion.x * repulsion.x + repulsion.y * repulsion.y);

	// Only apply repulsion if within the max distance
	if (distance < MAX_REPULSION_DISTANCE && distance > 0.01f) 
	{
		sf::Vector2f repulsionDir = repulsion / distance;
		float repulsionForce = REPULSION_STRENGTH * (1.0f / distance);

		// Clamp the minimum distance to prevent units from overlapping
		float overlap = MIN_UNIT_SPACING - distance;
		if (overlap > 0) 
		{
			repulsionForce += overlap; // Add extra force if overlapping
		}

		sf::Vector2f push = repulsionDir * repulsionForce;
		return push;
	}
	return sf::Vector2f(0,0);
}

bool MovementManager::isDestinationReached(sf::Vector2f t_position, FlowField* t_flowfield)
{
	sf::Vector2f length = t_position - t_flowfield->destinationPosition;
	float distance = std::sqrt(length.x * length.x + length.y * length.y);

	// Distance check to stop unit vibration when moving
	if (distance <= 10.f)
	{
		return true;
	}
	else {
		return false;
	}
}

bool MovementManager::isNearDestination(sf::Vector2f t_position, FlowField* t_flowfield, float t_radius)
{
	sf::Vector2f length = t_position - t_flowfield->destination->getPostion();
	float distance = std::sqrt(length.x * length.x + length.y * length.y);

	// Distance check to stop unit vibration when moving
	if (distance <= t_radius)
	{
		return true;
	}
	else {
		return false;
	}
}

sf::Vector2f MovementManager::normalize(const sf::Vector2f& vector)
{
	float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
	
	if (length != 0)
		return vector / length;

	return sf::Vector2f(0, 0);
}
