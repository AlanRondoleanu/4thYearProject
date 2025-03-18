#ifndef UNITS_H
#define UNITS_H
#include <SFML/Graphics.hpp>
#include "Trigonometry.hpp"
#include "VectorAlgebra2D.h"
#include "VectorAlgebra2D.inl"
#include "Targetable.h"
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

enum class UnitFacing {
	Front,
	Back
};

class Units : public Targetable
{
public:
	Units(sf::Vector2f t_startPosition, FlowfieldMovement t_flowfieldMovement, AStarMovement t_astarMovement);
	~Units();

	static const int MAX_UNITS{ 100 };

	void update(float t_deltaTime);
	void loadTexture(const std::string& filePath);
	void swapTexture(int t_value);
	void draw(sf::RenderWindow& t_window);
	void setFlowField(FlowField t_field, UnitState t_state);
	void select();
	void deselect();
	bool isInsideSelection(const sf::FloatRect& selection) const;
	void push(sf::Vector2f t_direction);
	bool canAttack();
	void resetAttackTimer();
	std::string stateToString();
	void takeDamage(float amount) override;

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
	std::shared_ptr<Targetable> currentTarget;

	// Texture
	std::vector<sf::Texture> textures;
	sf::Sprite sprite;
	UnitFacing currentFacing{UnitFacing::Front};

	//Booleans
	bool enemy{ false };
	bool alive{ true };
	bool foundTarget{ false };
	bool blocked{ false };
	bool destinationReached{ true };

	//Getters-Setters
	std::string GetUnitType() const override {
		return "None";
	}
	sf::Vector2f getPos() const override { return pos; }
	float getRadius() const override{ return body.getRadius(); }
	float getDiameter() { return body.getRadius() * 2; }
	bool getAlive() const override { return alive; }
	std::shared_ptr<Targetable> getTarget();
	UnitState getState();
	UnitStats getUnitStats() const override;
	int getCellID() const override;

	void setPos(sf::Vector2f t_position);
	void setAlive(bool t_status) { alive = t_status; }
	void setCellID();
	void setState(UnitState t_state);
	void setTarget(std::shared_ptr<Targetable> t_target);


private:

	sf::Vector2f pos{ -100, -100 };
	float attackCooldown = 0.0f;

protected:
	int i = 9;
};
#endif
