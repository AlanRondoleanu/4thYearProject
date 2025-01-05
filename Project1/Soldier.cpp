#include "Soldier.h"

Soldier::Soldier()
{
	initialize();
}


void Soldier::initialize()
{
	float size = 25;
	body.setRadius(size);
	body.setOrigin({ size, size});
	body.setFillColor(sf::Color::Blue);

	stats.range = 50;
}


