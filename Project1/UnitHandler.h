#ifndef UNITHANDLER_H
#define UNITHANDLER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Units.h"
#include "Soldier.h"
#include "Mouse.h"
#include "MovementManager.h"


class UnitHandler
{
public:
	
	// Delete copy constructor and assignment operator to enforce singleton
	UnitHandler(const UnitHandler&) = delete;
	UnitHandler& operator=(const UnitHandler&) = delete;
	
	// Get the singleton instance
	static UnitHandler& getInstance()
	{
		static UnitHandler instance;
		return instance;
	}
	
	void update();
	void render(sf::RenderWindow& t_window);
	void spawnUnit();

	MovementManager movementManager;

	std::vector<std::unique_ptr<Units>> playerUnits;
	std::vector<std::unique_ptr<Units>> enemyUnits;
	std::vector<Units*> selectedUnits;
private:
	
	int const MAX_UNITS{ 100 };

	UnitHandler() = default;
};

#endif
