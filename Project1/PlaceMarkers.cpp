#include "PlaceMarkers.h"

PlaceMarkers::PlaceMarkers(sf::Vector2f t_postion)
{
	color = sf::Color::Green;

	shape.setRadius(10);
	shape.setOrigin(10, 10);
	shape.setOutlineColor(color);
	shape.setOutlineThickness(3);
	shape.setPosition(t_postion);
	shape.setFillColor(sf::Color(0, 0, 0, 0));	
}

void PlaceMarkers::render(sf::RenderWindow& t_window)
{
	t_window.draw(shape);
}

void PlaceMarkers::update()
{
	if (color.a > 0)
		color.a -= 3;

	shape.setOutlineColor(color);
}

bool PlaceMarkers::isExpired() const
{
	return color.a <= 0;
}
