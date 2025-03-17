#ifndef SOLDIER_BUILDING_H
#define SOLDIER_BUILDING_H

#include "Buildings.h"

class Soldier_Building : public Buildings
{
public:
	Soldier_Building(const sf::Vector2f& position, bool isEnemy);
	void update(float t_deltaTime) override;
	std::string GetUnitType() const override;
private:
};
#endif
