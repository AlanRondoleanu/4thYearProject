#ifndef UNITS_H
#define UNITS_H
#include <SFML/Graphics.hpp>
#include "Trigonometry.hpp"
#include "VectorAlgebra2D.h"
#include "VectorAlgebra2D.inl"
#include "UnitStats.h"
#include "FlowField.h"
#include "FlowfieldMovement.h"
#include "AstarMovement.h"
#include "HealthBars.h"

enum class UnitState {
	Moving,
	Following,
	AttackMove,
	AttackFollow,
	Attacking,
	Idle
};

class Units
{
public:
	Units(sf::Vector2f t_startPosition, FlowfieldMovement t_flowfieldMovement, AStarMovement t_astarMovement);
	~Units();

	static const int MAX_UNITS{ 100 };

	void update(float t_deltaTime);
	void draw(sf::RenderWindow& t_window);
	void setFlowField(FlowField t_field, UnitState t_state);
	void select();
	void deselect();
	bool isInsideSelection(const sf::FloatRect& selection) const;
	void push(sf::Vector2f t_direction);
	bool canAttack();
	void resetAttackTimer();
	std::string stateToString();

	// Health Bars
	HealthBars healthBar;

	// Flowfield Movement
	sf::Vector2f flowfieldDirection;
	sf::Vector2f velocity;

	// Movement Strategy
	FlowfieldMovement flowfieldMovement;
	AStarMovement astarMovement;

	// Statistic values for unit
	UnitStats stats;
	UnitState state = { UnitState::Idle };
	
	int cellID;
	sf::CircleShape body;
	std::shared_ptr<Units> currentTarget;

	//Booleans
	bool enemy{ false };
	bool alive{ true };
	bool foundTarget{ false };
	bool blocked{ false };
	bool destinationReached{ true };

	//Getters-Setters
	sf::Vector2f getPos() { return pos; }
	float getRadius() { return body.getRadius(); }
	float getDiameter() { return body.getRadius() * 2; }
	bool getAlive() { return alive; }
	virtual std::string GetUnitType() const = 0;

	void setPos(sf::Vector2f t_position) { pos = t_position, body.setPosition(pos); }
	void setAlive(bool t_status) { alive = t_status; }
	void setCellID();

private:

	sf::Vector2f pos{ -100, -100 };
	float attackCooldown = 0.0f;
};
#endif
