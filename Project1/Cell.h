#ifndef CELL_H
#define CELL_H
#include <SFML/Graphics.hpp>
#include <algorithm>

class Cell
{
public:
	Cell();

	// Functions
	void render(sf::RenderWindow& t_window);
	void debugColor();
	void initialize();

	// Const Vars
	static int const MAX_COST{ 99999 };

	// Vars
	bool passable{ true };
	bool goal{ false };
	float cost{ 1 };
	float bestCost{ MAX_COST };
	sf::Vector2f direction{ 0,0 };
	std::vector<Cell*> neighbouringCells;
	std::vector<Cell*> diagonalCells;

	// Getters and Setters
	sf::Vector2f getPosition() { return { shape.getPosition().x + 25, shape.getPosition().y + 25 }; }
	void setPosition(sf::Vector2f t_position) { shape.setPosition(t_position); }
	void setDirection(sf::Vector2f t_direction);
	float getWidth() { return width; }
	float getHeight() { return height; }
	int getID() { return cellID; }
	std::vector<Cell*> getNeighbors() { return neighbouringCells; }
	void setFont(sf::Font& t_font);
	void setCost(int t_cost);
	void setBestCost(int t_cost);
	void setID(int t_ID) { cellID = t_ID; }
	void setSize(sf::Vector2f t_size);
	sf::Vector2f getDirection(){ return direction; }

	sf::RectangleShape shape;
	sf::Vertex lines[2];

	//std::vector<Units*> unitsInCell;

private:

	float distanceFromGoal{ 0 };
	float width{ 0 };
	float height{ 0 };
	int cellID{ 0 }; 
	sf::Text text;
};

#endif