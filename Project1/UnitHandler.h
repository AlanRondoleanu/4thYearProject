#ifndef UNITHANDLER_H
#define UNITHANDLER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "Units.h"
#include "Soldier.h"
#include "Mouse.h"
#include "MovementManager.h"
#include "UnitGroup.h"

class UnitHandler
{
public:
	
	UnitHandler(const UnitHandler&) = delete;
	UnitHandler& operator=(const UnitHandler&) = delete;
	static UnitHandler& getInstance()
	{
		static UnitHandler instance;
		return instance;
	}	

	void update();
	void render(sf::RenderWindow& t_window);
	void spawnUnit();
	Cell* selectCell();
	std::vector<sf::Vector2f> formationMoveOrder();
	sf::Vector2f attackMoveOrder();
	std::vector<Units*> getUnitsInCellAndNeighbors(int cellID);
	void createNewGroupFromSelectedUnits(const std::unordered_set<Units*>& selectedUnits);
	void createNewGroupFromSelectedUnits(const std::unordered_set<Units*>& selectedUnits, Units* t_target);


	void setFlowField(FlowField* t_flowfield) { mainFlowField = t_flowfield; }

	MovementManager movementManager;
	FlowField* mainFlowField;
	std::unordered_map<int, std::vector<Units*>> partitionedMap;
	std::vector<std::shared_ptr<Units>> playerUnits;
	std::vector<std::shared_ptr<Units>> enemyUnits;
	std::unordered_set<Units*> selectedUnits;
	std::vector<UnitGroup> groups;
private:
	
	int const MAX_UNITS{ 100 };

	UnitHandler() = default;
};

#endif
