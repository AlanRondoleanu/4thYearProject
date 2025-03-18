#ifndef BUILDINGMANAGER_H
#define BUILDINGMANAGER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Buildings.h"
#include "Spawner.h"
#include "Soldier_Building.h"
#include "Orc_Building.h"
#include "NightElf_Building.h"
#include "Mouse.h"
#include "UnitHandler.h"
#include "Turret.h"
#include "Turret_Fantasy.h"
#include "Nexus.h"
#include "Nexus_Fantasy.h"
#include "Placement_Building.h"

class BuildingManager
{
public:
    BuildingManager(sf::Vector2f t_mapSize);
    bool addBuilding(std::string t_type, const sf::Vector2f& position, bool isEnemy);
    void update(float deltaTime, bool buildingMode);
    void draw(sf::RenderWindow& window, bool buildingMode);


    void HandlePlacementBuilding();

    std::vector<std::shared_ptr<Targetable>> playerBuildings;
    std::vector<std::shared_ptr<Targetable>> enemyBuildings;

private:

    void eraseDeadBuildings(std::vector<std::shared_ptr<Targetable>>& t_units);
    bool isFullyInside(const sf::FloatRect& inner, const sf::FloatRect& outer);
    Buildings* placementTemp;
    sf::Vector2f mapSize;
    sf::Clock timerClock;
    float timerInterval = 10;
};

#endif