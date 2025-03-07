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
#include "RayCasting.h"
#include "FlowfieldMovement.h"
#include "CombatHandler.h"

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

	void update(float t_deltaTime);
	void render(sf::RenderWindow& t_window);
	void spawnUnit(bool t_friendly);
	void setMovementFields(FlowField* t_flowfield, Astar* t_astar) { mainFlowField = t_flowfield, mainAstar = t_astar; }

	std::vector<sf::Vector2f> formationMoveOrder(UnitState t_state);
	sf::Vector2f attackFollowMoveOrder();


	// Movement
	MovementManager movementManager;
	FlowField* mainFlowField;
	Astar* mainAstar;

	// Combat
	CombatHandler combat;

	// Raycasting
	RayCasting raycasting;

	std::unordered_map<int, std::vector<Units*>> partitionedMap;
	std::vector<std::shared_ptr<Units>> playerUnits;
	std::vector<std::shared_ptr<Units>> enemyUnits;
	std::unordered_set<Units*> selectedUnits;
	std::vector<UnitGroup> groups;

private:

	Cell* selectCell();
	Cell* selectCell(sf::Vector2f t_position);
	std::vector<Units*> getUnitsInCellAndNeighbors(int cellID);
	void createNewGroupFromSelectedUnits(const std::unordered_set<Units*>& selectedUnits);
	void createNewGroupFromSelectedUnits(const std::unordered_set<Units*>& selectedUnits, Units* t_target);


	int const MAX_UNITS{ 100 };

	UnitHandler() = default;
};

#endif
