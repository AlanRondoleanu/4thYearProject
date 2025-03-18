#pragma once
#include <SFML/Graphics.hpp>

class UnitStats
{
public:
	//Stat variables
	float health{ 100 };
	float max_health{ 100 };
	float speed{ 1 };
	float aggroRange{ 250 };
	float range{ 100 };
	float attackSpeed{ 1 };
	float damage{ 1 };
	float projectile_speed{ 1 };
	sf::Vector2f velocity{ 0,-1 };
};

