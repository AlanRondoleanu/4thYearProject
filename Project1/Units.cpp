#include "Units.h"

Units::Units() {}
Units::~Units() {}

void Units::update()
{
	if (getAlive() == true)
	{
		body.move(velocity * speed);
		pos = body.getPosition();
	}
}

void Units::draw(sf::RenderWindow& t_window)
{
	t_window.draw(body);
}

void Units::findEnemy(Units* t_enemies[], static int t_amount)
{
	if (foundTarget == false && alive == true)
	{
		for (int i = 0; i < t_amount; i++)
		{
			sf::Vector2f distanceVector = getPos() - t_enemies[i]->getPos();
			float contactRadius = std::sqrtf(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y);

			if (distanceVector != sf::Vector2f(0.0f, 0.0f) &&
				contactRadius < aggroRange)
			{
				body.setFillColor(sf::Color::Red);
				
				currentTarget = t_enemies[i];
				foundTarget = true;
			}
		}
	}
	if (foundTarget == true && alive == true)
	{
		chaseEnemy();
	}
}

void Units::chaseEnemy()
{
	sf::Vector2f distanceVector = currentTarget->getPos() - getPos();
	float contactRadius = std::sqrtf(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y);
	velocity = thor::unitVector(distanceVector);

	if (distanceVector != sf::Vector2f(0.0f, 0.0f) &&
		contactRadius < range)
	{
		velocity = { 0,0 };
	}
}

void Units::setEnemy(bool t_enemy)
{
	enemy = t_enemy;
	if (t_enemy == true)
	{
		velocity = { 0,1 };
		ENEMY_UNIT_AMOUNT++;
	}
	else if (t_enemy == false)
	{
		velocity = { 0,-1 };
		PLAYER_UNIT_AMOUNT++;
	}
}