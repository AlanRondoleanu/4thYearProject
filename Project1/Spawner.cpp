#include "Spawner.h"

Spawner::Spawner()
{
	initialize();
}

void Spawner::update()
{

}

void Spawner::initialize()
{
	float size = 225;
	body.setSize(sf::Vector2f{ size,size });
	body.setOrigin(size / 2, size / 2);
	body.setFillColor(sf::Color::Green);
}


