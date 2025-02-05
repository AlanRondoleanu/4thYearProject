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
    void render(sf::RenderWindow& t_window);

    std::vector<Unit> getBlockingUnits(Unit movingUnit, std::vector<Unit> otherUnits);
    sf::Vector2f avoidObstacle(Unit movingUnit, std::vector<Unit> obstacleUnits, sf::Vector2f flowFieldDirection);

private:
    sf::Vertex lines[2];

    sf::Vector2f calculateAvoidanceDirection(Unit& movingUnit, const std::vector<Unit>& blockingUnits);
    bool rayIntersectsCircle(const sf::Vector2f& A, const sf::Vector2f& B, const sf::Vector2f& C, float totalRadius);
    sf::Vector2f normalize(const sf::Vector2f& vector);
    float dot(const sf::Vector2f& a, const sf::Vector2f& b);
    float lengthSquared(const sf::Vector2f& v);
};

#endif