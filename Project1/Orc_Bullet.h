#ifndef ORC_BULLET_H
#define ORC_BULLET_H

#include <SFML/Graphics.hpp>
#include "Projectile.h"

class Orc_Bullet : public Projectile
{
public:
    Orc_Bullet(sf::Vector2f startPos, sf::Vector2f dir, std::shared_ptr<Targetable> targetUnit, float t_damage, float t_projectile_speed);


    void OnImpact() override;
};

#endif
