#include "RayCasting.h"

std::vector<Unit> RayCasting::getBlockingUnits(Unit movingUnit, std::vector<Unit> otherUnits)
{
    std::vector<Unit> blockingUnits;  // List to store blocking units

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
            // If a collision is detected, add the unit to the blockingUnits list
            blockingUnits.push_back(otherUnit);
        }
    }

    return blockingUnits;
}

sf::Vector2f RayCasting::calculateAvoidanceDirection(const Unit movingUnit, const std::vector<Unit> blockingUnits)
{
    if (blockingUnits.empty()) 
    {
        return movingUnit.velocity;  // No blocking units, keep current velocity
    }

    // Calculate the average position and combined radius of all blocking units
    sf::Vector2f averagePosition(0, 0);
    float combinedRadius = 0.0f;

    for (Unit unit : blockingUnits) 
    {
        averagePosition += unit.position;
        combinedRadius += unit.radius;
    }
    averagePosition /= static_cast<float>(blockingUnits.size());

    // Check if the moving unit is inside the combined circle
    sf::Vector2f toCenter = averagePosition - movingUnit.position;
    float distanceToCenterSq = toCenter.x * toCenter.x + toCenter.y * toCenter.y;
    float combinedRadiusSq = combinedRadius * combinedRadius;
    float length = std::sqrt(movingUnit.velocity.x * movingUnit.velocity.x + movingUnit.velocity.y * movingUnit.velocity.y);

    if (distanceToCenterSq < combinedRadiusSq) {
        // Move away from the center until outside the combined circle
        float distanceToCenter = std::sqrt(distanceToCenterSq);
        sf::Vector2f direction = toCenter / distanceToCenter;  // Normalized direction away from center

        return direction * length;  // Move away at the same speed
    }

    // Calculate a tangent direction to move around the combined circle
    sf::Vector2f tangentDirection(-toCenter.y, toCenter.x);  // Perpendicular to the direction to the center
    tangentDirection /= std::sqrt(tangentDirection.x * tangentDirection.x + tangentDirection.y * tangentDirection.y);  // Normalize

    return tangentDirection * length;  // Move tangentially at the same speed
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