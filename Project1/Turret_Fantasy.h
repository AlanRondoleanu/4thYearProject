#ifndef TURRET_FANTASY_H
#define TURRET_FANTASY_H

#include "Buildings.h"

class Turret_Fantasy : public Buildings
{
public:
	Turret_Fantasy(const sf::Vector2f& position, bool isEnemy);
	void update(float t_deltaTime) override;
	std::string GetUnitType() const override;

private:
};
#endif