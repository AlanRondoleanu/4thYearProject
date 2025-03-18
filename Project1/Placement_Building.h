#ifndef PLACEMENT_BUILDING_H
#define PLACEMENT_BUILDING_H

#include "Buildings.h"

class Placement_Building : public Buildings
{
public:
	Placement_Building(const sf::Vector2f& position, bool isEnemy);
	void update(float t_deltaTime) override;
	std::string GetUnitType() const override {
		return "Placement_Building";
	}
private:
};
#endif