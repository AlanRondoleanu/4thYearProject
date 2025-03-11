#include "Soldier_Bullet.h"

Soldier_Bullet::Soldier_Bullet(sf::Vector2f startPos, sf::Vector2f dir, std::shared_ptr<Units> targetUnit)
    : Projectile(startPos, dir, 300.0f, 10.0f, targetUnit)
{
    body.setSize(sf::Vector2f(10.0f, 5.0f));
    body.setFillColor(sf::Color::Black);

    // Rotate the arrow based on its movement direction
    float angle = std::atan2(direction.y, direction.x) * 180.0f / 3.14159f;
    body.setRotation(angle);
}

void Soldier_Bullet::OnImpact()
{
    currentTarget->stats.health -= 10;
    Deactivate();
}
