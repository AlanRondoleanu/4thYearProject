#ifndef RAYCASTING_H
#define RAYCASTING_H

#include <SFML/Graphics.hpp>
#include <algorithm> 
#include <vector> 
#include <iostream> 

class Unit 
{
public:
    sf::Vector2f position;
    float radius;
    sf::Vector2f velocity;
};

class RayCasting
{
public:
    std::vector<Unit> getBlockingUnits(Unit movingUnit, std::vector<Unit> otherUnits);
    sf::Vector2f calculateAvoidanceDirection(const Unit movingUnit, const std::vector<Unit> blockingUnits);

private:
    bool rayIntersectsCircle(const sf::Vector2f& A, const sf::Vector2f& B, const sf::Vector2f& C, float totalRadius);
    sf::Vector2f normalize(const sf::Vector2f& vector);
    float dot(const sf::Vector2f& a, const sf::Vector2f& b);
    float lengthSquared(const sf::Vector2f& v);
};

#endif