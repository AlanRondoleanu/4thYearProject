#include "Cell.h"
#include <iostream>

Cell::Cell()
{
	shape.setOutlineColor(sf::Color::Blue);
	shape.setFillColor(sf::Color(0,0,0,0));
	shape.setOutlineThickness(1);
	//shape.setOrigin(t_size.x / 2, t_size.y / 2);

	sf::Vector2f linePosition = shape.getPosition();
	linePosition.x += width / 2;
	linePosition.y += height / 2;

	lines[0].position = linePosition;
	lines[0].color = sf::Color::Red;

	lines[1].position = linePosition;
	lines[1].color = sf::Color::Red;
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

void Cell::setCost(int t_cost)
{
	cost = t_cost;
}

void Cell::setBestCost(int t_cost)
{
	bestCost = t_cost;
	int temp = bestCost;
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

