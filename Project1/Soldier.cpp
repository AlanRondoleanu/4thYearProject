#include "Soldier.h"

Soldier::Soldier()
{
	initialize();
}


void Soldier::initialize()
{
	float size = 50;
	body.setSize(sf::Vector2f{ size, size });
	body.setOrigin({ size / 2, size / 2 });
	body.setFillColor(sf::Color::Blue);

	range = 50;
}


