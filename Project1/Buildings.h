#ifndef BUILDINGS_H
#define BUILDINGS_H

#include <SFML/Graphics.hpp>
#include "HealthBars.h"
#include "UnitStats.h"
//#include "Units.h"

class Units;

class Buildings
{
public:
	Buildings(const sf::Vector2f& position, bool isEnemy);

	virtual void update(float t_deltaTime) = 0;
	void draw(sf::RenderWindow& t_window);
	void resetAttackTimer();
	void loadTileTexture(const std::string& filePath);

	sf::RectangleShape body;
	bool placed{ false };

	// Target
	std::shared_ptr<Units> currentTarget;

	// Health Bar
	HealthBars healthBar;

	// Unit Stats
	UnitStats stats;

	// Texture
	sf::Texture texture;
	sf::Sprite sprite;

	//Getters-Setters
	sf::Vector2f getPos() { return body.getPosition(); }
	bool getBlocked() { return blocked; }
	sf::Vector2f getSpawnPoint();
	bool isAggressive() { return aggressive; }
	bool isAlive() { return alive; }
	bool canAttack();
	virtual std::string GetUnitType() const = 0;

	void setAlive(bool t_alive) { alive = t_alive; }
	void setPos(sf::Vector2f t_position);
	void setBlocked(bool t_blocked) { blocked = t_blocked; }

private:
	

protected:

	float attackCooldown = 0.0f;
	bool aggressive{ false };
	bool enemy{ false };
	bool blocked{ false };
	bool alive{ true };
};
#endif
