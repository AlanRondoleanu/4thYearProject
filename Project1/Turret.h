#ifndef TURRET_H
#define TURRET_H

#include "Buildings.h"

class Turret : public Buildings
{
public:
	Turret(const sf::Vector2f& position, bool isEnemy);
	void update(float t_deltaTime) override;
	std::string GetUnitType() const override;

private:
};
#endif