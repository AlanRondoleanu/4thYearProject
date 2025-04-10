#ifndef UNITHANDLER_H
#define UNITHANDLER_H

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "Units.h"
#include "Soldier.h"
#include "Orc.h"
#include "NightElf.h"
#include "Mouse.h"
#include "MovementManager.h"
#include "UnitGroup.h"
#include "RayCasting.h"
#include "FlowfieldMovement.h"
#include "PartitionGrid.h"

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
	void spawnUnit(std::string t_type, sf::Vector2f t_position, bool t_friendly);
	void spawnUnitFromBuilding(std::string t_type, sf::Vector2f t_position, sf::Vector2f t_target, bool t_friendly);
	void setMovementFields(FlowField* t_flowfield, Astar* t_astar) { mainFlowField = t_flowfield, mainAstar = t_astar; }
	std::shared_ptr<Targetable> getFirstUnitInList();

	void singleMoveOrder(Units* t_unit, sf::Vector2f t_position);
	std::vector<sf::Vector2f> formationMoveOrder(UnitState t_state);
	sf::Vector2f attackFollowMoveOrder(Units* t_target);


	// Movement
	MovementManager movementManager;
	FlowField* mainFlowField;
	Astar* mainAstar;

	// Partitioning
	PartitionGrid partitioning{ FlowField::GRID_WIDTH, FlowField::GRID_HEIGHT };

	// Raycasting
	RayCasting raycasting;

	std::vector<std::shared_ptr<Targetable>> playerUnits;
	std::vector<std::shared_ptr<Targetable>> enemyUnits;
	std::unordered_set<Units*> selectedUnits;
	std::vector<UnitGroup> groups;

private:
	
	void updateUnits(std::vector<std::shared_ptr<Targetable>>& t_units, float t_deltaTime);
	void eraseDeadUnits(std::vector<std::shared_ptr<Targetable>>& t_units);
	Cell* selectCell();
	Cell* selectCell(sf::Vector2f t_position);
	void createNewGroupFromSelectedUnits(const std::unordered_set<Units*>& selectedUnits);
	void createNewGroupFromSelectedUnits(const std::unordered_set<Units*>& selectedUnits, Targetable* t_target);
	void aStarHandout(sf::Vector2f postion, Units* unit);
	std::shared_ptr<Targetable> findSharedPtrFromRaw(Targetable* rawPtr);
	std::vector<AstarUnit> AStarUnitHandOut(Units* t_unit);

	int const MAX_UNITS{ 100 };

	UnitHandler() = default;
};

#endif
