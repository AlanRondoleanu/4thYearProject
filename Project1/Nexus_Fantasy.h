#ifndef NEXUS_FANTASY_H
#define NEXUS_FANTASY_H

#include "Buildings.h"

class Nexus_Fantasy : public Buildings
{
public:
	Nexus_Fantasy(const sf::Vector2f& position, bool isEnemy);
	void update(float t_deltaTime) override;
	std::string GetUnitType() const override;

private:
};
#endif