#include "Spawner.h"

Spawner::Spawner()
{
	initialize();
}

void Spawner::update()
{
	if (placed == false)
	{
		body.setPosition(getPos());
	}
}

void Spawner::initialize()
{
	body.setSize(sf::Vector2f{ 50,50 });
	body.setOrigin(25, 25);
	body.setFillColor(sf::Color::Green);
}


