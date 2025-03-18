#include "BuildingManager.h"

BuildingManager::BuildingManager(sf::Vector2f t_mapSize)
	: mapSize(t_mapSize)
{
	placementTemp = new Placement_Building({0,0}, false);
	placementTemp->setDebug(true);

	sf::Vector2f mapSize = { 1250, 2500 };

	// Needed Buildings
	sf::Vector2f playerSpawner = { mapSize.x / 2, mapSize.y * 0.95f };
	//playerBuildings.push_back(std::make_unique<Orc_Building>(playerSpawner, false));
	sf::Vector2f enemySpawner = { mapSize.x / 2, mapSize.y * 0.05f };
	//enemyBuildings.push_back(std::make_unique<Soldier_Building>(enemySpawner, true));
	sf::Vector2f playerTurret = { mapSize.x / 2, mapSize.y * 0.85f };
	playerBuildings.push_back(std::make_unique<Nexus_Fantasy>(playerTurret, false));
	sf::Vector2f enemyTurret = { mapSize.x / 2, mapSize.y * 0.15f };
	enemyBuildings.push_back(std::make_unique<Nexus>(enemyTurret, true));
	sf::Vector2f playerTurret2 = { mapSize.x / 2, mapSize.y * 0.65f };
	playerBuildings.push_back(std::make_unique<Turret_Fantasy>(playerTurret2, false));
	sf::Vector2f enemyTurret2 = { mapSize.x / 2, mapSize.y * 0.35f };
	enemyBuildings.push_back(std::make_unique<Turret>(enemyTurret2, true));

	// Additional Buildings

}

bool BuildingManager::addBuilding(std::string t_type, const sf::Vector2f& position, bool isEnemy)
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
		else if (t_type == "NightElf_Building")
		{
			playerBuildings.push_back(std::make_unique<NightElf_Building>(position, isEnemy));
		}
		return true;
	}
	return false;
}

void BuildingManager::update(float deltaTime, bool buildingMode)
{
	// Erase Buildings that died
	eraseDeadBuildings(playerBuildings);

	// Check if 30 seconds have passed
	if (timerClock.getElapsedTime().asSeconds() >= timerInterval)
	{
		// Player Unit Spawn
		for (auto& playerBuilding : playerBuildings)
		{
			std::shared_ptr<Buildings> building = std::dynamic_pointer_cast<Buildings>(playerBuilding);
			sf::Vector2f enemySpawner = { 625, 75 };

			if (building->GetUnitType() == "Soldier_Building")
			{
				UnitHandler::getInstance().spawnUnitFromBuilding("Soldier", building->getSpawnPoint(), enemySpawner, true);
			}
			else if (building->GetUnitType() == "Orc_Building")
			{
				UnitHandler::getInstance().spawnUnitFromBuilding("Orc", building->getSpawnPoint(), enemySpawner, true);
			}
			else if (building->GetUnitType() == "NightElf_Building")
			{
				UnitHandler::getInstance().spawnUnitFromBuilding("NightElf", building->getSpawnPoint(), enemySpawner, true);
			}
		}
		// Enemy Unit Spawn
		for (auto& enemyBuilding : enemyBuildings)
		{
			std::shared_ptr<Buildings> building = std::dynamic_pointer_cast<Buildings>(enemyBuilding);
			sf::Vector2f playerSpawner = { 625, 2425 };

			if (building->GetUnitType() == "Soldier_Building")
			{
				UnitHandler::getInstance().spawnUnitFromBuilding("Soldier", building->getSpawnPoint(), playerSpawner, false);
			}
			else if (building->GetUnitType() == "Orc_Building")
			{
				UnitHandler::getInstance().spawnUnitFromBuilding("Orc", building->getSpawnPoint(), playerSpawner, false);
			}
			else if (building->GetUnitType() == "NightElf_Building")
			{
				UnitHandler::getInstance().spawnUnitFromBuilding("NightElf", building->getSpawnPoint(), playerSpawner, false);
			}
		}

		// Reset the timer
		timerClock.restart();
	}

	for (auto& building : playerBuildings)
	{
		if (auto build = std::dynamic_pointer_cast<Buildings>(building))
		{
			build->update(deltaTime);
		}
	}
	for (auto& building : enemyBuildings)
	{
		if (auto build = std::dynamic_pointer_cast<Buildings>(building))
		{
			build->update(deltaTime);
		}
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
		if (auto build = std::dynamic_pointer_cast<Buildings>(building))
		{
			build->draw(window);
		}
	}
	for (auto& building : enemyBuildings)
	{
		if (auto build = std::dynamic_pointer_cast<Buildings>(building))
		{
			build->draw(window);
		}
	}

	if (buildingMode)
		placementTemp->draw(window);
}

void BuildingManager::HandlePlacementBuilding()
{
	placementTemp->setPos(Mouse::getInstance().getPositionWithGrid());
	placementTemp->body.setFillColor(sf::Color::Green);
	placementTemp->setBlocked(false);

	float mapWidth = mapSize.x;
	float mapHeight = mapSize.y;
	float startY = mapHeight * 0.9f; // 90% of map not allowed to place

	sf::FloatRect allowedArea(0.f, startY, mapWidth, mapHeight - startY);
	sf::FloatRect bounds = placementTemp->body.getGlobalBounds();

	if (!isFullyInside(bounds, allowedArea))
	{
		placementTemp->body.setFillColor(sf::Color::Red);
		placementTemp->setBlocked(true);
	}

	for (auto& building : playerBuildings)
	{
		auto build = std::dynamic_pointer_cast<Buildings>(building);

		if (build->body.getGlobalBounds().intersects(placementTemp->body.getGlobalBounds()))
		{
			placementTemp->body.setFillColor(sf::Color::Red);
			placementTemp->setBlocked(true);
		}
	}
}

bool BuildingManager::isFullyInside(const sf::FloatRect& inner, const sf::FloatRect& outer)
{
	return
		inner.left >= outer.left &&
		inner.top >= outer.top &&
		(inner.left + inner.width) <= (outer.left + outer.width) &&
		(inner.top + inner.height) <= (outer.top + outer.height);
}

void BuildingManager::eraseDeadBuildings(std::vector<std::shared_ptr<Targetable>>& t_units)
{
	t_units.erase(
		std::remove_if(t_units.begin(), t_units.end(),
			[&](std::shared_ptr<Targetable>& unit)
			{
				// Remove from groups if needed (if you have groups for enemies)
				if (!unit->getAlive())
				{
					return true;
				}
				return false;
			}),
		t_units.end()
	);
}