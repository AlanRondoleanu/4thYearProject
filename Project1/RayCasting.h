#ifndef RAYCASTING_H
#define RAYCASTING_H

#include <SFML/Graphics.hpp>
#include <algorithm> 
#include <vector> 
#include <iostream> 
#include "PartitionGrid.h"
#include "Units.h"

class RayCasting
{
public:
    bool isBlocked(Units& movingUnit, PartitionGrid& partition);

private:
    bool rayIntersectsCircle(const sf::Vector2f& A, const sf::Vector2f& B, const sf::Vector2f& C, float totalRadius);
    sf::Vector2f normalize(const sf::Vector2f& vector);
    float dot(const sf::Vector2f& a, const sf::Vector2f& b);
    float lengthSquared(const sf::Vector2f& v);
};

#endif