/// <summary>
/// Author: Alan Rondoleanu
/// Date: 15/11/2021
/// </summary>
#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Buildings.h"
#include "Spawner.h"
#include "Units.h"
#include "Soldier.h"
#include "FlowField.h"

class Game
{
public:
	Game();
	~Game();

	void run();

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void processKeyReleases(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();
	void initialize();
	void spawnUnits();
	void placeBuilding();
	Cell* selectCell();

	// Grid
	FlowField flowfield;

	// Mouse positions
	sf::Vector2f gridPlacement();
	void getMousePosition(sf::RenderWindow& t_window);
	sf::Vector2i mousePosition;
	sf::Vector2f mousePostion_Mapped;
	sf::Vector2f mouseGridPlacement;

	Buildings* playerMainBuilding;
	Buildings* enemyMainBuilding;
	Buildings* placementTemp;
	std::vector<Buildings*> playerBuildings;
	Buildings* enemyBuildings[10];
	Units* playerUnits[Units::MAX_UNITS];
	Units* enemyUnits[Units::MAX_UNITS];

	sf::RenderWindow m_window; // main SFML window
	sf::Sprite backgroundSprite; // the sprite for the background
	sf::Texture gameBackground; // The background for the game
	
	//Camera
	sf::View camera;
	sf::Vector2f cameraMovement;
	float cameraSpeed{ 0.5 };

	bool m_exitGame; // control exiting game

};

#endif // !GAME_HPP

