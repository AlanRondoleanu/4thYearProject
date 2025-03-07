#include "RayCasting.h"

bool RayCasting::isBlocked(Unit movingUnit, std::vector<Unit> otherUnits)
{
    sf::Vector2f A = movingUnit.position;
    sf::Vector2f B = A + movingUnit.velocity * (movingUnit.radius / 2);
    float r1 = movingUnit.radius;
  

    // Check for collision with any other unit
    for (const Unit& otherUnit : otherUnits)
    {
        sf::Vector2f C = otherUnit.position;
        float r2 = otherUnit.radius;

        if (rayIntersectsCircle(A, B, C, r1 + r2))
        {
            return true;
        }
    }

    return false;
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