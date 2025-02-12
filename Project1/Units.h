#ifndef UNITS_H
#define UNITS_H
#include <SFML/Graphics.hpp>
#include "Trigonometry.hpp"
#include "VectorAlgebra2D.h"
#include "VectorAlgebra2D.inl"
#include "UnitStats.h"
#include "FlowField.h"
#include "MovementStrategy.h"
#include "FlowfieldMovement.h"

enum class UnitState {
	Moving,
	Following,
	AttackMove,
	AttackFollow,
	Idle
};

class Units
{
public:
	Units(sf::Vector2f t_startPosition, FlowfieldMovement t_flowfieldMovement);
	~Units();

	static const int MAX_UNITS{ 100 };

	void update();
	void draw(sf::RenderWindow& t_window);
	void setFlowField(FlowField t_field, UnitState t_state) { flowfield = t_field, state = t_state; }
	void select();
	void deselect();
	bool isInsideSelection(const sf::FloatRect& selection) const;
	void push(sf::Vector2f t_direction);

	// Movement Manager
	FlowField flowfield;
	sf::Vector2f flowfieldDirection;
	sf::Vector2f velocity;

	// Movement Strategy
	MovementStrategy* movementStrategy;
	FlowfieldMovement& flowfieldMovement;

	void setMovementStrategy();

	// Statistic values for unit
	UnitStats stats;
	UnitState state = { UnitState::Idle };

	int cellID;
	sf::CircleShape body;
	Units* currentTarget = nullptr;

	//Booleans
	bool enemy{ false };
	bool alive{ true };
	bool foundTarget{ false };

	//Getters-Setters
	sf::Vector2f getPos() { return pos; }
	float getRadius() { return body.getRadius(); }
	bool getAlive() { return alive; }

	void setPos(sf::Vector2f t_position) { pos = t_position, body.setPosition(pos); }
	void setAlive(bool t_status) { alive = true; }
	void setCellID();

private:
	virtual void initialize() = 0;

	sf::Vector2f pos{ -100, -100 };
};
#endif
