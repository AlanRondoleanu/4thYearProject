#include "RayCasting.h"


void RayCasting::render(sf::RenderWindow& t_window)
{
    t_window.draw(lines, 2, sf::Lines);
}

std::vector<Unit> RayCasting::getBlockingUnits(Unit movingUnit, std::vector<Unit> otherUnits)
{
    std::vector<Unit> blockingUnits;  // List to store blocking units

    sf::Vector2f A = movingUnit.position;
    sf::Vector2f B = A + movingUnit.velocity * 10.f;
    float r1 = movingUnit.radius;
    
    //Debug
    lines[0].position = A;
    lines[0].color = sf::Color::Blue;

    lines[1].position = B;
    lines[1].color = sf::Color::Blue;

    // Check for collision with any other unit
    for (const Unit& otherUnit : otherUnits)
    {
        sf::Vector2f C = otherUnit.position;
        float r2 = otherUnit.radius;

        if (rayIntersectsCircle(A, B, C, r1 + r2))
        {
            // If a collision is detected, add the unit to the blockingUnits list
            blockingUnits.push_back(otherUnit);
        }
    }

    return blockingUnits;
}

sf::Vector2f RayCasting::avoidObstacle(Unit movingUnit, std::vector<Unit> obstacleUnits, sf::Vector2f flowFieldDirection)
{
    std::vector<Unit> blockingUnits = getBlockingUnits(movingUnit, obstacleUnits);

    if (!blockingUnits.empty())
    {
        // Calculate an avoidance direction based on blocking units
        sf::Vector2f avoidanceDirection = calculateAvoidanceDirection(movingUnit, blockingUnits);

        return avoidanceDirection;
    }
    else {
        return movingUnit.velocity;
    }
}

sf::Vector2f RayCasting::calculateAvoidanceDirection(Unit& movingUnit, const std::vector<Unit>& blockingUnits)
{
    sf::Vector2f avoidanceVector(0.f, 0.f);

    for (const Unit& blocker : blockingUnits)
    {
        std::cout << blockingUnits.size() << std::endl;
        sf::Vector2f toBlocker = movingUnit.position - blocker.position;
        float distance = std::sqrt(toBlocker.x * toBlocker.x + toBlocker.y * toBlocker.y);

        if (distance > 0.0f)
        {
            // Normalize and scale inversely proportional to distance
            toBlocker /= distance;
            avoidanceVector += toBlocker / distance;
        }
    }

    // Normalize the avoidance vector
    float length = std::sqrt(avoidanceVector.x * avoidanceVector.x + avoidanceVector.y * avoidanceVector.y);
    if (length > 0.0f)
    {
        avoidanceVector /= length;
    }

    // Determine the side to steer (left or right) based on the unit's current velocity
    sf::Vector2f perpendicular(-movingUnit.velocity.y, movingUnit.velocity.x);
    float dotProduct = avoidanceVector.x * perpendicular.x + avoidanceVector.y * perpendicular.y;

    if (dotProduct < 0)
    {
        perpendicular = -perpendicular; // Steer to the opposite side
    }

    return perpendicular;
}

bool RayCasting::rayIntersectsCircle(const sf::Vector2f& A, const sf::Vector2f& B, const sf::Vector2f& C, float totalRadius) 
{
    const sf::Vector2f AB = B - A;  // Ray direction
    const sf::Vector2f AC = C - A;  // Vector from ray start to circle center

    // Project AC onto AB to find the closest point on the ray to the circle center
    float abLengthSq = AB.x * AB.x + AB.y * AB.y;
    float t = (AC.x * AB.x + AC.y * AB.y) / abLengthSq;

    // Clamp t to the ray segment [0, 1]
    t = std::clamp(t, 0.0f, 1.0f);

    // Find the closest point on the ray to the circle center
    sf::Vector2f closestPoint = A + t * AB;

    // Check if the closest point is within the circle's radius
    float dx = closestPoint.x - C.x;
    float dy = closestPoint.y - C.y;
    float distanceSq = dx * dx + dy * dy;

    return distanceSq <= (totalRadius * totalRadius);
}

sf::Vector2f RayCasting::normalize(const sf::Vector2f& vector)
{
    float length = std::sqrt(vector.x * vector.x + vector.y * vector.y);

    if (length != 0)
        return vector / length;

    return sf::Vector2f(0, 0);
}

float RayCasting::dot(const sf::Vector2f& a, const sf::Vector2f& b)
{
    return a.x * b.x + a.y * b.y;
}

float RayCasting::lengthSquared(const sf::Vector2f& v)
{
    return v.x * v.x + v.y * v.y;
}