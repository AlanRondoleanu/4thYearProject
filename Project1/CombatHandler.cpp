#include "CombatHandler.h"

void CombatHandler::update(std::vector<std::shared_ptr<Units>>& t_units, std::vector<std::shared_ptr<Units>>& t_enemyunits, std::vector<std::shared_ptr<Buildings>>& t_playerBuildings, std::vector<std::shared_ptr<Buildings>>& t_enemyBuildings, float t_deltaTime)
{
	// Player Unit Handling
	handleUnitCombat(t_units, t_enemyunits);
	// Enemy Unit Handling
	handleUnitCombat(t_enemyunits, t_units);
	// Player Building Handling
	handleBuildingCombat(t_enemyunits, t_playerBuildings);
	// Enemy Building Handling
	handleBuildingCombat(t_units, t_enemyBuildings);

	// Update Projectiles
	for (auto projectile : projectiles)
	{
		projectile->Update(t_deltaTime);
	}

	// Remove inactive projectiles
	projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
		[](const std::shared_ptr<Projectile>& p) { return !p->IsActive(); }),
		projectiles.end());
}

void CombatHandler::renderProjectiles(sf::RenderWindow& t_window)
{
	for (auto projectile : projectiles)
	{
		projectile->Render(t_window);
	}
}

void CombatHandler::handleAttack(Units& attacker, std::shared_ptr<Units> target)
{
	sf::Vector2f direction = target->getPos() - attacker.getPos();
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length != 0) direction /= length; // Normalize

	std::shared_ptr<Projectile> newProjectile;

	// Decide which projectile to create based on unit type
	if (attacker.GetUnitType() == "Soldier")
		newProjectile = std::make_shared<Soldier_Bullet>(attacker.getPos(), direction, target);

	if (newProjectile)
		projectiles.push_back(newProjectile);

	// Resets the timer for attack cooldown
	attacker.resetAttackTimer();
}

void CombatHandler::handleBuildingAttack(Buildings& attacker, std::shared_ptr<Units> target)
{
	sf::Vector2f direction = target->getPos() - attacker.getPos();
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length != 0) direction /= length; // Normalize

	std::shared_ptr<Projectile> newProjectile;

	// Decide which projectile to create based on unit type
	if (attacker.GetUnitType() == "Turret")
		newProjectile = std::make_shared<Turret_Bullet>(attacker.getPos(), direction, target);

	if (newProjectile)
		projectiles.push_back(newProjectile);

	// Resets the timer for attack cooldown
	attacker.resetAttackTimer();
}

void CombatHandler::handleUnitCombat(std::vector<std::shared_ptr<Units>>& t_units, std::vector<std::shared_ptr<Units>>& t_enemies)
{
	// Combat Handling
	for (auto& playerUnit : t_units)
	{
		// Check if in range to attack
		if (playerUnit->state != UnitState::Moving &&
			playerUnit->state != UnitState::Attacking)
		{
			for (auto& enemyUnit : t_enemies)
			{
				if (isInRange(playerUnit->getPos(), enemyUnit->getPos(), playerUnit->stats.range))
				{
					playerUnit->state = UnitState::Attacking;
					playerUnit->currentTarget = enemyUnit;
					break;
				}
			}
		}

		if (playerUnit->state == UnitState::Attacking)
		{
			if (playerUnit->currentTarget->alive == false)
			{
				if (playerUnit->destinationReached)
					playerUnit->state = UnitState::Idle;
				else
					playerUnit->state = UnitState::AttackMove;

				playerUnit->currentTarget = nullptr;
			}
			else if (playerUnit->canAttack())
			{
				// Creates a projectile
				handleAttack(*playerUnit.get(), playerUnit->currentTarget);
			}
		}
	}
}

void CombatHandler::handleBuildingCombat(std::vector<std::shared_ptr<Units>>& t_enemies, std::vector<std::shared_ptr<Buildings>>& t_buildings)
{
	for (auto& building : t_buildings)
	{
		if (building->isAggressive())
		{
			for (auto& enemyUnit : t_enemies)
			{
				if (isInRange(building->getPos(), enemyUnit->getPos(), building->stats.range))
				{
					building->currentTarget = enemyUnit;
					break;
				}
			}

			if (building->currentTarget != nullptr)
			{
				if (building->currentTarget->alive == false)
				{
					building->currentTarget = nullptr;
				}
				else if (building->canAttack())
				{
					// Creates a projectile
					handleBuildingAttack(*building, building->currentTarget);
				}
			}
		}	
	}
}

bool CombatHandler::isInRange(sf::Vector2f t_position, sf::Vector2f t_other, float t_range)
{
	sf::Vector2f mainPosition = t_position;
	sf::Vector2f otherPosition = t_other;

	// Calculate squared distance
	float dx = otherPosition.x - mainPosition.x;
	float dy = otherPosition.y - mainPosition.y;
	float distSq = dx * dx + dy * dy;

	float attackRange = t_range;

	return distSq <= attackRange * attackRange;
}
