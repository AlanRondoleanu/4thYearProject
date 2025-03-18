#include "Placement_Building.h"

Placement_Building::Placement_Building(const sf::Vector2f& position, bool isEnemy)
	: Buildings(position, isEnemy)
{
	float size = 100;
	body.setSize(sf::Vector2f{ size,size });
	body.setOrigin(size / 2, size / 2);
	body.setFillColor(sf::Color::Green);
}

void Placement_Building::update(float t_deltaTime)
{
	
}
