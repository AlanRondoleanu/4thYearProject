#include "Buildings.h"

Buildings::Buildings()
{
	
}

Buildings::~Buildings(){}

void Buildings::draw(sf::RenderWindow& t_window)
{
	t_window.draw(body);
}

void Buildings::setPos(sf::Vector2f t_position)
{
	body.setPosition(t_position);
	spawnPoint = body.getPosition();
	spawnPoint.y -= 225 / 2;
}

void Buildings::setEnemy(bool t_enemy)
{ 
	enemy = t_enemy; 
	if (t_enemy == true)
	{
		Buildings::ENEMY_BUILDING_AMOUNT++;
	}
	else if (t_enemy == false)
	{
		Buildings::PLAYER_BUILDING_AMOUNT++;
	}
}


void Buildings::placementCollision(std::vector<Buildings*> t_buildings)
{
	body.setFillColor(sf::Color::Green);
	blocked = false;
	for (int i = 0; i < t_buildings.size(); i++)
	{
		if (body.getGlobalBounds().intersects(t_buildings[i]->body.getGlobalBounds()))
		{
			body.setFillColor(sf::Color::Red);
			blocked = true;
		}
	}
}
