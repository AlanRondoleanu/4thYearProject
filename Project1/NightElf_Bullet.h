#ifndef NIGHTELF_BULLET_H
#define NIGHTELF_BULLET_H

#include <SFML/Graphics.hpp>
#include "Projectile.h"

class NightElf_Bullet : public Projectile
{
public:
    NightElf_Bullet(sf::Vector2f startPos, sf::Vector2f dir, std::shared_ptr<Targetable> targetUnit, float t_damage, float t_projectile_speed);


    void OnImpact() override;
};

#endif
