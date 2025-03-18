#include "Turret.h"

Turret::Turret(const sf::Vector2f& position, bool isEnemy)
	: Buildings(position, isEnemy)
{
	// Texture
	loadTileTexture("Assets/Textures/Sci-fi/Turret.png");

	float size = 64;
	body.setSize(sf::Vector2f{ size,size });
	body.setOrigin(size / 2, size / 2);
	body.setFillColor(sf::Color::Black);

	stats.range = 300;
	stats.attackSpeed = 1.0f;

	aggressive = true;

	// Scaling Solution
	sf::FloatRect spriteBounds = sprite.getLocalBounds();
	float spriteWidth = spriteBounds.width;
	float spriteHeight = spriteBounds.height;

	float circleDiameter = 64 * 2;
	float largestDimension = std::max(spriteWidth, spriteHeight);
	float scaleFactor = circleDiameter / largestDimension;

	sprite.setScale(scaleFactor, scaleFactor);
}

void Turret::update(float t_deltaTime)
{
	if (getAlive())
	{
		// Health Set
		healthBar.setHealth(stats.health, stats.max_health);
		healthBar.setPosition(getPos(), body.getSize().y);

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
