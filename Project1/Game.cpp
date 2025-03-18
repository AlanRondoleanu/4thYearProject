/// <summary>
/// Author: Alan Rondoleanu
/// Date: 24/10/2024
/// </summary>

#include "Game.h"

/// <summary>
/// default constructor
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 1600U, 1200U, 32U }, "SFML Game" },
	unitUI(m_window),
	resourceManager(m_window),
	m_exitGame{ false },
	tileMap(flowfield.GRID_WIDTH, flowfield.GRID_HEIGHT, flowfield.CELL_WIDTH),
	buildingManager(sf::Vector2f{ flowfield.GRID_WIDTH * flowfield.CELL_WIDTH, flowfield.GRID_HEIGHT * flowfield.CELL_WIDTH })
{
	// Ui View
	uiView = m_window.getDefaultView();

	// Giving the flowfield to the unit handler
	UnitHandler::getInstance().setMovementFields(&flowfield, &astar);

	createCallbacks();
}

/// <summary>
/// default destructor
/// </summary>
Game::~Game()
{
}

/// <summary>
/// main game loop
/// updates 60 times per second
/// </summary>
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps

	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handles events and inputs fron the user
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if (sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
		if (sf::Event::KeyReleased == newEvent.type)
		{
			processKeyReleases(newEvent);
		}
		if (sf::Event::MouseButtonPressed == newEvent.type)
		{
			processMouse(newEvent);
		}
		if (sf::Event::MouseWheelScrolled == newEvent.type)
		{
			if (newEvent.mouseWheelScroll.delta > 0) 
			{
				camera.zoom(0.9f); // Zoom in
			}
			else {
				camera.zoom(1.1f); // Zoom out
			}
			m_window.setView(camera);
		}

		
		selector.handleEvent(newEvent, unitUI.isInsideUI(Mouse::getInstance().getPositionOnScreen()));
	}
}


/// <summary>
/// handles the key presses by the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::F1 == t_event.key.code)
	{
		m_exitGame = true;
	}
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		buildingMode = false;
	}
	if (sf::Keyboard::Q == t_event.key.code)
	{
		UnitHandler::getInstance().spawnUnit("NightElf", Mouse::getInstance().getPosition(), true);
	}
	if (sf::Keyboard::E == t_event.key.code)
	{
		UnitHandler::getInstance().spawnUnit("Soldier", Mouse::getInstance().getPosition(), false);
	}
	if (sf::Keyboard::A == t_event.key.code)
	{
		for (auto& markerLocation : UnitHandler::getInstance().formationMoveOrder(UnitState::AttackMove))
		{
			PlaceMarkers newMarker(markerLocation);
			moveMarkers.push_back(newMarker);
		}
	}
	if (sf::Keyboard::P == t_event.key.code)
	{
		debug = !debug;
	}

	// Camera movement
	if (sf::Keyboard::Up == t_event.key.code)
	{
		cameraMovement = { cameraMovement.x, -cameraSpeed };
	}
	if (sf::Keyboard::Left == t_event.key.code)
	{
		cameraMovement = { -cameraSpeed, cameraMovement.y };
	}
	if (sf::Keyboard::Down == t_event.key.code)
	{
		cameraMovement = { cameraMovement.x, cameraSpeed };
	}
	if (sf::Keyboard::Right == t_event.key.code)
	{
		cameraMovement = { cameraSpeed, cameraMovement.y };
	}
}

/// <summary>
/// Processes the keys released on the keyboard
/// </summary>
/// <param name="t_event">the key released</param>
void Game::processKeyReleases(sf::Event t_event)
{
	// Camera movement
	if (sf::Keyboard::Up == t_event.key.code)
	{
		cameraMovement = { cameraMovement.x, 0 };
	}
	if (sf::Keyboard::Left == t_event.key.code)
	{
		cameraMovement = { 0, cameraMovement.y };
	}
	if (sf::Keyboard::Down == t_event.key.code)
	{
		cameraMovement = { cameraMovement.x, 0 };
	}
	if (sf::Keyboard::Right == t_event.key.code)
	{
		cameraMovement = { 0, cameraMovement.y };
	}
}

void Game::processMouse(sf::Event t_event)
{
	if (sf::Mouse::Left == t_event.mouseButton.button)
	{
		if (unitUI.insideUI == true)
		{
			// UI Clicking
			unitUI.HandleClick(Mouse::getInstance().getPositionOnScreen());
		}
		else {
			if (buildingMode && !selectedBuildingType.empty() &&
				resourceManager.checkCost("Gold", resourceManager.getCost(selectedBuildingType)))
			{
				if (buildingManager.addBuilding(selectedBuildingType, Mouse::getInstance().getPositionWithGrid(), false))
				{
					resourceManager.spendResource("Gold", resourceManager.getCost(selectedBuildingType));
					buildingMode = false;
					selectedBuildingType = "";
				}			
			}
			else {
				buildingMode = false;
				selectedBuildingType = "";
			}
		}
	}
	if (sf::Mouse::Right == t_event.mouseButton.button)
	{
		if (unitUI.insideUI == false)
		{
			if (!buildingMode)
			{
				if (Mouse::getInstance().hoveredState == MouseHover::Enemy)
				{
					sf::Vector2f markerLocation = UnitHandler::getInstance().attackFollowMoveOrder(Mouse::getInstance().getHoveredUnit());
					PlaceMarkers newMarker(markerLocation);
					moveMarkers.push_back(newMarker);
				}
				else
				{
					for (auto& markerLocation : UnitHandler::getInstance().formationMoveOrder(UnitState::Moving))
					{
						PlaceMarkers newMarker(markerLocation);
						moveMarkers.push_back(newMarker);
					}
				}				
			}
		}
	}
}

/// <summary>
/// Update the game
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}

	// Mouse Position
	Mouse::getInstance().UpdateMousePostion(m_window, camera);

	// Mouse Hover Events
	Mouse::getInstance().updateHovering(UnitHandler::getInstance().enemyUnits, UnitHandler::getInstance().playerUnits);

	// Move Marker
	for (auto it = moveMarkers.begin(); it != moveMarkers.end(); )
	{
		it->update();
		if (it->isExpired()) 
		{
			it = moveMarkers.erase(it); // Remove expired marker
		}
		else {
			++it;
		}
	}
	
	// Update Units
	UnitHandler::getInstance().update(t_deltaTime.asSeconds());

	// Combat Update
	combat.update(UnitHandler::getInstance().playerUnits, UnitHandler::getInstance().enemyUnits, buildingManager.playerBuildings, buildingManager.enemyBuildings, t_deltaTime.asSeconds());

	// Update UI
	unitUI.SetSelectedUnit(UnitHandler::getInstance().getFirstUnitInList());

	// Building Update
	buildingManager.update(t_deltaTime.asSeconds(), buildingMode);

	// Camera
	float delta = t_deltaTime.asMilliseconds();
	sf::Vector2f cameraOffset = { cameraMovement.x * delta , cameraMovement.y * delta };
	camera.move(cameraOffset);


	// Every other second update
	if (tickEverySecond())
	{
		resourceManager.addResource("Gold", 1);
	}
}

/// <summary>
/// draws the sprites and displays the window
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::White);

	// Map Texture
	tileMap.draw(m_window);

	// Grid Render
	if (debug)
		flowfield.render(m_window);

	// Move Markers
	for (auto& marker : moveMarkers)
	{
		marker.render(m_window);
	}

	// Building Render
	buildingManager.draw(m_window, buildingMode);

	// Unit rendering
	UnitHandler::getInstance().render(m_window);

	// Render for projectiles
	combat.renderProjectiles(m_window);

	// Selector Render
	selector.render(m_window);

	// Draw Unit UI
	unitUI.Render(m_window, uiView);

	// Draw Resources
	resourceManager.Render(m_window, uiView);

	// Camera
	m_window.setView(camera);

	// Display window
	m_window.display();
}

void Game::createCallbacks()
{
	unitUI.SetDefaultButtonAction(0, [&]() {
		buildingMode = true;
		selectedBuildingType = "Orc_Building";
		});

	unitUI.SetDefaultButtonAction(1, [&]() {
		buildingMode = true;
		selectedBuildingType = "Soldier_Building";
		});

	unitUI.SetDefaultButtonAction(2, [&]() {
		buildingMode = true;
		selectedBuildingType = "NightElf_Building";
		});

	unitUI.SetDefaultButtonAction(3, [&]() {
		selectedBuildingType = "4";
		});

	unitUI.SetDefaultButtonAction(4, [&]() {
		selectedBuildingType = "5";
		});

	unitUI.SetDefaultButtonAction(5, [&]() {
		selectedBuildingType = "6";
		});

	unitUI.SetDefaultButtonAction(6, [&]() {
		selectedBuildingType = "7";
		});

	unitUI.SetDefaultButtonAction(7, [&]() {
		selectedBuildingType = "8";
		});
}

bool Game::tickEverySecond()
{
	static sf::Clock clock;
	static float interval = 1.0f; // Seconds

	if (clock.getElapsedTime().asSeconds() >= interval)
	{
		clock.restart();
		return true;
	}

	return false;
}
