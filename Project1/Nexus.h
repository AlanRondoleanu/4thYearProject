#ifndef NEXUS_H
#define NEXUS_H

#include "Buildings.h"

class Nexus : public Buildings
{
public:
	Nexus(const sf::Vector2f& position, bool isEnemy);
	void update(float t_deltaTime) override;
	std::string GetUnitType() const override;

private:
};
#endif