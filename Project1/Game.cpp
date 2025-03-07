/// <summary>
/// Author: Alan Rondoleanu
/// Date: 24/10/2024
/// </summary>

#include "Game.h"

int Buildings::PLAYER_BUILDING_AMOUNT = 0;
int Buildings::ENEMY_BUILDING_AMOUNT = 0;

/// <summary>
/// default constructor
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 1600U, 1200U, 32U }, "SFML Game" },
	m_exitGame{ false } //when true game will exit

{
	initialize();
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

		selector.handleEvent(newEvent);
	}
}


/// <summary>
/// handles the key presses by the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
	if (sf::Keyboard::Num1 == t_event.key.code)
	{
		currentMode = Selecting;
	}
	if (sf::Keyboard::Num2 == t_event.key.code)
	{
		currentMode = Building;
	}
	if (sf::Keyboard::Q == t_event.key.code)
	{
		UnitHandler::getInstance().spawnUnit(true);
	}
	if (sf::Keyboard::E == t_event.key.code)
	{
		UnitHandler::getInstance().spawnUnit(false);
	}
	if (sf::Keyboard::A == t_event.key.code)
	{
		for (auto& markerLocation : UnitHandler::getInstance().formationMoveOrder(UnitState::AttackMove))
		{
			PlaceMarkers newMarker(markerLocation);
			moveMarkers.push_back(newMarker);
		}
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
		if (currentMode == Building)
			placeBuilding();
	}
	if (sf::Mouse::Right == t_event.mouseButton.button)
	{
		if (currentMode == Selecting)
		{
			if (currentHover == Nothing)
			{
				for (auto& markerLocation : UnitHandler::getInstance().formationMoveOrder(UnitState::Moving))
				{
					PlaceMarkers newMarker(markerLocation);
					moveMarkers.push_back(newMarker);
				}
			}
			else if (currentHover == Enemy)
			{
				sf::Vector2f markerLocation = UnitHandler::getInstance().attackFollowMoveOrder();
				PlaceMarkers newMarker(markerLocation);
				moveMarkers.push_back(newMarker);
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
	bool hoveringEnemy = Mouse::getInstance().isHoveringEnemy(UnitHandler::getInstance().playerUnits);
	currentHover = hoveringEnemy ? Enemy : Nothing;

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

	// Temp stuff
	placementTemp->setPos(Mouse::getInstance().getPositionWithGrid());
	placementTemp->update();
	placementTemp->placementCollision(playerBuildings);

	// Camera
	float delta = t_deltaTime.asMilliseconds();
	sf::Vector2f cameraOffset = { cameraMovement.x * delta , cameraMovement.y * delta };
	camera.move(cameraOffset);
}

/// <summary>
/// draws the sprites and displays the window
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::White);

	// Grid Render
	flowfield.render(m_window);

	// Move Markers
	for (auto& marker : moveMarkers)
	{
		marker.render(m_window);
	}

	// Selector Render
	selector.render(m_window);

	// Player Building Render
	for (size_t i = 0; i < playerBuildings.size(); i++)
	{
		playerBuildings[i]->draw(m_window);
	}
	// Enemy Building Render
	for (size_t i = 0; i < Buildings::ENEMY_BUILDING_AMOUNT; i++)
	{
		enemyBuildings[i]->draw(m_window);
	}

	// Unit rendering
	UnitHandler::getInstance().render(m_window);

	//Draws the selected building on mouse location
	if (currentMode == Building)
		placementTemp->draw(m_window);

	m_window.setView(camera);
	m_window.display();
}

void Game::initialize()
{
	placementTemp = new Spawner;

	for (int i = 0; i < 10; i++)
	{
		enemyBuildings[i] = new Spawner;
	}

	// Camera
	sf::View cameraView(sf::FloatRect(0, 0, 1200, 600));

	// Giving the flowfield to the unit handler
	UnitHandler::getInstance().setMovementFields(&flowfield, &astar);
}

void Game::placeBuilding()
{
	if (placementTemp->getBlocked() == false)
	{
		placementTemp->setEnemy(false);

		Buildings* building = new Spawner;
		building->setPos(Mouse::getInstance().getPositionWithGrid());
		building->body.setPosition(Mouse::getInstance().getPositionWithGrid());
		building->body.setFillColor(sf::Color::Black);
		building->placed = true;
		building->setEnemy(false);
		playerBuildings.push_back(building);
	}
}

/// <summary>
/// loads the background for the game
/// </summary>
//void Game::loadTextures()
//{
//	//Robot
//	if (!robotTexture.loadFromFile("robot.png"))
//	{
//		std::cout << "problem loading robot" << std::endl;
//	}
//	robot.setTexture(robotTexture);
//	robot.setTextureRect(sf::IntRect{ 0,0,width,height });
//	robot.setOrigin(width/2, height/2);
//	robot.setScale(0.5f, 0.5f);
//	robot.setPosition(startPos);
//	
//	//Flower
//	if (!flowerTexture.loadFromFile("plant.png"))
//	{
//		std::cout << "problem loading flower" << std::endl;
//	}
//	flower.setTexture(flowerTexture);
//	flower.setTextureRect(sf::IntRect{ 0,0,100,229 });
//	flower.setScale(0.2f, 0.2f);
//	flower.setPosition(startPos.x + 570, startPos.y - 10);
//
//	//Background
//	if (!gameBackground.loadFromFile("background.jpg"))
//	{
//		std::cout << "problem loading background" << std::endl;
//	}
//	backgroundSprite.setTexture(gameBackground);
//	backgroundSprite.setTextureRect(sf::IntRect{ 0,0,819,175 });
//}
//void Game::loadMusic()
//{
//	if (!music.openFromFile("music.wav"))
//	{
//		std::cout << "problem loading music" << std::endl;
//	}
//	music.play();
//	music.setVolume(10.0f);
//}
//
