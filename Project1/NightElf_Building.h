#ifndef NIGHTELF_BUILDING_H
#define NIGHTELF_BUILDING_H

#include "Buildings.h"

class NightElf_Building : public Buildings
{
public:
	NightElf_Building(const sf::Vector2f& position, bool isEnemy);
	void update(float t_deltaTime) override;
	std::string GetUnitType() const override;
private:
};
#endif
