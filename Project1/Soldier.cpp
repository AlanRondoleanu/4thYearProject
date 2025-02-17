#include "Soldier.h"

Soldier::Soldier(sf::Vector2f t_startPosition, FlowfieldMovement t_flowfieldMovement, AStarMovement t_astarMovement)
	: Units(t_startPosition, t_flowfieldMovement, t_astarMovement)
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


