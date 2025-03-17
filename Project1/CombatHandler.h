#ifndef COMBATHANDLER_H
#define COMBATHANDLER_H

#include <SFML/Graphics.hpp>
#include "Units.h"
#include "Projectile.h"
#include "Soldier_Bullet.h"
#include "Turret_Bullet.h"
#include "Buildings.h"
#include "Turret.h"

class CombatHandler
{
public:
	void update(std::vector<std::shared_ptr<Units>>& t_units, std::vector<std::shared_ptr<Units>>& t_enemyunits, std::vector<std::shared_ptr<Buildings>>& t_playerBuildings, std::vector<std::shared_ptr<Buildings>>& t_enemyBuildings, float t_deltaTime);
	void renderProjectiles(sf::RenderWindow& t_window);
	void handleAttack(Units& attacker, std::shared_ptr<Units> target);
	void handleBuildingAttack(Buildings& attacker, std::shared_ptr<Units> target);
	void handleUnitCombat(std::vector<std::shared_ptr<Units>>& t_units, std::vector<std::shared_ptr<Units>>& t_enemies);
	void handleBuildingCombat(std::vector<std::shared_ptr<Units>>& t_enemies, std::vector<std::shared_ptr<Buildings>>& t_buildings);
	bool isInRange(sf::Vector2f t_position, sf::Vector2f t_other, float t_range);

private:
	std::vector<std::shared_ptr<Projectile>> projectiles;

};

#endif