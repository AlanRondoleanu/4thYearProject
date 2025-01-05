#include "Units.h"

Units::Units() 
{
	//movement.stats = &stats;
}
Units::~Units() {}

void Units::update()
{
	if (getAlive() == true)
	{
		pos = body.getPosition();
	}
}

void Units::moveUnit(sf::Vector2f t_direction)
{
	if (getAlive() == true)
	{
		body.move(t_direction * stats.speed);
	}
}

void Units::draw(sf::RenderWindow& t_window)
{
	t_window.draw(body);
}

void Units::select()
{
	body.setFillColor(sf::Color::Green);
}

void Units::deselect()
{
	body.setFillColor(sf::Color::Blue);
}

bool Units::isInsideSelection(const sf::FloatRect& selection) const
{
	return selection.intersects(body.getGlobalBounds());
}
