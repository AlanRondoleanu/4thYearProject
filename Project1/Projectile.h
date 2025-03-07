#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include "Units.h"

class Projectile
{
protected:
    // Values
    sf::Vector2f position;
    sf::Vector2f direction;
    float speed;
    float damage;
    bool active;

    // Body
    sf::RectangleShape body;

public:
    Projectile(sf::Vector2f t_startPos, sf::Vector2f t_dir, float t_speed, float t_dmg, std::shared_ptr<Units> targetUnit);
    virtual ~Projectile() = default;

    virtual void Update(float deltaTime);
    virtual void Render(sf::RenderWindow& t_window);
    virtual void OnImpact() = 0;

    std::shared_ptr<Units> currentTarget;

    bool IsActive() const { return active; }
    bool IsNearTarget() const;
    void Deactivate() { active = false; }
    sf::Vector2f GetPosition() const { return position; }
    sf::RectangleShape GetBody() const { return body; }

private:

    
};

#endif