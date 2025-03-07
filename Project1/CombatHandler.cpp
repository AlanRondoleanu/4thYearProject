#include "CombatHandler.h"

void CombatHandler::update(const std::vector<std::shared_ptr<Units>>& t_units, const std::vector<std::shared_ptr<Units>>& t_enemyunits, float t_deltaTime)
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
				playerUnit->state != UnitState::Attacking &&
				isInRange(*playerUnit.get(), *enemyUnit.get()))
			{
				playerUnit->state = UnitState::Attacking;
				playerUnit->currentTarget = enemyUnit;
				std::cout << "setting to attack" << std::endl;
				break;
			}
		}

		if (playerUnit->state == UnitState::Attacking)
		{
			if (playerUnit->canAttack())
			{
				// Creates a projectile
				handleAttack(*playerUnit.get(), playerUnit->currentTarget);
			}
		}
	}

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

	// Debug
	std::cout << "attack" << std::endl;
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
