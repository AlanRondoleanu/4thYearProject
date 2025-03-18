#include "Turret_Bullet.h"

Turret_Bullet::Turret_Bullet(sf::Vector2f startPos, sf::Vector2f dir, std::shared_ptr<Targetable> targetUnit)
	: Projectile(startPos, dir, 300.0f, 10.0f, targetUnit)
{
    sf::Vector2f size{ 20.0f, 10.0f };
    body.setSize(size);
    body.setOrigin(size.x / 2, size.y / 2);
    body.setFillColor(sf::Color::Blue);

    // Rotate the arrow based on its movement direction
    float angle = std::atan2(direction.y, direction.x) * 180.0f / 3.14159f;
    body.setRotation(angle);
}

void Turret_Bullet::OnImpact()
{
    currentTarget->takeDamage(50);
    Deactivate();
}
