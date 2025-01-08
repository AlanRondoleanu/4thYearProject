#ifndef UNITHANDLER_H
#define UNITHANDLER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Units.h"
#include "Soldier.h"
#include "Mouse.h"
#include "MovementManager.h"
#include <unordered_map>
#include <unordered_set>

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
	void setFlowField(FlowField* t_flowfield) { mainFlowField = t_flowfield; }
	
	void update();
	void render(sf::RenderWindow& t_window);
	void spawnUnit();
	std::vector<sf::Vector2f> unitMoveOrder();
	Cell* selectCell();

	std::vector<Units*> getUnitsInCellAndNeighbors(int cellID);
	void applyAlignmentAndCohesion(Units* t_self);

	MovementManager movementManager;
	FlowField* mainFlowField;
	std::unordered_map<int, std::vector<Units*>> partitionedMap;

	std::vector<std::unique_ptr<Units>> playerUnits;
	std::vector<std::unique_ptr<Units>> enemyUnits;
	std::unordered_set<Units*> selectedUnits;

private:
	
	int const MAX_UNITS{ 100 };

	UnitHandler() = default;
};

#endif
