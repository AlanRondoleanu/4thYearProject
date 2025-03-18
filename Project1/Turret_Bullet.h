#ifndef TURRET_BULLET_H
#define TURRET_BULLET_H

#include <SFML/Graphics.hpp>
#include "Projectile.h"

class Turret_Bullet : public Projectile
{
public:
    Turret_Bullet(sf::Vector2f startPos, sf::Vector2f dir, std::shared_ptr<Targetable> targetUnit);


    void OnImpact() override;
};

#endif