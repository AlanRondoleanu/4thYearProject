/// <summary>
/// Author: Alan Rondoleanu
/// Date: 24/10/2024
/// </summary>

#include "Game.h"

int Buildings::PLAYER_BUILDING_AMOUNT = 0;
int Buildings::ENEMY_BUILDING_AMOUNT = 0;
int Units::ENEMY_UNIT_AMOUNT = 0;
int Units::PLAYER_UNIT_AMOUNT = 0;

/// <summary>
/// default constructor
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 1200U, 900U, 32U }, "SFML Game" },
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
	if (sf::Keyboard::Space == t_event.key.code)
	{
		if (playerBuildings[Buildings::PLAYER_BUILDING_AMOUNT]->getBlocked() == false)
		{
			playerBuildings[Buildings::PLAYER_BUILDING_AMOUNT]->place(gridPlacement());
			playerBuildings[Buildings::PLAYER_BUILDING_AMOUNT]->setEnemy(false);
		}
	}
	if (sf::Keyboard::E == t_event.key.code)
	{
		spawnUnits();
	}
}

/// <summary>
/// Processes the keys released on the keyboard
/// </summary>
/// <param name="t_event">the key released</param>
void Game::processKeyReleases(sf::Event t_event)
{
	
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
	mousePosition = getMousePosition(m_window);

	//Update for players
	for (int i = 0; i < Units::PLAYER_UNIT_AMOUNT; i++)
	{
		playerUnits[i]->update();
		playerUnits[i]->findEnemy(enemyUnits, Units::ENEMY_UNIT_AMOUNT);
	}
	//Update for enemies
	for (int i = 0; i < Units::ENEMY_UNIT_AMOUNT; i++)
	{
		enemyUnits[i]->update();
		enemyUnits[i]->findEnemy(playerUnits, Units::PLAYER_UNIT_AMOUNT);
	}

	playerBuildings[Buildings::PLAYER_BUILDING_AMOUNT]->setPos(gridPlacement());
	playerBuildings[Buildings::PLAYER_BUILDING_AMOUNT]->update();
	playerBuildings[Buildings::PLAYER_BUILDING_AMOUNT]->placementCollision(playerBuildings);
}

/// <summary>
/// draws the sprites and displays the window
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::White);

	//Player Building Render
	for (size_t i = 0; i < Buildings::PLAYER_BUILDING_AMOUNT; i++)
	{
		playerBuildings[i]->draw(m_window);
	}
	//Enemy Building Render
	for (size_t i = 0; i < Buildings::ENEMY_BUILDING_AMOUNT; i++)
	{
		enemyBuildings[i]->draw(m_window);
	}
	//Player unit Render
	for (int i = 0; i < Units::PLAYER_UNIT_AMOUNT; i++)
	{
		playerUnits[i]->draw(m_window);
	}
	//Enemy unit Render
	for (int i = 0; i < Units::ENEMY_UNIT_AMOUNT; i++)
	{
		enemyUnits[i]->draw(m_window);
	}
	//Draws the selected building on mouse location
	playerBuildings[Buildings::PLAYER_BUILDING_AMOUNT]->draw(m_window);
	m_window.display();
}

void Game::initialize()
{
	for (int i = 0; i < 10; i++)
	{
		playerBuildings[i] = new Spawner;
	}
	for (int i = 0; i < 10; i++)
	{
		enemyBuildings[i] = new Spawner;
	}

	enemyBuildings[0]->place(sf::Vector2f{ 500, 100 });
	enemyBuildings[0]->setEnemy(true);

	for (int i = 0; i < 100; i++)
	{
		playerUnits[i] = new Soldier;
	}
	for (int i = 0; i < 100; i++)
	{
		enemyUnits[i] = new Soldier;
	}
}

void Game::spawnUnits()
{
	for (int i = 0; i < Buildings::PLAYER_BUILDING_AMOUNT; i++)
	{
		for (int k = 0; k < Units::MAX_UNITS; k++)
		{
			if (playerUnits[k]->getAlive() == false)
			{
				playerUnits[k]->setPos(playerBuildings[i]->getPos());
				playerUnits[k]->setEnemy(false);
				playerUnits[k]->setAlive(true);
				break;
			}
		}
	}
	for (int i = 0; i < Buildings::ENEMY_BUILDING_AMOUNT; i++)
	{
		for (int k = 0; k < Units::MAX_UNITS; k++)
		{
			if (enemyUnits[k]->getAlive() == false)
			{
				enemyUnits[k]->setPos(enemyBuildings[i]->getPos());
				enemyUnits[k]->setEnemy(true);
				enemyUnits[k]->setAlive(true);
				break;
			}
		}
	}
}

sf::Vector2f Game::gridPlacement()
{
	mouseGridPlacement.x = (static_cast<int>(mousePosition.x) / 15) * 15;
	mouseGridPlacement.y = (static_cast<int>(mousePosition.y) / 15) * 15;
	return mouseGridPlacement;
}

sf::Vector2f Game::getMousePosition(sf::RenderWindow& t_window)
{
	sf::Vector2f m_mousePosition;

	m_mousePosition.x = (float)sf::Mouse::getPosition(t_window).x;
	m_mousePosition.y = (float)sf::Mouse::getPosition(t_window).y;

	return m_mousePosition;
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
