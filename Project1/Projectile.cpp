#include "Projectile.h"

Projectile::Projectile(sf::Vector2f t_startPos, sf::Vector2f t_dir, float t_speed, float t_dmg, std::shared_ptr<Targetable> targetUnit)
	: position(t_startPos), direction(t_dir), speed(t_speed), damage(t_dmg), active(true), currentTarget(targetUnit)
{

}

void Projectile::Update(float deltaTime)
{
    sf::Vector2f toTarget = currentTarget->getPos() - position;
    float length = std::sqrt(toTarget.x * toTarget.x + toTarget.y * toTarget.y);

    if (length != 0)
    {
        sf::Vector2f normalizedDirection = toTarget / length;

        // Move toward the target
        position += normalizedDirection * speed * deltaTime;
        body.setPosition(position);

        if (IsNearTarget())
        {
            OnImpact();
        }
    }
    else
    {
        OnImpact();
    }
}

void Projectile::Render(sf::RenderWindow& t_window)
{
	t_window.draw(body);
}

bool Projectile::IsNearTarget() const
{
	float dx = position.x - currentTarget->getPos().x;
	float dy = position.y - currentTarget->getPos().y;
	float distSq = dx * dx + dy * dy;

    float targetRadius = currentTarget->getRadius();
    float targetDiameter = targetRadius * targetRadius;

	return distSq <= targetDiameter;
}
