#pragma once
#include <SFML/Graphics.hpp>

class PlaceMarkers
{
public:
	PlaceMarkers(sf::Vector2f t_postion);
	void render(sf::RenderWindow& t_window);
	void update();
	bool isExpired() const;
private:
	sf::CircleShape shape;
	sf::Color color;
};

