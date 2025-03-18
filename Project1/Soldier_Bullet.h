#ifndef SOLDIER_BULLET_H
#define SOLDIER_BULLET_H

#include <SFML/Graphics.hpp>
#include "Projectile.h"

class Soldier_Bullet : public Projectile
{
public:
    Soldier_Bullet(sf::Vector2f startPos, sf::Vector2f dir, std::shared_ptr<Targetable> targetUnit, float t_damage, float t_projectile_speed);
        

    void OnImpact() override;
};

#endif
