#ifndef COMBATHANDLER_H
#define COMBATHANDLER_H

#include <SFML/Graphics.hpp>
#include "Units.h"
#include "Projectile.h"
#include "Soldier_Bullet.h"

class CombatHandler
{
public:
	void update(const std::vector<std::shared_ptr<Units>>& t_units, const std::vector<std::shared_ptr<Units>>& t_enemyunits, float t_deltaTime);
	void renderProjectiles(sf::RenderWindow& t_window);
	void handleAttack(Units& attacker, std::shared_ptr<Units> target);

	bool isInRange(Units& t_main, Units& t_other);

private:
	std::vector<std::shared_ptr<Units>> playerUnits;
	std::vector<std::shared_ptr<Units>> enemyUnits;
	std::vector<std::shared_ptr<Projectile>> projectiles;

};

#endif