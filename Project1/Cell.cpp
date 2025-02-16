#include "Cell.h"
#include <iostream>

Cell::Cell()
{
	shape.setOutlineColor(sf::Color::Blue);
	shape.setOutlineThickness(1);
	//shape.setOrigin(t_size.x / 2, t_size.y / 2);
}

void Cell::initialize()
{
	sf::Vector2f linePosition = shape.getPosition();
	linePosition.x += width / 2;
	linePosition.y += height / 2;

	lines[0].position = linePosition;
	lines[0].color = sf::Color::Red;

	lines[1].position = linePosition;
	lines[1].color = sf::Color::Red;

	// Font
	text.setPosition(shape.getPosition());
	text.setCharacterSize(10U);
	text.setFillColor(sf::Color::Black);
	text.setString("0");
}

void Cell::setDirection(sf::Vector2f t_direction)
{
	direction = t_direction;

	sf::Vector2f linePosition = shape.getPosition();
	linePosition.x += width / 2;
	linePosition.y += height / 2;
	lines[0].position = linePosition;

	linePosition.x += (width / 2) * (direction.x);
	linePosition.y += (height / 2) * (direction.y);;
	lines[1].position = linePosition;
}

void Cell::setFont(sf::Font& t_font)
{
	text.setFont(t_font);
}

void Cell::setCost(int t_cost)
{
	cost = t_cost;
}

void Cell::setBestCost(int t_cost)
{
	bestCost = t_cost;
	int temp = bestCost; // Removes decimal places
	if (bestCost != 99999) {
		text.setString(std::to_string(temp));
	}
}

void Cell::setSize(sf::Vector2f t_size)
{
	shape.setSize(t_size);
	width = t_size.x;
	height = t_size.y;
}


void Cell::render(sf::RenderWindow& t_window)
{
	t_window.draw(shape);
	t_window.draw(lines, 2, sf::Lines);
}

