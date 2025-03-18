#include "UnitHandler.h"

void UnitHandler::update(float t_deltaTime)
{
	// Erase Dead Units
	eraseDeadUnits(playerUnits);
	eraseDeadUnits(enemyUnits);

	// Spacial Partition
	partitionedMap.clear();
	for (auto& playerUnit : playerUnits) // Player Units
	{
		partitionedMap[playerUnit->getCellID()].push_back(playerUnit.get());
	}
	for (auto& enemyUnit : enemyUnits) // Player Units
	{
		partitionedMap[enemyUnit->getCellID()].push_back(enemyUnit.get());
	}

	// Update for units
	updateUnits(playerUnits, t_deltaTime);
	updateUnits(enemyUnits, t_deltaTime);

	// Groups update
	for (auto& group : groups)
	{
		// Checks if the target of the group has moved from it's current cell to create another flowfield to follow
		if (group.getTarget().lock() &&
			group.getTargetLastCellID() != group.getTarget().lock()->getCellID())
		{
			group.refreshLastCellID();

			mainFlowField->computePath(group.getTarget().lock()->getPos());

			for (auto& unit : group.getUnits())
			{
				Units* units = dynamic_cast<Units*>(unit);
				mainFlowField->setDestinationPosition(group.getTarget().lock()->getPos());
				units->setFlowField(*mainFlowField, UnitState::AttackFollow);
			}
		}
		else {
			group.getTarget().reset();
		}
	}
}

void UnitHandler::render(sf::RenderWindow& t_window)
{
	// Render for players
	for (auto& playerUnit : playerUnits)
	{
		if (auto unit = std::dynamic_pointer_cast<Units>(playerUnit))
		{
			unit->draw(t_window); // Unit-specific draw
		}
	}
	// Render for enemies
	for (auto& enemyUnit : enemyUnits)
	{
		if (auto unit = std::dynamic_pointer_cast<Units>(enemyUnit))
		{
			unit->draw(t_window); // Unit-specific draw
		}
	}

	// Render for hovered unit
	Units* hovered = Mouse::getInstance().getHoveredUnit();
	if (hovered != nullptr && hovered->alive == true)
	{
		hovered->healthBar.render(t_window);
	}
}

void UnitHandler::updateUnits(std::vector<std::shared_ptr<Targetable>>& t_units, float t_deltaTime)
{
	// Update for player units
	for (auto& unit : t_units)
	{
		std::shared_ptr<Units> playerUnit = std::dynamic_pointer_cast<Units>(unit);
		playerUnit->setCellID();

		if (playerUnit->state == UnitState::Attacking)
		{
			playerUnit->velocity = { 0,0 };
		}

		// Preparing Raycasting
		Unit raycastingUnit = { playerUnit->getPos(), playerUnit->getRadius(), playerUnit->flowfieldDirection };
		std::vector<Unit> unitsToAvoid;

		// Partitioned Units Update
		for (auto& partitionUnit : getUnitsInCellAndNeighbors(playerUnit->cellID))
		{
			Units* otherUnit = dynamic_cast<Units*>(partitionUnit);

			if (playerUnit.get() != otherUnit)
			{
				// Repulsion
				sf::Vector2f repulsion = movementManager.repulsion(playerUnit->getPos(), otherUnit->getPos(), playerUnit->body.getRadius(), otherUnit->body.getRadius());
				otherUnit->push(-repulsion);
				playerUnit->push(repulsion);

				// Raycasting Check
				Unit avoiding = { otherUnit->getPos(), otherUnit->getRadius() };
				unitsToAvoid.push_back(avoiding);
			}
		}
		// Raycasting finds any units blocking current unit
		if (unitsToAvoid.size() > 0 && playerUnit->blocked == false)
		{
			Unit mainUnit = { playerUnit->getPos(), playerUnit->getRadius(), playerUnit->velocity };
			playerUnit->blocked = raycasting.isBlocked(mainUnit, unitsToAvoid);
		}

		// Movement Update
		if (playerUnit->state != UnitState::Idle && playerUnit->state != UnitState::Attacking)
		{
			sf::Vector2f currentPostion = playerUnit->getPos();

			if (playerUnit->blocked == false) // Flowfield Movement
			{
				playerUnit->velocity = playerUnit->flowfieldMovement.MoveTo(currentPostion);
			}
			else if (playerUnit->blocked == true) // Astar Movement
			{
				playerUnit->astarMovement.getAstar()->setUnitPositions(AStarUnitHandOut(playerUnit.get()));
				playerUnit->velocity = playerUnit->astarMovement.MoveTo(currentPostion, playerUnit->getRadius());

				if (playerUnit->velocity == sf::Vector2{ 0.f,0.f }) // NEEDED TO STOP LAG
				{
					playerUnit->state = UnitState::Idle;
				}
			}

			// Direct movement 
			playerUnit->velocity = movementManager.useDirectMovement(currentPostion, playerUnit->velocity, playerUnit->flowfieldMovement.getFlowfield()->destinationPosition, playerUnit->flowfieldMovement.getFlowfield()->destination->getPosition());


			// Compile all needed variables for adding flocking/swarm weights
			std::vector<sf::Vector2f> unitVelocities;
			std::vector<sf::Vector2f> unitPositions;

			// If selected units is larger than 1 and contains current this unit
			for (auto& group : groups)
			{
				if (group.getUnits().size() > 1 &&
					group.containsUnit(playerUnit.get()))
				{
					for (auto& unit : group.getUnits())
					{
						if (unit == playerUnit.get()) continue; // Skips  itself

						Units* currentUnit = dynamic_cast<Units*>(unit);
						unitVelocities.push_back(currentUnit->velocity);
						unitPositions.push_back(currentUnit->getPos());
					}

					if (playerUnit->blocked == false)
						playerUnit->velocity = movementManager.applyAlignmentAndCohesion(playerUnit->getPos(), playerUnit->velocity, unitVelocities, unitPositions);
				}
			}

			// Stop movement when destination is reached
			if (movementManager.isDestinationReached(playerUnit->getPos(), playerUnit->flowfieldMovement.getFlowfield()))
			{
				playerUnit->state = UnitState::Idle;
				playerUnit->blocked = false;
				playerUnit->destinationReached = true;
			}

		}
		else {
			playerUnit->velocity = { 0,0 };
		}

		playerUnit->update(t_deltaTime);
	}
}

void UnitHandler::eraseDeadUnits(std::vector<std::shared_ptr<Targetable>>& t_units)
{
	t_units.erase(
		std::remove_if(t_units.begin(), t_units.end(),
			[&](std::shared_ptr<Targetable>& unit)
			{
				// Remove from groups if needed (if you have groups for enemies)
				if (!unit->getAlive())
				{
					for (auto& group : groups)
						group.removeUnit(unit.get());

					return true;
				}
				return false;
			}),
		t_units.end()
	);
}

void UnitHandler::spawnUnit(std::string t_type, sf::Vector2f t_position, bool t_friendly)
{
	FlowfieldMovement flowfieldMovement(*mainFlowField);
	AStarMovement astarMovement(*mainAstar);

	if (t_type == "Soldier")
	{
		if (t_friendly)
			playerUnits.push_back(std::make_shared<Soldier>(t_position, flowfieldMovement, astarMovement));
		else
			enemyUnits.push_back(std::make_shared<Soldier>(t_position, flowfieldMovement, astarMovement));
	}
	else if (t_type == "Orc")
	{
		if (t_friendly)
			playerUnits.push_back(std::make_shared<Orc>(t_position, flowfieldMovement, astarMovement));
		else
			enemyUnits.push_back(std::make_shared<Orc>(t_position, flowfieldMovement, astarMovement));
	}
	else if (t_type == "NightElf")
	{
		if (t_friendly)
			playerUnits.push_back(std::make_shared<NightElf>(t_position, flowfieldMovement, astarMovement));
		else
			enemyUnits.push_back(std::make_shared<NightElf>(t_position, flowfieldMovement, astarMovement));
	}
}

void UnitHandler::spawnUnitFromBuilding(std::string t_type, sf::Vector2f t_position, sf::Vector2f t_target, bool t_friendly)
{
	FlowfieldMovement flowfieldMovement(*mainFlowField);
	AStarMovement astarMovement(*mainAstar);

	// Pointer to the newly spawned unit
	std::shared_ptr<Units> newUnit;

	if (t_type == "Soldier")
	{
		newUnit = std::make_shared<Soldier>(t_position, flowfieldMovement, astarMovement);
	}
	else if (t_type == "Orc")
	{
		newUnit = std::make_shared<Orc>(t_position, flowfieldMovement, astarMovement);
	}
	else if (t_type == "NightElf")
	{
		newUnit = std::make_shared<NightElf>(t_position, flowfieldMovement, astarMovement);
	}

	// Move Order
	singleMoveOrder(newUnit.get(), t_target);

	// Now push it into the container
	if (t_friendly)
		playerUnits.push_back(std::move(newUnit));
	else
		enemyUnits.push_back(std::move(newUnit));
}

std::shared_ptr<Targetable> UnitHandler::getFirstUnitInList()
{
	if (!selectedUnits.empty()) {
		Units* rawUnit = *selectedUnits.begin();

		// Find the corresponding shared_ptr in allUnits
		for (const auto& sharedUnit : playerUnits) 
		{
			if (sharedUnit.get() == rawUnit) 
			{
				return sharedUnit; // Return the existing shared_ptr
			}
		}
	}
	return nullptr;
}

void UnitHandler::singleMoveOrder(Units* t_unit, sf::Vector2f t_position)
{
	// Boolean to make sure units continue moving after attacking
	t_unit->destinationReached = false;

	// Flowfield Handout
	mainFlowField->computePath(t_position);
	t_unit->setFlowField(*mainFlowField, UnitState::AttackMove);

	// Astar Handout
	t_unit->astarMovement.getAstar()->setTarget(t_position);
}

std::vector<sf::Vector2f> UnitHandler::formationMoveOrder(UnitState t_state)
{
	std::vector<sf::Vector2f> formationPositions;

	if (selectedUnits.size() <= 0)
		return formationPositions;

	// Create formation
	auto numUnits = selectedUnits.size();
	const float radius = 75;
	const sf::Vector2f center = Mouse::getInstance().getPosition();

	formationPositions.push_back(center);

	// Creates the formation circle
	for (int i = 0; i < numUnits-1; i++)
	{
		float angle = (i * 2 * 3.1416) / numUnits; // Evenly distribute the units around the circle
		sf::Vector2f position(center.x + radius * cos(angle), center.y + radius * sin(angle));
		formationPositions.push_back(position);
	}

	int i = 0;
	// Gives the units flowfields for each position
	for (auto& selected : UnitHandler::getInstance().selectedUnits)
	{
		// Boolean to make sure units continue moving after attacking
		selected->destinationReached = false;

		// Flowfield Handout
		mainFlowField->computePath(formationPositions[i]);
		selected->setFlowField(*mainFlowField, t_state);

		// Astar Handout
		selected->astarMovement.getAstar()->setTarget(formationPositions[i]);

		// Increment
		i++;
	}

	createNewGroupFromSelectedUnits(selectedUnits);

	return formationPositions;
}

sf::Vector2f UnitHandler::attackFollowMoveOrder(Units* t_target)
{
	if (selectedUnits.size() <= 0)
		return { 0,0 };

	sf::Vector2f mousePosition = Mouse::getInstance().getPosition();

	mainFlowField->computePath(mousePosition);
	mainFlowField->setDestinationPosition(mousePosition);

	// Handouts
	for (auto& selected : UnitHandler::getInstance().selectedUnits)
	{
		// Boolean to make sure units continue moving after attacking
		selected->destinationReached = false;

		// Flowfield Handout
		selected->setFlowField(*mainFlowField, UnitState::AttackFollow);

		// AStarHandout
		aStarHandout(mousePosition, selected);
	}

	createNewGroupFromSelectedUnits(selectedUnits, t_target);

	return mousePosition;
}


std::vector<Targetable*> UnitHandler::getUnitsInCellAndNeighbors(int cellID)
{
	std::vector<Targetable*> result;

	// Neighboring cell offsets including the current cell
	std::vector<int> offsets = 
	{ -FlowField::GRID_WIDTH - 1, -FlowField::GRID_WIDTH, -FlowField::GRID_WIDTH + 1,
	  -1, 0, 1,
	  FlowField::GRID_WIDTH - 1, FlowField::GRID_WIDTH, FlowField::GRID_WIDTH + 1 
	};

	for (int offset : offsets) 
	{
		int neighborCellID = cellID + offset;

		if (partitionedMap.find(neighborCellID) != partitionedMap.end()) 
		{
			result.insert(result.end(), partitionedMap[neighborCellID].begin(), partitionedMap[neighborCellID].end());
		}
	}

	return result;
}

void UnitHandler::createNewGroupFromSelectedUnits(const std::unordered_set<Units*>& selectedUnits)
{
	UnitGroup newGroup;

	if (groups.size() != 0) 
	{
		// Remove the selected units from their original groups
		for (auto& unit : selectedUnits)
		{
			// Find the group that contains the unit
			for (auto& group : groups)
			{
				// Remove the unit from the group if it exists
				if (std::find(group.getUnits().begin(), group.getUnits().end(), unit) != group.getUnits().end()) {
					group.removeUnit(unit);
					break;
				}
			}
			newGroup.addUnit(unit);
		}
		groups.push_back(std::move(newGroup));
	}
	else // If there are no groups available skip searching
	{
		for (auto& unit : selectedUnits)
		{
			newGroup.addUnit(unit);
		}
		groups.push_back(std::move(newGroup));
	}

	// Check for empty groups and remove them
	for (auto it = groups.begin(); it != groups.end();) 
	{
		if (it->getUnits().empty()) 
		{
			it = groups.erase(it);
		}
		else {
			++it;
		}
	}
}

void UnitHandler::createNewGroupFromSelectedUnits(const std::unordered_set<Units*>& selectedUnits, Targetable* t_target)
{
	UnitGroup newGroup;

	// Set Target to follow for group
	newGroup.targetUnit(findSharedPtrFromRaw(t_target));

	if (groups.size() != 0)
	{
		// Remove the selected units from their original groups
		for (auto& unit : selectedUnits)
		{
			// Find the group that contains the unit
			for (auto& group : groups)
			{
				// Remove the unit from the group if it exists
				if (std::find(group.getUnits().begin(), group.getUnits().end(), unit) != group.getUnits().end()) {
					group.removeUnit(unit);
					break;
				}
			}
			newGroup.addUnit(unit);
		}
		groups.push_back(std::move(newGroup));
	}
	else // If there are no groups available skip searching
	{
		for (auto& unit : selectedUnits)
		{
			newGroup.addUnit(unit);
		}
		groups.push_back(std::move(newGroup));
	}

	// Check for empty groups and remove them
	for (auto it = groups.begin(); it != groups.end();)
	{
		if (it->getUnits().empty())
		{
			it = groups.erase(it);
		}
		else {
			++it;
		}
	}
}

void UnitHandler::aStarHandout(sf::Vector2f postion, Units* unit)
{
	// Astar Handout
	unit->astarMovement.getAstar()->setTarget(postion);
	std::vector<AstarUnit> astarUnits;
	for (auto& unit : playerUnits)
	{
		std::shared_ptr<Units> playerUnit = std::dynamic_pointer_cast<Units>(unit);
		if (playerUnit->getPos() != unit->getPos())
		{
			AstarUnit astarUnit(playerUnit->getPos(), playerUnit->getDiameter());
			astarUnits.push_back(astarUnit);
		}
	}
	for (auto& unit : enemyUnits)
	{
		std::shared_ptr<Units> enemyUnit = std::dynamic_pointer_cast<Units>(unit);
		if (enemyUnit->getPos() != unit->getPos())
		{
			AstarUnit astarUnit(enemyUnit->getPos(), enemyUnit->getDiameter());
			astarUnits.push_back(astarUnit);
		}
	}
}

std::shared_ptr<Targetable> UnitHandler::findSharedPtrFromRaw(Targetable* rawPtr)
{
	for (auto& unit : playerUnits) 
	{
		if (unit.get() == rawPtr) return unit; // Found in player units		
	}

	for (auto& unit : enemyUnits) 
	{
		if (unit.get() == rawPtr) return unit; // Found in enemy units		
	}

	return nullptr; // Not found
}

std::vector<AstarUnit> UnitHandler::AStarUnitHandOut(Units* t_unit)
{
	std::vector<AstarUnit> astarUnits;
	
	for (auto& unit : playerUnits)
	{
		if (unit.get() != t_unit)
		{
			AstarUnit astarUnit(unit->getPos(), unit->getRadius() * 2);
			astarUnits.push_back(astarUnit);
		}
	}
	for (auto& unit : enemyUnits)
	{
		if (unit.get() != t_unit)
		{
			AstarUnit astarUnit(unit->getPos(), unit->getRadius() * 2);
			astarUnits.push_back(astarUnit);
		}
	}
	return astarUnits;
}

Cell* UnitHandler::selectCell()
{
	for (auto& row : mainFlowField->Grid)
	{
		for (Cell& cell : row)
		{
			if (cell.shape.getGlobalBounds().contains(Mouse::getInstance().getPosition()))
			{
				return &cell;
			}
		}
	}
}

Cell* UnitHandler::selectCell(sf::Vector2f t_position)
{
	for (auto& row : mainFlowField->Grid)
	{
		for (Cell& cell : row)
		{
			if (cell.shape.getGlobalBounds().contains(t_position))
			{
				return &cell;
			}
		}
	}
}

