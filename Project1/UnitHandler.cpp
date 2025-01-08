#include "UnitHandler.h"

void UnitHandler::update()
{
	partitionedMap.clear();
	for (auto& playerUnit : playerUnits)
	{
		// Spacial Partition
		partitionedMap[playerUnit->cellID].push_back(playerUnit.get());
	}

	// Update for players
	for (auto& playerUnit : playerUnits)
	{
		playerUnit->setCellID(mainFlowField);

		// Movement Update
		if (playerUnit->moving)
		{
			playerUnit->velocity = movementManager.applyFlowFieldDirection(playerUnit->getPos(), &playerUnit->flowfield);
			
			if (selectedUnits.size() > 1 &&
				selectedUnits.find(playerUnit.get()) != selectedUnits.end())
			{
				applyAlignmentAndCohesion(playerUnit.get());
			}

			if (movementManager.isDestinationReached(playerUnit->getPos(), &playerUnit->flowfield))
			{
				playerUnit->moving = false;
			}		
		}
		else {
			playerUnit->velocity = { 0,0 };
		}

		// Repulsion
		for (auto& otherUnit : getUnitsInCellAndNeighbors(playerUnit->cellID))
		{
			if (playerUnit.get() != otherUnit)
			{
				sf::Vector2f repulsion = movementManager.repulsion(playerUnit->getPos(), otherUnit->getPos(), playerUnit->body.getRadius(), otherUnit->body.getRadius());
				playerUnit->push(repulsion);
			}
		}

		playerUnit->update();
	}
	// Update for enemies
	for (auto& enemyUnit : enemyUnits)
	{
		enemyUnit->update();
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
	playerUnits.push_back(std::make_unique<Soldier>());
	playerUnits.back()->setPos(Mouse::getInstance().getPosition());
}

std::vector<sf::Vector2f> UnitHandler::unitMoveOrder()
{
	mainFlowField->resetField();
	mainFlowField->setDestinationCell(selectCell());
	mainFlowField->createIntegrationField();
	mainFlowField->createFlowField();

	// Create formation
	const int numUnits = selectedUnits.size();
	const float radius = 75;
	const sf::Vector2f center = Mouse::getInstance().getPosition();

	std::vector<sf::Vector2f> formationPositions;
	formationPositions.push_back(center);

	// Creates the formation circle
	for (int i = 0; i < numUnits-1; i++)
	{
		float angle = (i * 2 * 3.1416) / numUnits; // Evenly distribute the units around the circle
		sf::Vector2f position(center.x + radius * cos(angle), center.y + radius * sin(angle));
		formationPositions.push_back(position);
	}

	int i = 0;
	for (auto& selected : UnitHandler::getInstance().selectedUnits)
	{
		mainFlowField->setDestinationPosition(formationPositions[i]);
		selected->setFlowField(*mainFlowField);
		selected->moving = true;
		i++;
	}

	return formationPositions;
}


std::vector<Units*> UnitHandler::getUnitsInCellAndNeighbors(int cellID)
{
	std::vector<Units*> result;

	// Neighboring cell offsets (including the current cell)
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

void UnitHandler::applyAlignmentAndCohesion(Units* t_self)
{
	// Alignment Weights
	const float ALIGNMENT_WEIGHT = 0.5f;   // Influence of alignment
	const float FLOWFIELD_WEIGHT = 0.5f;   // Influence of the flow field
	// Cohesion Weights
	const float COHESION_WEIGHT = 0.005f;   // Influence of cohesion

	sf::Vector2f alignmentVector = { 0, 0 };
	sf::Vector2f cohesionVector = { 0.f, 0.f };

	// Selected Unit count
	int selectedCount = 0;

	for (auto& unit : selectedUnits)
	{
		if (unit == t_self) continue; // Skips  itself

		alignmentVector += unit->velocity;
		cohesionVector += unit->getPos();
		selectedCount++;
	}

	// Average the vectors
	alignmentVector /= static_cast<float>(selectedCount);
	cohesionVector /= static_cast<float>(selectedCount);
	cohesionVector = cohesionVector - t_self->getPos();

	// Combine alignment, cohesion and flow field vectors
	sf::Vector2f combinedVector = 
		(ALIGNMENT_WEIGHT * alignmentVector) + 
		(COHESION_WEIGHT * cohesionVector) +
		(FLOWFIELD_WEIGHT * t_self->velocity);

	// Normalize
	float length = std::sqrt(combinedVector.x * combinedVector.x + combinedVector.y * combinedVector.y);
	if (length > 0) 
	{
		combinedVector /= length; 
	}

	// Update the unit's velocity
	t_self->velocity = combinedVector;
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