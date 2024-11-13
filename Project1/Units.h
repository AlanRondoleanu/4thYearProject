#ifndef UNITS_H
#define UNITS_H
#include <SFML/Graphics.hpp>
#include "Trigonometry.hpp"
#include "VectorAlgebra2D.h"
#include "VectorAlgebra2D.inl"
#include "FlowField.h"


class Units
{
public:
	Units();
	~Units();

	static int ENEMY_UNIT_AMOUNT;
	static int PLAYER_UNIT_AMOUNT;
	static const int MAX_UNITS{ 100 };

	void update();
	void draw(sf::RenderWindow& t_window);
	void findEnemy(Units* t_enemies[], static int t_amount);
	void chaseEnemy();
	void setFlowField(FlowField* t_field) { flowfield = t_field; }
	
	sf::RectangleShape body;
	Units* currentTarget = nullptr;
	FlowField* flowfield;

	//Booleans
	bool enemy{ false };
	bool alive{ false };
	bool foundTarget{ false };

	//Stat variables
	float speed{ 1 };
	float aggroRange{ 250 };
	float range{ 0 };
	sf::Vector2f velocity{ 0,-1 };
	

	//Getters-Setters
	sf::Vector2f getPos() { return pos; }
	bool getAlive() { return alive; }

	void setPos(sf::Vector2f t_position) { pos = t_position, body.setPosition(pos); }
	void setEnemy(bool t_enemy);
	void setAlive(bool t_status) { alive = true; }

private:
	virtual void initialize() = 0;

	sf::Vector2f pos{ -100, -100 };
};
#endif
