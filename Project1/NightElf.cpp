#include "NightElf.h"

NightElf::NightElf(sf::Vector2f t_startPosition, FlowfieldMovement t_flowfieldMovement, AStarMovement t_astarMovement)
	: Units(t_startPosition, t_flowfieldMovement, t_astarMovement)
{
	float size = 25;
	body.setRadius(size);
	body.setOrigin({ size, size });
	body.setFillColor(sf::Color::Blue);

	stats.range = 300;
	stats.attackSpeed = 0.5f;
	stats.damage = 25;
	stats.projectile_speed = 300;

	// Texture
	loadTexture("Assets/Textures/Fantasy/NightElf_Back.png");
	loadTexture("Assets/Textures/Fantasy/NightElf_Front.png");
	swapTexture(0);

	// Scaling Solution
	sf::FloatRect spriteBounds = sprite.getLocalBounds();
	float spriteWidth = spriteBounds.width;
	float spriteHeight = spriteBounds.height;

	float circleDiameter = size * 2.f;
	float largestDimension = std::max(spriteWidth, spriteHeight);
	float scaleFactor = circleDiameter / largestDimension;

	sprite.setScale(scaleFactor, scaleFactor);
}
