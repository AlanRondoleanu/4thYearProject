#pragma once
#include <SFML/Graphics.hpp>

class UnitStats
{
public:
	//Stat variables
	float speed{ 1 };
	float aggroRange{ 250 };
	float range{ 0 };
	sf::Vector2f velocity{ 0,-1 };
};

