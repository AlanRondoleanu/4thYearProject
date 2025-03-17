#include "BuildingManager.h"

BuildingManager::BuildingManager()
{
	placementTemp = new Spawner({0,0}, false);

	sf::Vector2f playerSpawner = { 625, 2425 };
	sf::Vector2f playerTurret = { 625, 2025 };
	sf::Vector2f enemySpawner = { 625, 75 };
	sf::Vector2f enemyTurret = { 625, 475 };

	playerBuildings.push_back(std::make_unique<Spawner>(playerSpawner, false));
	playerBuildings.push_back(std::make_unique<Turret>(playerTurret, false));
	enemyBuildings.push_back(std::make_unique<Spawner>(enemySpawner, true));
	enemyBuildings.push_back(std::make_unique<Turret>(enemyTurret, true));
}

void BuildingManager::addBuilding(std::string t_type, const sf::Vector2f& position, bool isEnemy)
{
	if (placementTemp->body.getFillColor() == sf::Color::Green)
	{
		if (t_type == "Spawner")
		{
			playerBuildings.push_back(std::make_unique<Spawner>(position, isEnemy));
		}
		else if (t_type == "Soldier_Building")
		{
			playerBuildings.push_back(std::make_unique<Soldier_Building>(position, isEnemy));
		}
		else if (t_type == "Orc_Building")
		{
			playerBuildings.push_back(std::make_unique<Orc_Building>(position, isEnemy));
		}
	}
}

void BuildingManager::update(float deltaTime, bool buildingMode)
{
	// Check if 30 seconds have passed
	if (timerClock.getElapsedTime().asSeconds() >= timerInterval)
	{
		// Player Unit Spawn
		for (auto& building : playerBuildings)
		{
			if (building->GetUnitType() == "Soldier_Building")
			{
				sf::Vector2f enemySpawner = { 625, 75 };
				UnitHandler::getInstance().spawnUnitFromBuilding("Soldier", building->getSpawnPoint(), enemySpawner, true);
			}
			else if (building->GetUnitType() == "Orc_Building")
			{
				sf::Vector2f enemySpawner = { 625, 75 };
				UnitHandler::getInstance().spawnUnitFromBuilding("Orc", building->getSpawnPoint(), enemySpawner, true);
			}
		}
		// Enemy Unit Spawn
		for (auto& building : enemyBuildings)
		{
			if (building->GetUnitType() == "Soldier_Building")
			{
				sf::Vector2f playerSpawner = { 625, 2425 };
				UnitHandler::getInstance().spawnUnitFromBuilding("Soldier", building->getSpawnPoint(), playerSpawner, false);
			}
			else if (building->GetUnitType() == "Orc_Building")
			{
				sf::Vector2f enemySpawner = { 625, 75 };
				UnitHandler::getInstance().spawnUnitFromBuilding("Orc", building->getSpawnPoint(), enemySpawner, true);
			}
		}

		// Reset the timer
		timerClock.restart();
	}

	for (auto& building : playerBuildings)
	{
		building->update(deltaTime);
	}
	for (auto& building : enemyBuildings)
	{
		building->update(deltaTime);
	}

	if (buildingMode)
	{
		HandlePlacementBuilding();
	}
}

void BuildingManager::draw(sf::RenderWindow& window, bool buildingMode)
{
	for (auto& building : playerBuildings)
	{
		building->draw(window);
	}
	for (auto& building : enemyBuildings)
	{
		building->draw(window);
	}

	if (buildingMode)
		placementTemp->draw(window);
}

void BuildingManager::HandlePlacementBuilding()
{
	placementTemp->setPos(Mouse::getInstance().getPositionWithGrid());
	placementTemp->body.setFillColor(sf::Color::Green);
	placementTemp->setBlocked(false);

	for (auto& building : playerBuildings)
	{
		if (building->body.getGlobalBounds().intersects(placementTemp->body.getGlobalBounds()))
		{
			placementTemp->body.setFillColor(sf::Color::Red);
			placementTemp->setBlocked(true);
		}
	}
}
