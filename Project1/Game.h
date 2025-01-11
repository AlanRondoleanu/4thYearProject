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
#include "FlowField.h"
#include "SelectorTool.h"
#include "Mouse.h"
#include "UnitHandler.h"
#include "PlaceMarkers.h"

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
	void processMouse(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();
	void initialize();
	void placeBuilding();

	// Grid
	FlowField flowfield;

	//Selector Tool
	SelectorTool selector;
	std::vector<PlaceMarkers> moveMarkers;

	Buildings* playerMainBuilding;
	Buildings* enemyMainBuilding;
	Buildings* placementTemp;
	std::vector<Buildings*> playerBuildings;
	Buildings* enemyBuildings[10];

	sf::RenderWindow m_window; // main SFML window
	sf::Sprite backgroundSprite; // the sprite for the background
	sf::Texture gameBackground; // The background for the game
	
	enum modes {
		Building,
		Selecting
	};
	modes currentMode{ Selecting };

	enum mouseHover {
		Nothing,
		UI,
		Enemy
	};
	mouseHover currentHover{ Nothing };

	//Camera
	sf::View camera;
	sf::Vector2f cameraMovement;
	float cameraSpeed{ 0.5 };

	bool m_exitGame; // control exiting game

};

#endif // !GAME_HPP

