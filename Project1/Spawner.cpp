#include "Spawner.h"

Spawner::Spawner(const sf::Vector2f& position, bool isEnemy)
	: Buildings(position, isEnemy)
{
	// Texture
	loadTileTexture("Assets/Textures/Sci-fi/Structure/scifiStructure_01.png");

	float size = texture.getSize().x;
	body.setSize(sf::Vector2f{ size,size });
	body.setOrigin(size / 2, size / 2);
	body.setFillColor(sf::Color::Black);
}

void Spawner::update(float t_deltaTime)
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

std::string Spawner::GetUnitType() const
{
	return "Spawner";
}


