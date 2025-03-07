#include "CombatHandler.h"

void CombatHandler::update(const std::vector<std::shared_ptr<Units>>& t_units, const std::vector<std::shared_ptr<Units>>& t_enemyunits)
{
	playerUnits = t_units;
	enemyUnits = t_enemyunits;

	// Combat Handling
	for (auto& playerUnit : playerUnits)
	{
		// Check if in range to attack
		for (auto& enemyUnit : enemyUnits)
		{
			if (playerUnit->state != UnitState::Moving &&
				isInRange(*playerUnit.get(), *enemyUnit.get()))
			{
				playerUnit->state = UnitState::Attacking;
				break;
			}
		}

		if (playerUnit->state != UnitState::Attacking)
		{

		}
	}
}

bool CombatHandler::isInRange(Units& t_main, Units& t_other)
{
	sf::Vector2f mainPosition = t_main.getPos();
	sf::Vector2f otherPosition = t_other.getPos();

	// Calculate squared distance
	float dx = otherPosition.x - mainPosition.x;
	float dy = otherPosition.y - mainPosition.y;
	float distSq = dx * dx + dy * dy;

	float attackRange = t_main.stats.range;

	return distSq <= attackRange * attackRange;
}
