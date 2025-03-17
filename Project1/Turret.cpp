#include "Turret.h"

Turret::Turret(const sf::Vector2f& position, bool isEnemy)
	: Buildings(position, isEnemy)
{
	// Texture
	loadTileTexture("Assets/Textures/Sci-fi/Structure/scifiStructure_14.png");

	float size = texture.getSize().x / 2;
	body.setSize(sf::Vector2f{ size,size });
	body.setOrigin(size / 2, size / 2);
	body.setFillColor(sf::Color::Black);

	stats.range = 200;
	stats.attackSpeed = 1.0f;

	aggressive = true;
}

void Turret::update(float t_deltaTime)
{
	if (isAlive())
	{
		// Health Set
		healthBar.setHealth(stats.health, stats.max_health);
		healthBar.setPosition(getPos(), body.getSize().y / 2);

		// Update attack timer
		if (attackCooldown > 0.0f) 
		{
			attackCooldown -= t_deltaTime;
		}

		// Kill unit
		if (stats.health <= 0)
		{
			setAlive(false);
		}
	}
}

std::string Turret::GetUnitType() const
{
	return "Turret";
}
