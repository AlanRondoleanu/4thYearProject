#ifndef FLOWFIELD_H
#define FLOWFIELD_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>
#include "Cell.h"
#include "Buildings.h"
#include <iostream>

class FlowField
{
public:
	FlowField();
	static int const CELL_WIDTH{ 50 };
	static int const CELL_HEIGHT{ 50 };
	static int const CELL_AMOUNT{ 50 };

	Cell* destination;
	std::vector<std::vector<Cell>> Grid;

	// Functions
	void resetField(std::vector<Buildings*> t_buildings);
	void createIntegrationField();
	void createFlowField();
	void render(sf::RenderWindow& t_window);

	// Gets-Sets
	void setNeighbors();
	void setDestination(Cell* t_cell);

private:
	void loadFont();
	sf::Vector2f directionBetweenTwoPoints(sf::Vector2f t_1, sf::Vector2f t_2);

	sf::Font font;

};

#endif