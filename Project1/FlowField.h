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
	static int const GRID_HEIGHT{ 50 };
	static int const GRID_WIDTH{ 50 };

	Cell* destination;
	sf::Vector2f destinationPosition;
	std::vector<std::vector<Cell>> Grid;

	// Functions
	void resetField();
	void createIntegrationField();
	void createFlowField();
	void render(sf::RenderWindow& t_window);

	// Gets-Sets
	void setNeighbors();
	void setDestinationCell(Cell* t_cell);
	void setDestinationPosition(sf::Vector2f t_destination);

private:
	void loadFont();
	sf::Vector2f directionBetweenTwoPoints(sf::Vector2f t_1, sf::Vector2f t_2);

	sf::Font font;

};

#endif