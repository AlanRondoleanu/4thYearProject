#ifndef BUILDINGS_H
#define BUILDINGS_H

#include <SFML/Graphics.hpp>
#include "HealthBars.h"
#include "UnitStats.h"
#include "Targetable.h"

class Units;

class Buildings : public Targetable
{
public:
	Buildings(const sf::Vector2f& position, bool isEnemy);

	virtual void update(float t_deltaTime) = 0;
	void draw(sf::RenderWindow& t_window);
	void resetAttackTimer();
	void loadTileTexture(const std::string& filePath);
	void takeDamage(float amount) override;

	sf::RectangleShape body;
	bool placed{ false };

	// Target
	std::shared_ptr<Targetable> currentTarget;

	// Health Bar
	HealthBars healthBar;

	// Unit Stats
	UnitStats stats;

	// Texture
	sf::Texture texture;
	sf::Sprite sprite;

	//Getters-Setters
	std::string GetUnitType() const override {
		return "None";
	}
	sf::Vector2f getPos() const override { return body.getPosition(); }
	bool getBlocked() { return blocked; }
	sf::Vector2f getSpawnPoint();
	bool isAggressive() { return aggressive; }
	bool getAlive() const override { return alive; }
	bool canAttack();
	float getRadius() const override { return 32; }
	UnitStats getUnitStats() const override;
	int getCellID() const override;

	void setAlive(bool t_alive) { alive = t_alive; }
	void setPos(sf::Vector2f t_position);
	void setBlocked(bool t_blocked) { blocked = t_blocked; }
	void setDebug(bool t_debug) { debug = t_debug; }

private:
	

protected:

	float attackCooldown = 0.0f;
	bool aggressive{ false };
	bool enemy{ false };
	bool blocked{ false };
	bool alive{ true };
	bool debug{ false };
};

#endif
