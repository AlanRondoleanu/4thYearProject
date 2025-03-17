#include "Soldier_Building.h"

Soldier_Building::Soldier_Building(const sf::Vector2f& position, bool isEnemy)
	: Buildings(position, isEnemy)
{
	float size = 64;
	body.setSize(sf::Vector2f{ size,size });
	body.setOrigin(size / 2, size / 2);
	body.setFillColor(sf::Color::Black);

	// Texture
	loadTileTexture("Assets/Textures/Sci-fi/Soldier_Building.png");

	// 1. Get the sprite's original size
	sf::FloatRect spriteBounds = sprite.getLocalBounds();
	float spriteWidth = spriteBounds.width;
	float spriteHeight = spriteBounds.height;
	float targetWidth = 128;

	float scaleFactor = targetWidth / spriteWidth;
	sprite.setScale(scaleFactor, scaleFactor);
}

void Soldier_Building::update(float t_deltaTime)
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

std::string Soldier_Building::GetUnitType() const
{
	return "Soldier_Building";
}
