#include "UnitHandler.h"

void UnitHandler::update()
{
	// Spacial Partition
	partitionedMap.clear();
	for (auto& playerUnit : playerUnits)
	{
		partitionedMap[playerUnit->cellID].push_back(playerUnit.get());
	}

	// Update for player units
	for (auto& playerUnit : playerUnits)
	{
		playerUnit->setCellID();

		// Movement Update
		if (playerUnit->state != UnitState::Idle)
		{
			/*
			Cell currentCell = playerUnit->flowfield.Grid[gridY][gridX];
			sf::Vector2f direction = currentCell.getDirection();
			playerUnit->velocity = movementManager.applyFlowFieldDirection(currentPostion, direction, playerUnit->flowfield.destinationPosition, playerUnit->flowfield.destination->getPosition());
			playerUnit->flowfieldDirection = direction;*/

			// NEW SYSTEM
			sf::Vector2f currentPostion = playerUnit->getPos();
			playerUnit->velocity = playerUnit->flowfieldMovement.MoveTo(currentPostion);
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

						unitVelocities.push_back(unit->velocity);
						unitPositions.push_back(unit->getPos());
					}
					playerUnit->velocity = movementManager.applyAlignmentAndCohesion(playerUnit->getPos(), playerUnit->velocity, unitVelocities, unitPositions);
				}	
			}

			// Stop movement when destination is reached
			if (movementManager.isDestinationReached(playerUnit->getPos(), playerUnit->flowfieldMovement.getFlowfield()))
			{
				playerUnit->state = UnitState::Idle;
			}		
		}
		else {
			playerUnit->velocity = { 0,0 };
		}

		// Partitioned Update
		for (auto& otherUnit : getUnitsInCellAndNeighbors(playerUnit->cellID))
		{
			if (playerUnit.get() != otherUnit)
			{
				// Repulsion
				sf::Vector2f repulsion = movementManager.repulsion(playerUnit->getPos(), otherUnit->getPos(), playerUnit->body.getRadius(), otherUnit->body.getRadius());
				playerUnit->push(repulsion);
			}
		}

		// Raycasting Avoidance
		//if (playerUnit->state == UnitState::Moving)
		//{
		//	Unit raycastingUnit = { playerUnit->getPos(), playerUnit->getRadius(), playerUnit->flowfieldDirection };
		//	std::vector<Unit> unitsToAvoid;

		//	for (auto& otherUnit : getUnitsInCellAndNeighbors(playerUnit->cellID))
		//	{
		//		if (playerUnit.get() != otherUnit)
		//		{
		//			Unit avoiding = { otherUnit->getPos(), otherUnit->getRadius() };
		//			unitsToAvoid.push_back(avoiding);
		//		}
		//	}

		//	// WIP
		//	std::vector<Unit> unitsBlocked = raycasting.getBlockingUnits(raycastingUnit, unitsToAvoid);

		//	bool isBlocked = unitsBlocked.size() > 0;
		//	if (isBlocked)
		//	{
		//		playerUnit->velocity = raycasting.calculateAvoidanceDirection(raycastingUnit, unitsBlocked);
		//		std::cout << "Avoidance Velocity: " << playerUnit->velocity.x << ", " << playerUnit->velocity.y << std::endl;
		//	}
		//}

		playerUnit->update();
	}

	// Groups update
	for (auto& group : groups)
	{
		// Checks if the target of the group has moved from it's current cell to create another flowfield to follow
		if (group.getTarget() != nullptr &&
			group.getTargetLastCellID() != group.getTarget()->cellID)
		{
			group.refreshLastCellID();

			mainFlowField->computePath(group.getTarget()->getPos());

			for (auto& units : group.getUnits())
			{
				mainFlowField->setDestinationPosition(group.getTarget()->getPos());
				units->setFlowField(*mainFlowField, UnitState::AttackFollow);
			}
		}
	}
}

void UnitHandler::render(sf::RenderWindow& t_window)
{
	// Render for players
	for (auto& playerUnit : playerUnits)
	{
		playerUnit->draw(t_window);
	}
	// Render for enemies
	for (auto& enemyUnit : enemyUnits)
	{
		enemyUnit->draw(t_window);
	}
}

void UnitHandler::spawnUnit()
{
	FlowfieldMovement flowfieldMovement(*mainFlowField);
	AStarMovement astarMovement(*mainAstar);

	playerUnits.push_back(std::make_unique<Soldier>(Mouse::getInstance().getPosition(), flowfieldMovement, astarMovement));
}

std::vector<sf::Vector2f> UnitHandler::formationMoveOrder()
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
		// Flowfield Handout
		mainFlowField->computePath(formationPositions[i]);
		selected->setFlowField(*mainFlowField, UnitState::Moving);

		// Astar Handout
		selected->astarMovement.getAstar()->setTarget(formationPositions[i]);
		std::vector<AstarUnit> astarUnits;
		for (auto& playerUnit : playerUnits)
		{
			if (playerUnit->getPos() != selected->getPos())
			{
				AstarUnit astarUnit(playerUnit->getPos(), playerUnit->getDiameter());
				astarUnits.push_back(astarUnit);
			}
		}
		selected->astarMovement.getAstar()->setUnitPositions(astarUnits);

		// Increment
		i++;
	}

	createNewGroupFromSelectedUnits(selectedUnits);

	return formationPositions;
}

sf::Vector2f UnitHandler::attackFollowMoveOrder()
{
	if (selectedUnits.size() <= 0)
		return { 0,0 };

	sf::Vector2f mousePosition = Mouse::getInstance().getPosition();

	mainFlowField->computePath(mousePosition);
	mainFlowField->setDestinationPosition(mousePosition);

	for (auto& selected : UnitHandler::getInstance().selectedUnits)
	{
		selected->setFlowField(*mainFlowField, UnitState::AttackFollow);
	}

	createNewGroupFromSelectedUnits(selectedUnits, Mouse::getInstance().getHovered());

	return mousePosition;
}


std::vector<Units*> UnitHandler::getUnitsInCellAndNeighbors(int cellID)
{
	std::vector<Units*> result;

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

void UnitHandler::createNewGroupFromSelectedUnits(const std::unordered_set<Units*>& selectedUnits, Units* t_target)
{
	UnitGroup newGroup;

	// Set Target to follow for group
	newGroup.targetUnit(t_target);

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
