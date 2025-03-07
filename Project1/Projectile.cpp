#include "Projectile.h"

Projectile::Projectile(sf::Vector2f t_startPos, sf::Vector2f t_dir, float t_speed, float t_dmg, std::shared_ptr<Units> targetUnit)
	: position(t_startPos), direction(t_dir), speed(t_speed), damage(t_dmg), active(true), currentTarget(targetUnit)
{

}

void Projectile::Update(float deltaTime)
{
	position += direction * speed * deltaTime;
	body.setPosition(position);

	if (IsNearTarget())
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
	return distSq <= (currentTarget->getRadius() * currentTarget->getRadius());
}
