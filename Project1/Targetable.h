#ifndef TARGETABLE_H
#define TARGETABLE_H

#include <SFML/Graphics.hpp>
#include "UnitStats.h"

class Targetable 
{
public:
    virtual ~Targetable() = default;

    virtual sf::Vector2f getPos() const = 0;
    virtual float getRadius() const = 0; // If they need collision/range checks
    virtual void takeDamage(float amount) = 0;
    virtual bool getAlive() const = 0;
    virtual UnitStats getUnitStats() const = 0;
    virtual int getCellID() const = 0;
    virtual std::string GetUnitType() const = 0;


};

#endif