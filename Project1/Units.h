#ifndef UNITS_H
#define UNITS_H
#include <SFML/Graphics.hpp>
#include "Trigonometry.hpp"
#include "VectorAlgebra2D.h"
#include "VectorAlgebra2D.inl"
#include "UnitStats.h"
#include "FlowField.h"

class Units
{
public:
	Units();
	~Units();

	static const int MAX_UNITS{ 100 };

	void update();
	void draw(sf::RenderWindow& t_window);
	void setFlowField(FlowField t_field) { flowfield = t_field, moving = true; }
	void select();
	void deselect();
	bool isInsideSelection(const sf::FloatRect& selection) const;
	void push(sf::Vector2f t_direction);

	// Movement Manager
	FlowField flowfield;

	// Statistic values for unit
	UnitStats stats;

	int cellID;
	sf::CircleShape body;
	sf::Vector2f velocity;
	Units* currentTarget = nullptr;

	//Booleans
	bool enemy{ false };
	bool alive{ true };
	bool foundTarget{ false };
	bool moving{ false };


	//Getters-Setters
	sf::Vector2f getPos() { return pos; }
	bool getAlive() { return alive; }

	void setPos(sf::Vector2f t_position) { pos = t_position, body.setPosition(pos); }
	void setAlive(bool t_status) { alive = true; }
	void setCellID(FlowField* t_flowfield);

private:
	virtual void initialize() = 0;

	sf::Vector2f pos{ -100, -100 };
};
#endif
