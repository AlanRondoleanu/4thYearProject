#include "CombatHandler.h"

void CombatHandler::update(std::vector<std::shared_ptr<Targetable>>& t_units, std::vector<std::shared_ptr<Targetable>>& t_enemyunits, std::vector<std::shared_ptr<Targetable>>& t_playerBuildings, std::vector<std::shared_ptr<Targetable>>& t_enemyBuildings, float t_deltaTime)
{
	// Player Unit Handling
	handleUnitCombat(t_units, t_enemyunits, t_enemyBuildings);
	// Enemy Unit Handling
	handleUnitCombat(t_enemyunits, t_units, t_playerBuildings);
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

void CombatHandler::handleAttack(Units& attacker, std::shared_ptr<Targetable> target)
{
	sf::Vector2f direction = target->getPos() - attacker.getPos();
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length != 0) direction /= length; // Normalize

	std::shared_ptr<Projectile> newProjectile;

	// Decide which projectile to create based on unit type
	if (attacker.GetUnitType() == "Soldier")
		newProjectile = std::make_shared<Soldier_Bullet>(attacker.getPos(), direction, target, attacker.stats.damage, attacker.stats.projectile_speed);
	else if (attacker.GetUnitType() == "Orc")
		newProjectile = std::make_shared<Orc_Bullet>(attacker.getPos(), direction, target, attacker.stats.damage, attacker.stats.projectile_speed);
	else if (attacker.GetUnitType() == "NightElf")
		newProjectile = std::make_shared<NightElf_Bullet>(attacker.getPos(), direction, target, attacker.stats.damage, attacker.stats.projectile_speed);


	if (newProjectile)
		projectiles.push_back(newProjectile);

	// Resets the timer for attack cooldown
	attacker.resetAttackTimer();
}

void CombatHandler::handleBuildingAttack(Buildings& attacker, std::shared_ptr<Targetable> target)
{
	sf::Vector2f direction = target->getPos() - attacker.getPos();
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length != 0) direction /= length; // Normalize

	std::shared_ptr<Projectile> newProjectile;

	// Decide which projectile to create based on unit type
	if (attacker.GetUnitType() == "Turret")
		newProjectile = std::make_shared<Turret_Bullet>(attacker.getPos(), direction, target);
	else if (attacker.GetUnitType() == "Turret_Fantasy")
		newProjectile = std::make_shared<Turret_Bullet>(attacker.getPos(), direction, target);
	else if (attacker.GetUnitType() == "Nexus")
		newProjectile = std::make_shared<Turret_Bullet>(attacker.getPos(), direction, target);
	else if (attacker.GetUnitType() == "Nexus_Fantasy")
		newProjectile = std::make_shared<Turret_Bullet>(attacker.getPos(), direction, target);


	if (newProjectile)
		projectiles.push_back(newProjectile);

	// Resets the timer for attack cooldown
	attacker.resetAttackTimer();
}

void CombatHandler::handleUnitCombat(std::vector<std::shared_ptr<Targetable>>& t_units, std::vector<std::shared_ptr<Targetable>>& t_enemies, std::vector<std::shared_ptr<Targetable>>& t_buildings)
{
	// Combat Handling Between Units
	for (auto& playerUnit : t_units)
	{	
		std::shared_ptr<Units> currentUnit = std::dynamic_pointer_cast<Units>(playerUnit);

		if (currentUnit->getState() != UnitState::Moving &&
			currentUnit->getState() != UnitState::Attacking)
		{
			// Check Enemies Units in Range
			for (auto& enemyUnit : t_enemies)
			{
				// Check if in range
				if (isInRange(currentUnit->getPos(), enemyUnit->getPos(), currentUnit->getUnitStats().range))
				{
					currentUnit->setState(UnitState::Attacking);
					currentUnit->setTarget(enemyUnit);
					break;
				}
			}

			// Check Enemy Buildings in Range
			for (auto& buildings : t_buildings)
			{
				// Check if in range
				if (isInRange(currentUnit->getPos(), buildings->getPos(), playerUnit->getUnitStats().range))
				{
					currentUnit->setState(UnitState::Attacking);
					currentUnit->setTarget(buildings);
					break;
				}
			}
		}

		if (currentUnit->getState() == UnitState::Attacking)
		{
			if (currentUnit->getTarget()->getAlive() == false)
			{
				if (currentUnit->destinationReached == true)
					currentUnit->setState(UnitState::Idle);
				else
					currentUnit->setState(UnitState::AttackMove);

				currentUnit->setTarget(nullptr);
			}
			else if (currentUnit->canAttack())
			{
				// Creates a projectile
				handleAttack(*currentUnit, currentUnit->currentTarget);
			}
		}
	}
}

void CombatHandler::handleBuildingCombat(std::vector<std::shared_ptr<Targetable>>& t_enemies, std::vector<std::shared_ptr<Targetable>>& t_buildings)
{
	for (auto& building : t_buildings)
	{
		std::shared_ptr<Buildings> currentBuilding = std::dynamic_pointer_cast<Buildings>(building);

		if (currentBuilding->isAggressive())
		{
			for (auto& enemyUnit : t_enemies)
			{
				if (isInRange(currentBuilding->getPos(), enemyUnit->getPos(), currentBuilding->stats.range))
				{
					currentBuilding->currentTarget = enemyUnit;
					break;
				}
			}

			if (currentBuilding->currentTarget != nullptr)
			{
				if (currentBuilding->currentTarget->getAlive() == false)
				{
					currentBuilding->currentTarget = nullptr;
				}
				else if (currentBuilding->canAttack())
				{
					// Creates a projectile
					handleBuildingAttack(*currentBuilding, currentBuilding->currentTarget);
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
