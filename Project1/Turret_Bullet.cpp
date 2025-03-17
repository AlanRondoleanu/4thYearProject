#include "Turret_Bullet.h"

Turret_Bullet::Turret_Bullet(sf::Vector2f startPos, sf::Vector2f dir, std::shared_ptr<Units> targetUnit)
	: Projectile(startPos, dir, 300.0f, 10.0f, targetUnit)
{
    body.setSize(sf::Vector2f(15.0f, 15.0f));
    body.setFillColor(sf::Color::Black);

    // Rotate the arrow based on its movement direction
    float angle = std::atan2(direction.y, direction.x) * 180.0f / 3.14159f;
    body.setRotation(angle);
}

void Turret_Bullet::OnImpact()
{
    currentTarget->stats.health -= 50;
    Deactivate();
}
