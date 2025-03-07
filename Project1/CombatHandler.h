#ifndef COMBATHANDLER_H
#define COMBATHANDLER_H

#include <SFML/Graphics.hpp>
#include "Units.h"

class CombatHandler
{
public:
	void update(const std::vector<std::shared_ptr<Units>>& t_units, const std::vector<std::shared_ptr<Units>>& t_enemyunits);
	bool isInRange(Units& t_main, Units& t_other);

private:
	std::vector<std::shared_ptr<Units>> playerUnits;
	std::vector<std::shared_ptr<Units>> enemyUnits;

};

#endif