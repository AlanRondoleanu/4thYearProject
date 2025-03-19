#include "MovementManager.h"


sf::Vector2f MovementManager::useDirectMovement(sf::Vector2f t_position, sf::Vector2f t_velocity, sf::Vector2f t_destination, sf::Vector2f t_cell_location)
{
	sf::Vector2f currentPosition = t_position;
	sf::Vector2f direction = t_velocity;

	// Swaps to direct movement when near destination cell
	if (isNearDestination(currentPosition, t_cell_location, 40))
	{
		sf::Vector2f direction = t_destination - currentPosition;
		direction = normalize(direction);
		return direction;
	}
	else {
		return direction;
	}
}

void MovementManager::repulsion(Units& t_main, PartitionGrid& t_partition)
{
	const float MAIN_UNIT_RADIUS = t_main.getRadius();

	for (auto& unit : t_partition.getUnitsInCellAndNeighbors(t_main.getCellID()))
	{
		Units* otherUnit = dynamic_cast<Units*>(unit);

		if (&t_main == otherUnit)
			continue;

		const float OTHER_UNIT_RADIUS = otherUnit->getRadius();
		const float MIN_UNIT_SPACING = (MAIN_UNIT_RADIUS + OTHER_UNIT_RADIUS) / 2;
		const float MAX_REPULSION_DISTANCE = MAIN_UNIT_RADIUS + OTHER_UNIT_RADIUS;
		const float REPULSION_STRENGTH = 15.0f;

		sf::Vector2f repulsion = t_main.getPos() - otherUnit->getPos();
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

			otherUnit->push(-push);
			t_main.push(push);
		}
	}
}

sf::Vector2f MovementManager::applyAlignmentAndCohesion(sf::Vector2f t_self_position, sf::Vector2f t_self_velocity, std::vector<sf::Vector2f> t_velocities, std::vector<sf::Vector2f> t_positions)
{
	const float ALIGNMENT_WEIGHT = 0.5f;   // Influence of alignment
	const float FLOWFIELD_WEIGHT = 0.5f;   // Influence of the flow field
	const float COHESION_WEIGHT = 0.2f;   // Influence of cohesion

	sf::Vector2f alignmentVector = { 0, 0 };
	sf::Vector2f cohesionVector = { 0.f, 0.f };

	int selectedCount = 0;
	for (size_t i = 0; i < t_velocities.size(); i++)
	{
		alignmentVector += t_velocities[i];
		cohesionVector += t_positions[i];
		selectedCount++;
	}

	// Average the vectors
	alignmentVector /= static_cast<float>(selectedCount);
	cohesionVector /= static_cast<float>(selectedCount);
	cohesionVector -= t_self_position;

	// Combine alignment, cohesion and flow field vectors
	sf::Vector2f combinedVector =
		(ALIGNMENT_WEIGHT * normalize(alignmentVector)) +
		(COHESION_WEIGHT * normalize(cohesionVector)) +
		(FLOWFIELD_WEIGHT * t_self_velocity);

	combinedVector = normalize(combinedVector);

	return combinedVector;
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

bool MovementManager::isNearDestination(sf::Vector2f t_position, sf::Vector2f t_destination, float t_radius)
{
	sf::Vector2f length = t_position - t_destination;
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

float MovementManager::length(const sf::Vector2f& t_vector)
{
	return std::sqrt(t_vector.x * t_vector.x + t_vector.y * t_vector.y);
}

sf::Vector2f MovementManager::normalize(const sf::Vector2f& vector)
{
	float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
	
	if (length != 0)
		return vector / length;

	return sf::Vector2f(0, 0);
}
