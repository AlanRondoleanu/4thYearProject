#ifndef BUILDINGMANAGER_H
#define BUILDINGMANAGER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Buildings.h"
#include "Spawner.h"
#include "Soldier_Building.h"
#include "Orc_Building.h"
#include "Mouse.h"
#include "UnitHandler.h"
#include "Turret.h"

class BuildingManager
{
public:
    BuildingManager();
    void addBuilding(std::string t_type, const sf::Vector2f& position, bool isEnemy);
    void update(float deltaTime, bool buildingMode);
    void draw(sf::RenderWindow& window, bool buildingMode);


    void HandlePlacementBuilding();

    std::vector<std::shared_ptr<Buildings>> playerBuildings;
    std::vector<std::shared_ptr<Buildings>> enemyBuildings;

private:

    Buildings* placementTemp;

    sf::Clock timerClock;
    float timerInterval = 10;
};

#endif