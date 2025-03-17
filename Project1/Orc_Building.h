#ifndef ORC_BUILDING_H
#define ORC_BUILDING_H

#include "Buildings.h"

class Orc_Building : public Buildings
{
public:
	Orc_Building(const sf::Vector2f& position, bool isEnemy);
	void update(float t_deltaTime) override;
	std::string GetUnitType() const override;
private:
};
#endif
