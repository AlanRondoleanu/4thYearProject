#include "Soldier.h"

Soldier::Soldier()
{
	initialize();
}


void Soldier::initialize()
{
	body.setSize(sf::Vector2f{ 10,10 });
	body.setFillColor(sf::Color::Blue);

	range = 50;
}


