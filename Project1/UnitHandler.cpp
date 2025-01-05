#include "UnitHandler.h"

void UnitHandler::update()
{
	// Update for players
	for (auto& playerUnit : playerUnits)
	{
		playerUnit->update();

		// Movement Update
		if (playerUnit->moving)
		{
			sf::Vector2f direction = movementManager.move(playerUnit->getPos(), &playerUnit->flowfield);
			playerUnit->moveUnit(direction);
			playerUnit->moving = movementManager.isDestinationReached(playerUnit->getPos(), &playerUnit->flowfield);
		}
		// Repulsion Update
		for (auto& otherUnit : playerUnits)
		{
			if (playerUnit != otherUnit)
			{
				sf::Vector2f repulsion = movementManager.repulsion(playerUnit->getPos(), otherUnit->getPos(), playerUnit->body.getRadius(), otherUnit->body.getRadius());
				playerUnit->moveUnit(repulsion);
			}
		}
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
