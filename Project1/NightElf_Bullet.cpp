#include "NightElf_Bullet.h"

NightElf_Bullet::NightElf_Bullet(sf::Vector2f startPos, sf::Vector2f dir, std::shared_ptr<Targetable> targetUnit, float t_damage, float t_projectile_speed)
    : Projectile(startPos, dir, t_projectile_speed, t_damage, targetUnit)
{
    sf::Vector2f size{ 30.0f, 3.0f };
    body.setSize(size);
    body.setOrigin(size.x / 2, size.y / 2);
    body.setFillColor(sf::Color::Black);

    // Rotate the arrow based on its movement direction
    float angle = std::atan2(direction.y, direction.x) * 180.0f / 3.14159f;
    body.setRotation(angle);
}

void NightElf_Bullet::OnImpact()
{
    currentTarget->takeDamage(damage);
    Deactivate();
}
